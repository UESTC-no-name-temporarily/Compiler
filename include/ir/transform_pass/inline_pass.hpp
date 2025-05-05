#pragma once
#include "PassBase.hpp"
#include "BasicClass.hpp"
#include "../Analysis/InlineCost.hpp"
#include <vector>
#include <unordered_set>

class InlinePass : public PassBase<InlinePass, Function> {
public:
  bool Run(Function& F) {
    bool Changed = false;
    auto& AM = getAnalysisManager();
    
    // 获取内联代价分析结果
    auto& ICA = AM.getResult<InlineCostAnalysis>(F);
    
    // 收集需要内联的调用点
    std::vector<CallInst*> Worklist;
    for (auto& BB : F) {
      for (auto& Inst : BB) {
        if (auto* CI = dynamic_cast<CallInst*>(&Inst)) {
          if (shouldInline(CI, ICA)) {
            Worklist.push_back(CI);
          }
        }
      }
    }
    
    // 处理内联
    for (auto* CI : Worklist) {
      Changed |= inlineCall(CI);
    }
    
    return Changed;
  }

private:
  // 判断是否应该内联该调用
  bool shouldInline(CallInst* CI, InlineCostAnalysis& ICA) {
    Function* Callee = CI->getCalledFunction();
    
    // 排除不可内联的情况
    if (!Callee || Callee->isDeclaration() || Callee->isVarArg()) 
      return false;
    
    // 检查递归调用
    if (Callee == CI->getFunction()) 
      return false;
    
    // 使用代价模型决策
    return ICA.getInlineCost(CI) <= InlineThreshold;
  }

  // 实际内联操作
  bool inlineCall(CallInst* CI) {
    Function* Caller = CI->getFunction();
    Function* Callee = CI->getCalledFunction();
    
    // 步骤1: 克隆被调用函数体
    ValueMap<Value*, Value*> VMap;
    Function* ClonedFunc = cloneFunction(Callee, VMap);
    
    // 步骤2: 参数替换
    for (size_t i = 0; i < CI->arg_size(); ++i) {
      Value* ArgVal = CI->getArgOperand(i);
      Value* MappedArg = VMap[Callee->getArg(i)];
      MappedArg->replaceAllUsesWith(ArgVal);
    }
    
    // 步骤3: 处理返回指令
    BasicBlock* OrigBB = CI->getParent();
    BasicBlock* SplitBB = OrigBB->splitBasicBlock(CI);
    BasicBlock* ReturnBB = nullptr;
    
    // 收集所有返回点并重定向
    std::vector<ReturnInst*> Returns;
    for (auto& BB : *ClonedFunc) {
      if (auto* RI = BB.getTerminatingReturn()) {
        Returns.push_back(RI);
      }
    }
    
    // 创建统一的返回块
    if (!Returns.empty()) {
      ReturnBB = BasicBlock::Create("inline.return");
      PHINode* Phi = nullptr;
      if (CI->getType()->isVoidTy()) {
        ReturnBB->createBr(SplitBB);
      } else {
        Phi = PHINode::Create(CI->getType());
        ReturnBB->insertFront(Phi);
        ReturnBB->createBr(SplitBB);
      }
      
      // 重定向返回指令
      for (auto* RI : Returns) {
        BasicBlock* PredBB = RI->getParent();
        if (Phi) {
          Phi->addIncoming(RI->getReturnValue(), PredBB);
        }
        PredBB->getTerminator()->replaceSuccessor(RI->getParent(), ReturnBB);
      }
    }
    
    // 步骤4: 连接基本块
    OrigBB->getTerminator()->replaceSuccessor(SplitBB, ClonedFunc->getEntryBlock());
    ClonedFunc->getExitBlock()->replaceSuccessor(nullptr, ReturnBB ? ReturnBB : SplitBB);
    
    // 步骤5: 清理调用指令
    CI->eraseFromParent();
    
    // 标记相关分析结果失效
    invalidateAnalyses();
    
    return true;
  }

  // 辅助函数：克隆函数体
  Function* cloneFunction(Function* F, ValueMap<Value*, Value*>& VMap) {
    Function* Clone = new Function(F->getName() + ".inline");
    // ...实现深拷贝逻辑...
    return Clone;
  }

  // 辅助函数：标记分析结果失效
  void invalidateAnalyses() {
    auto& AM = getAnalysisManager();
    AM.invalidate<DominatorTreeAnalysis>();
    AM.invalidate<CallGraphAnalysis>();
  }

  static constexpr int InlineThreshold = 50; // 内联阈值
};