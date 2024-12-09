#include "../BasicClass.hpp"
#include "../Type.hpp"
#include "../CFG.hpp"

class dominant {
    Func* func;
    std::map<BasicBlock*,int> num;
    std::map<int, BasicBlock*> reverse_num; // 反向编号 -> 节点映射
    std::map<BasicBlock*,BasicBlock*> idom;
    std::map<BasicBlock*,std::set<BasicBlock*>> dom;
    int dfs_num;
public:
    dominant(Func* _func) : func(_func),dfs_num(0) {}
    void run();
    void dfs(BasicBlock* bb);
    void tarjan();
    BasicBlock* intersect(BasicBlock* b1, BasicBlock* b2);
};
