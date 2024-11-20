#pragma once
#include "Type.hpp" 
#include <vector>
#include <algorithm>
#include <set>
#include "../utils/Singleton.hpp"
#include <memory>
#include "BasicClass.hpp"
class Variable:public User{

};
//TODO generate by ai, need to be modified
class AllocaInst:public Inst{
    Type* type;
    int num;
    AllocaInst(Type* _type,int _num):type(_type),num(_num){
        itype=InstType::Alloca;
    }
    Type* getType();
    int getNum();
};
class StoreInst:public Inst{
    Value* value;
    Value* pointer;
    StoreInst(Value* _value,Value* _pointer):value(_value),pointer(_pointer){
        itype=InstType::Store;
    }
    Value* getValue();
    Value* getPointer();
};
class LoadInst:public Inst{
    Value* pointer;
    LoadInst(Value* _pointer):pointer(_pointer){
        itype=InstType::Load;
    }
    Value* getPointer();
};
class CallInst:public Inst{
    Func* func;
    std::vector<Value*> args;
    CallInst(Func* _func,std::vector<Value*> _args):func(_func),args(_args){
        itype=InstType::Call;
    }
    Func* getFunc();
    std::vector<Value*> getArgs();
};
class CondInst:public Inst{
    Value* cond;
    BasicBlock* then;
    BasicBlock* els;
    CondInst(Value* _cond,BasicBlock* _then,BasicBlock* _els):cond(_cond),then(_then),els(_els){
        itype=InstType::Cond;
    }
    Value* getCond();
    BasicBlock* getThen();
    BasicBlock* getEls();
};
class UnCondInst:public Inst{
    BasicBlock* dest;
    UnCondInst(BasicBlock* _dest):dest(_dest){
        itype=InstType::UnCond;
    }
    BasicBlock* getDest();
};
class RetInst:public Inst{
    Value* value;
    RetInst(Value* _value):value(_value){
        itype=InstType::Ret;
    }
    Value* getValue();
};
class BinaryInst:public Inst{
    Value* lhs;
    Value* rhs;
    BinaryInst(Value* _lhs,Value* _rhs){
        lhs=_lhs;
        rhs=_rhs;
    }
    Value* getLhs();
    Value* getRhs();
};
class ZextInst:public Inst{
    Value* value;
    ZextInst(Value* _value):value(_value){
        itype=InstType::Zext;
    }
    Value* getValue();
};//Zero extend进行零扩展
class SextInst:public Inst{
    Value* value;
    SextInst(Value* _value):value(_value){
        itype=InstType::Sext;
    }
    Value* getValue();
};//Sign extend进行符号扩展
class PhiInst:public Inst{
    std::vector<Value*> values;
    std::vector<BasicBlock*> blocks;
    PhiInst(std::vector<Value*> _values,std::vector<BasicBlock*> _blocks):values(_values),blocks(_blocks){
        itype=InstType::Phi;
    }
    std::vector<Value*> getValues();
    std::vector<BasicBlock*> getBlocks();
};

class Inst:public User{
public:
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
    std::vector<BBptr> getBBList();

    void addParam(Value* param);
    void deleteParam(Value* param);
    std::vector<Paramptr> getParamList();
    
    void setTag(tag _tag);
    tag getTag();

    void setSideEffect(bool* _has_side_effect);
    bool hasSideEffect();

    Func* getNext();
    Func* getPrev();
    void dump();
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
    void deleteFunc(Func* func);
    Func& getmain();
    void addGlobalVariable(Variable* globalVariable);
    void deleteGlobalVariable(Variable* globalVariable);
    void dump();
};


