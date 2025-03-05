#include "../include/ir/BasicClass.hpp"
#include "../include/ir/Type.hpp"
#include "../include/ir/CFG.hpp"
#include "../include/ir/analysis_pass/dominant.hpp"

void dominant::run() {
    // 初始化
    for (auto bb = func->begin(); bb != func->end(); ++bb) {
        bb.operator*()->setVisited(false);
        num[*bb] = -1;
    }

    // 1. DFS 编号
    //TODO 应改为入口块而非起始块
    BasicBlock *entry = func->begin().operator*();
    dfs(entry);

    // 2. Tarjan 算法求解支配关系
    tarjan();
}
void dominant::dfs(BasicBlock *bb){
    bb->setVisited(true);
    num[bb]=dfs_num;
    reverse_num[dfs_num]=bb;
    dfs_num++;

    for(auto suc=bb->getNext().begin();suc!=bb->getNext().end();suc++){
        if(bb->checkVisited()){
            dfs(*suc);
        }
    }
    return ;
}
void dominant::tarjan() {
    // 初始化
    int total_blocks = dfs_num;
    std::vector<BasicBlock*> reverse_postorder(total_blocks);

    // 倒序 DFS 编号
    for (auto& [bb, id] : num) {
        reverse_postorder[id] = bb;
    }

    // 初始化 idom 的入口节点
    idom[reverse_postorder[0]] = reverse_postorder[0];

    bool changed = true;
    while (changed) {
        changed = false;

        // 遍历所有基本块，跳过入口块
        for (int i = 1; i < total_blocks; ++i) {
            auto* bb = reverse_postorder[i];
            BasicBlock* new_idom = nullptr;

            // 遍历前驱节点
            for (auto* pred : bb->getPrev()) {
                if (idom.find(pred) != idom.end()) { // 只有已经计算过 idom 的前驱才考虑
                    new_idom = new_idom ? intersect(new_idom, pred) : pred;
                }
            }

            if (idom[bb] != new_idom) {
                idom[bb] = new_idom;
                changed = true;
            }
        }
    }

    // 构造支配集合
    for (auto& [bb, dom_set] : dom) {
        dom_set.clear();
    }

    for (auto& [bb, imm_dom] : idom) {
        if (bb != imm_dom) {
            dom[imm_dom].insert(bb);
        }
    }
}

BasicBlock* dominant::intersect(BasicBlock* b1, BasicBlock* b2) {
    // 交叉向上搜索最近公共祖先
    while (b1 != b2) {
        if (num[b1] > num[b2]) {
            b1 = idom[b1];
        } else {
            b2 = idom[b2];
        }
    }
    return b1;
}