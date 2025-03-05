#include <iostream>
#include "../BasicClass.hpp"
#include "../CFG.hpp"
#include "../New_passManager.hpp"
class DCE : public PassBase<DCE, Func> {
    Func *func;
    DCE(Func &F) {
        func = &F;
    }
public:
    void run();

};
