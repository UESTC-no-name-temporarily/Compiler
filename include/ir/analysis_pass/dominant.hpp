#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include "../BasicClass.hpp"
#include "../Type.hpp"
#include "../CFG.hpp"
#include "../New_passManager.hpp"
#include "PassBase.hpp"

class dominant : public AnalysisBase<dominant, Func> {
public:
    struct DominanceResult {
        std::unordered_map<int, int> idoms;              // bb_id -> idom_bb_id
        std::unordered_map<int, std::vector<int>> doms;   // 支配集合
        std::unordered_map<int, std::set<int>> dominance_frontier; // 支配边界
    };

private:
    struct Node {
        int id;
        Node* idom = nullptr;
        Node* semi = nullptr;
        Node* parent = nullptr;
        int label = -1;
        
        std::vector<Node*> pred;
        std::vector<Node*> succ;
        std::vector<Node*> doms;

        explicit Node(int id) : id(id) {}
    };

    Node* entry = nullptr;
    std::unordered_map<int, Node*> nodes;
    std::vector<Node*> vertex;
    int dfs_time = 0;
    DominanceResult* result = nullptr;

    // 并查集结构
    struct UnionFind {
        std::unordered_map<Node*, Node*> parent;
        std::unordered_map<Node*, Node*> ancestor;
        std::unordered_map<Node*, Node*> best;
        std::unordered_map<Node*, int> semi;

        Node* find(Node* v) {
            if (!parent.count(v)) {
                parent[v] = v;
                ancestor[v] = nullptr;
                best[v] = v;
                return v;
            }
            
            if (ancestor[v] != nullptr) {
                Node* u = find(ancestor[v]);
                ancestor[v] = ancestor[u];
                if (semi[best[u]] < semi[best[v]]) {
                    best[v] = best[u];
                }
            }
            return parent[v];
        }
    };

    void init(Func* F) {
        // 根据Function初始化节点
        for (auto& bb : F->basic_blocks()) {
            Node* node = new Node(bb->id());
            nodes[bb->id()] = node;
            if (bb->is_entry()) entry = node;
        }

        // 构建前驱后继关系
        for (auto& bb : F->basic_blocks()) {
            Node* cur = nodes[bb->id()];
            for (auto* succ_bb : bb->successors()) {
                cur->succ.push_back(nodes[succ_bb->id()]);
                nodes[succ_bb->id()]->pred.push_back(cur);
            }
        }
    }

    void dfs(Node* u) {
        u->label = dfs_time++;
        vertex.push_back(u);
        for (Node* v : u->succ) {
            if (v->label == -1 && v != entry) {
                v->parent = u;
                dfs(v);
            }
        }
    }

    Node* eval(Node* v, UnionFind& uf) {
        if (uf.ancestor[v] == nullptr)
            return v;
        uf.find(v);
        Node* u = uf.best[v];
        if (uf.semi[uf.best[uf.ancestor[v]]] < uf.semi[u])
            u = uf.best[uf.ancestor[v]];
        return u;
    }

    void compute_dominance_frontier() {
        result->dominance_frontier.clear();
        for (auto& [bb_id, node] : nodes) {
            if (node->pred.size() > 1) {
                for (Node* runner : node->pred) {
                    while (runner != node->idom) {
                        result->dominance_frontier[runner->id].insert(node->id);
                        runner = runner->idom;
                    }
                }
            }
        }
    }

    void build_result() {
        result = new DominanceResult();
        // 转换节点指针为bb_id
        for (auto& [bb_id, node] : nodes) {
            if (node->idom) {
                result->idoms[bb_id] = node->idom->id;
            }
            for (Node* dom : node->doms) {
                result->doms[bb_id].push_back(dom->id);
            }
        }
        compute_dominance_frontier();
    }

public:
    explicit dominant(Func* F) { init(F); }
    
    ~dominant() {
        for (auto& pair : nodes) delete pair.second;
        delete result;
    }

    void run() override {
        if (result) return;
        // 步骤1: DFS遍历建立序号
        dfs(entry);
        
        UnionFind uf;
        std::unordered_map<Node*, std::vector<Node*>> bucket;
 
        // 初始化并查集
        for (auto& pair : nodes) {
            Node* v = pair.second;
            uf.semi[v] = v->label;
        }
 
        // 逆序遍历处理节点
        for (auto it = vertex.rbegin(); it != vertex.rend(); ++it) {
            Node* w = *it;
            if (w == entry) continue;
 
            // 步骤2: 计算半支配者
            for (Node* v : w->pred) {
                Node* u = eval(v, uf);
                if (uf.semi[u] < uf.semi[w])
                    uf.semi[w] = uf.semi[u];
            }
            bucket[vertex[uf.semi[w]]].push_back(w);

            // 连接w到父节点
            uf.parent[w] = w->parent;
            uf.ancestor[w] = w->parent;

            // 处理bucket中的节点
            for (Node* v : bucket[w->parent]) {
                Node* u = eval(v, uf);
                v->idom = (uf.semi[u] < uf.semi[v]) ? u : w->parent;
            }
            bucket[w->parent].clear();
        }

        // 步骤3: 确定最终idom
        for (auto it = vertex.rbegin(); it != vertex.rend(); ++it) {
            Node* w = *it;
            if (w != entry && w->idom != vertex[uf.semi[w]])
                w->idom = w->idom->idom;
        }
        entry->idom = nullptr;

        // 构建支配集合
        for (auto& pair : nodes) {
            Node* node = pair.second;
            if (node->idom)
                node->idom->doms.push_back(node);
        }
        build_result();
    }

    const DominanceResult& GetResult(Func* func) override {
        if (!result) run();
        return *result;
    }

    // 调试接口
    void dump_dominance_frontier() const {
        for (const auto& [bb_id, df_set] : result->dominance_frontier) {
            std::cout << "DF(BB" << bb_id << ") = { ";
            for (int df : df_set) 
                std::cout << df << " ";
            std::cout << "}\n";
        }
    }

    // Mem2Reg专用快速访问接口
    const std::set<int>& get_dominance_frontier(int bb_id) const {
        static std::set<int> empty;
        auto it = result->dominance_frontier.find(bb_id);
        return it != result->dominance_frontier.end() ? it->second : empty;
    }

    int get_idom(int bb_id) const {
        auto it = result->idoms.find(bb_id);
        return it != result->idoms.end() ? it->second : -1;
    }
};
