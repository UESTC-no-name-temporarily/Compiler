#pragma once
#include "Type.hpp" 
#include <vector>
#include <algorithm>
#include <set>
#include "../utils/Singleton.hpp"
#include <memory>
#include "BasicClass.hpp"
class Variable;

class Instruction{
    
};

class BasicBlock{

};

class Function{
    Function()=default;
    ~Function()=default;
    using Paramptr=std::unique_ptr<Value>;
    using BBptr=std::unique_ptr<BasicBlock>;
    std::vector<Paramptr> ParamList;
    std::vector<BBptr> BBList;
    public:
    enum tag{
        Normal,
        UnrollBody,
        LoopBody,
        ParallelBody,
        BuildIn,
    };

};

class Module{
private:
    using Functionptr=std::unique_ptr<Function>;
    using GlobalVariableptr=std::unique_ptr<Variable>;
    std::vector<Functionptr> Functionlist;
    std::vector<GlobalVariableptr> GlobalVariableList;
public:
    Module()=default;
    std::set<Function*> hasinlinedFunc;
    std::set<Function*> inlinedFunc;
    std::set<Function*> sideeffectFunc;
    std::vector<Function*> getFunctionList();
    Function& FunctionGen();
};


