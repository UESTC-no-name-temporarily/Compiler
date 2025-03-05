#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../BasicClass.hpp"
#include "../CFG.hpp"
#include "../New_passManager.hpp"
#include "../analysis_pass/dominant.hpp"
#include <vector>
#include <set>
#include <stack>
#include <unordered_map>

class Mem2Reg : public PassBase<Mem2Reg, Func> {
private:
    Func& F;
    dominant& Dom;
    
    // 核心数据结构
    struct VariableVersion {
        std::stack<Value*> VersionStack; // 当前变量版本栈
        std::vector<PhiInst*> PhiNodes;  // 关联的Phi指令
    };
    
    std::unordered_map<AllocaInst*, VariableVersion> AllocaVersions;

public:
    Mem2Reg(Func& func, dominant& dom) : F(func), Dom(dom) {}

    void run() {
        // 步骤1: 收集可提升的alloca
        std::vector<AllocaInst*> PromotableAllocas = collectPromotableAllocas();
        
        for (auto* AI : PromotableAllocas) {
            // 步骤2: 计算支配边界并插入Phi
            insertPhiNodes(AI);
            
            // 步骤3: 变量重命名
            renameVariables(AI);
            
            // 步骤4: 清理旧指令
            removeAllocaRelatedInsts(AI);
        }
    }

private:
    //===--------------------------------------------------------------------===//
    // 步骤1: 识别可提升的Alloca指令
    //===--------------------------------------------------------------------===//
    std::vector<AllocaInst*> collectPromotableAllocas() {
        std::vector<AllocaInst*> Promotable;
        for (auto& BB : Func) {
            for (auto& Inst : BB) {
                if (auto* AI = dynamic_cast<AllocaInst*>(&Inst)) {
                    if (isPromotable(AI)) {
                        Promotable.push_back(AI);
                    }
                }
            }
        }
        return Promotable;
    }

    bool isPromotable(AllocaInst* AI) {
        // 条件1: 没有被取地址操作
        for (auto* User : AI->users()) {
            if (!isa<LoadInst>(User) && !isa<StoreInst>(User)) {
                return false;
            }
        }
        return true;
    }

    //===--------------------------------------------------------------------===//
    // 步骤2: 插入Phi节点（关键支配边界计算）
    //===--------------------------------------------------------------------===//
    void insertPhiNodes(AllocaInst* AI) {
        std::set<BasicBlock*> DefBlocks = collectDefBlocks(AI);
        std::set<BasicBlock*> WorkList = DefBlocks;
        std::set<BasicBlock*> HasPhi;

        while (!WorkList.empty()) {
            auto* BB = *WorkList.begin();
            WorkList.erase(WorkList.begin());

            // 计算当前块的支配边界
            for (auto* FrontierBB : computeDominanceFrontier(BB)) {
                if (HasPhi.insert(FrontierBB).second) {
                    // 插入Phi节点
                    PhiInst* Phi = createPhi(FrontierBB, AI->getType());
                    AllocaVersions[AI].PhiNodes.push_back(Phi);
                    
                    // 将支配边界加入工作列表
                    WorkList.insert(FrontierBB);
                }
            }
        }
    }

    // 关键算法：计算基本块的支配边界
    std::set<BasicBlock*> computeDominanceFrontier(BasicBlock* BB) {
        std::set<BasicBlock*> DF;
        for (auto& OtherBB : Func) {
            // 条件1: OtherBB至少有一个前驱被BB支配
            // 条件2: OtherBB不被BB支配
            bool HasDominatingPred = false;
            for (auto* Pred : OtherBB.predecessors()) {
                if (ifdominant(BB, Pred)) { // 使用你的支配判断接口
                    HasDominatingPred = true;
                    break;
                }
            }
            if (HasDominatingPred && !ifdominant(BB, &OtherBB)) {
                DF.insert(&OtherBB);
            }
        }
        return DF;
    }

