#include "../include/ir/transform_pass/mem2reg.hpp"

void Mem2Reg::run() {
    promoteMemoryToRegister(*func);
}
void Mem2Reg::promoteMemoryToRegister(Func& F) {
    std::vector<BasicBlock*> workList;
    for (auto &BB : F.getBasicBlocks()) {
        for (auto &inst : BB.getInstructions()) {
            if (inst.getOpType() == Instruction::Alloca) {
                workList.push_back(&BB);
                break;
            }
        }
    }
    while (!workList.empty()) {
        BasicBlock* BB = workList.back();
        workList.pop_back();
        for (auto &inst : BB->getInstructions()) {
            if (inst.getOpType() == Instruction::Alloca) {
                std::vector<Instruction*> toBeDeleted;
                std::vector<Instruction*> toBeInserted;
                std::vector<Instruction*> toBeReplaced;
                std::vector<Instruction*> toBeReplacedWith;
                for (auto &user : inst.getUsers()) {
                    if (user->getOpType() == Instruction::Store) {
                        toBeDeleted.push_back(user);
                        toBeInserted.push_back(new StoreInst(user->getOperand(0), user->getOperand(1), user->getParent()));
                    }
                    else if (user->getOpType() == Instruction::Load) {
                        toBeReplaced.push_back(user);
                        toBeReplacedWith.push_back(new LoadInst(inst, user->getParent()));
                    }
                }
                for (auto &inst : toBeDeleted) {
                    inst->eraseFromParent();
                }
                for (auto &inst : toBeInserted) {
                    BB->addInstruction(inst);
                }
                for (int i = 0; i < toBeReplaced.size(); i++) {
                    toBeReplaced[i]->replaceAllUsesWith(toBeReplacedWith[i]);
                    toBeReplaced[i]->eraseFromParent();
                }
            }
        }
    }

}