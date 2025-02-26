#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../BasicClass.hpp"
#include "../CFG.hpp"
class Mem2Reg {
    Func *func;
    Mem2Reg(Func &F) {
        func = &F;
    }
    
public:
    void run();
private:
    void promoteMemoryToRegister(Func &F);

};