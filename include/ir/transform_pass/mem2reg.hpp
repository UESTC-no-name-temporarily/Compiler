#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../BasicClass.hpp"
#include "../CFG.hpp"
#include "../New_passManager.hpp"
class Mem2Reg :public PassBase<Mem2Reg, Func> {
    Func *func;
    Mem2Reg(Func &F) {
        func = &F;
    }
    void promoteMemoryToRegister(Func &F);
public:
    void run();
};