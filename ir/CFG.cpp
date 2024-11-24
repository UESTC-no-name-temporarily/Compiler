#include "./include/ir/CFG.hpp"
#include <iostream>
#include "./include/ir/BasicClass.hpp"

void BasicBlock::addInst(Inst* inst){
    insts.push_back(inst);
    inst->setParent(this);
}
void BasicBlock::deleteInst(Inst* inst){
    auto it = std::find(insts.begin(), insts.end(), inst);
    if (it != insts.end()) {
        insts.erase(it);
    }
}
void BasicBlock::dump() const{
    std::cout << "BasicBlock: Name = " << name << "\n";
    for (const auto& inst : insts) {
        inst->dump();
    }
}
void Func::addBB(BasicBlock* bb){
    bbs.push_back(bb);
    bb->setParent(this);
}
void Func::deleteBB(BasicBlock* bb){
    auto it = std::find(bbs.begin(), bbs.end(), bb);
    if (it != bbs.end()) {
        bbs.erase(it);
    }
}
void Func::dump() const{
    std::cout << "Function:\n";
    for (const auto& bb : bbs) {
        bb->dump();
    }
}
void Module::addFunc(Func* func){
    funcs.push_back(func);
    func->setParent(this);
}
void Module::deleteFunc(Func* func){
    auto it = std::find(funcs.begin(), funcs.end(), func);
    if (it != funcs.end()) {
        funcs.erase(it);
    }
}
void Module::dump() const{
    std::cout << "Module:\n";
    for (const auto& func : funcs) {
        func->dump();
    }
}