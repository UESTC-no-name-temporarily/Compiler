#include "./include/ir/CFG.hpp"
#include <iostream>
#include "./include/ir/BasicClass.hpp"

void BasicBlock::addInst(Inst* inst){
    //TODO need to be modified
    instList.push_back(inst);
    inst->setParent(this);
}
void BasicBlock::deleteInst(Inst* inst){
    auto it = std::find(instList.begin(), instList.end(), inst);
    if (it != instList.end()) {
        instList.erase(it);
    }
}
void BasicBlock::dump() const{
    std::cout << "BasicBlock: Name = " << GetName() << "\n";
    for (const auto& inst : instList) {
        inst->dump();
    }
}
void Func::addBB(BasicBlock* bb){
    //TODO need to be modified
    bbList.push_back(bb);
    bb->setParent(this);
}
void Func::deleteBB(BasicBlock* bb){
    auto it = std::find(bbList.begin(), bbList.end(), bb);
    if (it != bbList.end()) {
        bbList.erase(it);
    }
}
void Func::dump() const{
    std::cout << "Function:\n";
    for (const auto& bb : bbList) {
        bb->dump();
    }
}
void Module::addFunc(Func* func){
    //TODO need to be modified
    funcList.push_back(func);
    func->setParent(this);
}
void Module::deleteFunc(Func* func){
    auto it = std::find(funcList.begin(), funcList.end(), func);
    if (it != funcList.end()) {
        funcList.erase(it);
    }
}
void Module::dump() const{
    std::cout << "Module:\n";
    for (const auto& func : funcList) {
        func->dump();
    }
}