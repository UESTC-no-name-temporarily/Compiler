#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include "../BasicClass.hpp"
#include "../Type.hpp"
#include "../CFG.hpp"
#include "../passManager.hpp"

class dominant : public AnalysisPassManager {
public:
    struct Node {
        int id;
        Node* idom = nullptr;     // 直接支配节点
        Node* semi = nullptr;     // 半支配节点
        Node* parent = nullptr;   // DFS树父节点
        int label = -1;           // DFS序号
        
        std::vector<Node*> pred;  // 前驱节点
        std::vector<Node*> succ;  // 后继节点
        std::vector<Node*> doms;  // 支配集合
        
        explicit Node(int id) : id(id) {}
    };

private:
    Node* entry = nullptr;                  // 入口节点
    std::unordered_map<int, Node*> nodes;   // 所有节点
    std::vector<Node*> vertex;              // DFS遍历顺序
    int dfs_time = 0;                       // DFS时间戳

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

public:
    explicit dominant(Func* F) {
        init(F);
        run();
    }

    void run() {
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
    }

    // 查询接口
    Node* get_idom(int bb_id) const {
        auto it = nodes.find(bb_id);
        return it != nodes.end() ? it->second->idom : nullptr;
    }

    const std::vector<Node*>& get_dominators(int bb_id) const {
        static std::vector<Node*> empty;
        auto it = nodes.find(bb_id);
        return it != nodes.end() ? it->second->doms : empty;
    }

    // 调试输出
    void dump() const {
        for (auto& pair : nodes) {
            Node* node = pair.second;
            std::cout << "BB" << node->id << ":\n";
            std::cout << "  idom: " << (node->idom ? node->idom->id : -1) << "\n";
            std::cout << "  Dominates: ";
            for (Node* d : node->doms)
                std::cout << d->id << " ";
            std::cout << "\n\n";
        }
    }
};
