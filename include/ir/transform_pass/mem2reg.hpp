#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Mem2Reg {
public:
    void run();
private:
    void promoteMemoryToRegister(Function &F);

};