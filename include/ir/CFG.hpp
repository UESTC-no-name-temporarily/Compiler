#pragma once
#include "Type.hpp" 
#include <vector>
#include <algorithm>
#include <set>
#include "../utils/Singleton.hpp"
#include <memory>
#include "BasicClass.hpp"
class Variable;

class Inst:public User{
    enum InstType
    {
        None,
        // Terminators
        UnCond,
        Cond,
        Ret,
        // Memory
        Alloca,
        Load,
        Store,
        Memcpy,
        // Binary
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        And,
        Or,
        Xor,
        Eq,
        Ne,
        Ge,
        L,
        Le,
        G,
        // Other
        Gep,
        Phi,
        Call,
        Zext,
        Sext,
        Trunc,
        FP2SI,
        SI2FP,
        BinaryUnknown,
        Max,
        Min,
        Select,
    };
    InstType itype;
    bool IsTerminateInst();
    bool IsCondInst();
    bool IsUncondInst();
    bool IsMemoryInst();
    bool IsBinaryInst();
    bool HasSideEffect();
    
};

class BasicBlock:public Value{
    BasicBlock()=default;
    ~BasicBlock()=default;
    using Instptr=std::unique_ptr<Inst>;
    std::vector<Instptr> InstList;
    BasicBlock* next=nullptr;
    BasicBlock* prev=nullptr;
    public:
    void addInst(Inst* inst);
    void deleteInst(Inst* inst);
    void setNext(BasicBlock* _next);
    void setPrev(BasicBlock* _prev);
    BasicBlock* getNext();
    BasicBlock* getPrev();
    void print();
};

class Func:public Value{
    Func()=default;
    ~Func()=default;
    int bbcount=0;
    Func* next=nullptr;
    Func* prev=nullptr;
    bool has_side_effect=false;
    using Paramptr=std::unique_ptr<Value>;
    using BBptr=std::unique_ptr<BasicBlock>;
    std::vector<Paramptr> ParamList;//参数列表
    std::vector<BBptr> BBList;//基本块列表
    public:
    enum tag{
        Normal,
        UnrollBody,
        LoopBody,
        ParallelBody,
        BuildIn,
    };
    void addBB(BasicBlock* bb);
    void deleteBB(BasicBlock* bb);
    void addParam(Value* param);
    void deleteParam(Value* param);
    void setSideEffect(bool* _has_side_effect);
    bool hasSideEffect();
    void setTag(tag _tag);
    tag getTag();
    std::vector<BBptr> getBBList();
    std::vector<Paramptr> getParamList();
    Func* getNext();
    Func* getPrev();
    void print();
};

class Module{
private:
    using Funcptr=std::unique_ptr<Func>;
    using GlobalVariableptr=std::unique_ptr<Variable>;
    std::vector<Funcptr> Funclist;
    std::vector<GlobalVariableptr> GlobalVariableList;
public:
    Module()=default;
    std::set<Func*> hasinlinedFunc;
    std::set<Func*> inlinedFunc;
    std::set<Func*> sideeffectFunc;
    std::vector<Funcptr> getFunclist();
    std::vector<GlobalVariableptr> getGlobalVariableList();
    Func& FuncGen();
    void addFunc(Func* func);
    void deleteFunc(Func* func);
    void addGlobalVariable(Variable* globalVariable);
    void deleteGlobalVariable(Variable* globalVariable);
    void print();
};


