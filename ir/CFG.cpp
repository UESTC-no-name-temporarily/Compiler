#include "./include/ir/CFG.hpp"
#include <iostream>
#include "./include/ir/BasicClass.hpp"

void BasicBlock::addInst(Inst* inst){
    //TODO need to be modified
    instList.emplace_back(inst);
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
    bbList.emplace_back(bb);
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
    funcList.emplace_back(func);
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