    //===--------------------------------------------------------------------===//
    // 步骤3: 变量重命名（使用支配树遍历顺序）
    //===--------------------------------------------------------------------===//
    void renameVariables(AllocaInst* AI) {
        auto& VersionInfo = AllocaVersions[AI];
        VersionInfo.VersionStack.push(UndefValue::get(AI->getType()));

        // 按支配树顺序遍历基本块
        std::vector<BasicBlock*> DomOrder = getDomTreeOrder();
        for (auto* BB : DomOrder) {
            // 处理Phi节点（设置初始版本）
            for (auto* Phi : VersionInfo.PhiNodes) {
                if (Phi->getParent() == BB) {
                    VersionInfo.VersionStack.push(Phi);
                }
            }

            // 处理块内指令
            for (auto& Inst : *BB) {
                if (auto* Store = dynamic_cast<StoreInst*>(&Inst)) {
                    if (Store->getPointerOperand() == AI) {
                        // 推入新版本
                        VersionInfo.VersionStack.push(Store->getValueOperand());
                    }
                } else if (auto* Load = dynamic_cast<LoadInst*>(&Inst)) {
                    if (Load->getPointerOperand() == AI) {
                        // 替换Load使用
                        Load->replaceAllUsesWith(VersionInfo.VersionStack.top());
                    }
                }
            }

            // 递归处理支配的子节点
            for (auto* Child : getDomChildren(BB)) {
                renameVariablesInChild(Child, AI);
            }

            // 回退版本
            for (auto& Inst : *BB) {
                if (auto* Store = dynamic_cast<StoreInst*>(&Inst)) {
                    if (Store->getPointerOperand() == AI) {
                        VersionInfo.VersionStack.pop();
                    }
                }
            }
        }
    }

//     //===--------------------------------------------------------------------===//
//     // 辅助接口实现（需你提供的功能）
//     //===--------------------------------------------------------------------===//
// private:
//     // 示例接口实现（需根据你的实际数据结构调整）
    
//     // 获取支配树遍历顺序（深度优先）
//     std::vector<BasicBlock*> getDomTreeOrder() {
//         std::vector<BasicBlock*> Order;
//         std::function<void(BasicBlock*)> DFS = [&](BasicBlock* BB) {
//             Order.push_back(BB);
//             for (auto* Child : getDomChildren(BB)) {
//                 DFS(Child);
//             }
//         };
//         DFS(&Func.getEntryBlock());
//         return Order;
//     }

//     // 获取直接支配的子节点
//     std::vector<BasicBlock*> getDomChildren(BasicBlock* BB) {
//         std::vector<BasicBlock*> Children;
//         for (auto& OtherBB : Func) {
//             if (Dom.getIDom(&OtherBB) == BB) { // 假设有getIDom接口
//                 Children.push_back(&OtherBB);
//             }
//         }
//         return Children;
//     }

//     // 创建Phi指令
//     PhiInst* createPhi(BasicBlock* BB, Type* Ty) {
//         auto* Phi = new PhiInst(Ty);
//         BB->insertFront(Phi); // 在基本块开始插入
//         return Phi;
//     }

//     // 清理相关指令
//     void removeAllocaRelatedInsts(AllocaInst* AI) {
//         std::vector<Instruction*> ToDelete;
//         for (auto* User : AI->users()) {
//             if (isa<LoadInst>(User) || isa<StoreInst>(User)) {
//                 ToDelete.push_back(cast<Instruction>(User));
//             }
//         }
//         for (auto* Inst : ToDelete) Inst->eraseFromParent();
//         AI->eraseFromParent();
//     }
// };

// //===--------------------------------------------------------------------===//
// // 外部调用接口
// //===--------------------------------------------------------------------===//
// void runMem2Reg(Function& F) {
//     auto* Dom = get<dominant>(F); // 你的支配分析接口
//     Mem2RegPass(F, *Dom).run();
// }