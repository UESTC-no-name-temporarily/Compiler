#pragma once
#include "Type.hpp" 
#include <vector>
#include <algorithm>
#include <set>
#include "../utils/Singleton.hpp"
#include <memory>
#include "BasicClass.hpp"
#include <../utils/List.hpp>
class Variable : public User {
    enum class VarType {
        Global,
        Const,
        Param,
    }vartype;
    bool isGlobalVar=false;
    bool isConst=false;
    bool isParam=false;
    Value* initializer; // 对于全局变量，可能会有初始值
public:
    // 构造函数
    Variable(std::string name,VarType _vartype, Type* type, Value* init = nullptr)
        : User(name, type),vartype(_vartype), initializer(init) {setinfo();}
    // 获取初始化
    Value* getInitializer() const {
        return initializer;
    }
    // 设置初始化值
    void setInitializer(Value* init) {
        initializer = init;
    }
    // 打印变量信息
    void dump() ;
    // 获取变量类型

    void setinfo(){
        if(vartype==VarType::Global)
            isGlobalVar=true;
        if(vartype==VarType::Const)
            isConst=true;
        if(vartype==VarType::Param)
            isParam=true;
    }

    virtual int getValueID() const override {
        //TODO need to be modified
    }
};

//TODO need to be modified
class AllocaInst:public Inst{
    Type* type;
    int num;
    AllocaInst(std::string name, Type* _type, int _num)
        : Inst(std::move(name), _type, InstType::Alloca), num(_num) {}
    Type* getType() const { return type; }
    int getNum() const { return num; }

    void AllocaInst::dump() ;
};

class StoreInst : public Inst {
    Value* value;   // 要存储的值
    Value* pointer; // 存储的地址

public:
    StoreInst(std::string name, Type* _type, Value* _value, Value* _pointer)
        : Inst(std::move(name), _type, InstType::Store), value(_value), pointer(_pointer) {}

    Value* getValue() const { return value; }
    Value* getPointer() const { return pointer; }

    void dump() ;
};

class LoadInst : public Inst {
    Value* pointer; // 要加载的地址

public:
    LoadInst(std::string name, Type* _type, Value* _pointer)
        : Inst(std::move(name), _type, InstType::Load), pointer(_pointer) {}

    Value* getPointer() const { return pointer; }

    void dump() ;
};

class CallInst : public Inst {
    Func* func;                    // 被调用的函数
    std::vector<Value*> arguments; // 参数列表

public:
    CallInst(std::string name, Type* _type, Func* _func, std::vector<Value*> _args)
        : Inst(std::move(name), _type, InstType::Call), func(_func), arguments(std::move(_args)) {}

    Func* getFunc() const { return func; }
    const std::vector<Value*>& getArgs() const { return arguments; }

    void dump() const override {
        std::cout << "CallInst: Func = " << func->GetName()
                  << ", Args = [";
        for (const auto& arg : arguments) {
            std::cout << arg->GetName() << ", ";
        }
        std::cout << "]\n";
    }
};

class CondInst : public Inst {
    Value* condition;
    BasicBlock* thenBlock;
    BasicBlock* elseBlock;

public:
    CondInst(std::string name, Type* _type, Value* _cond, BasicBlock* _then, BasicBlock* _else)
        : Inst(std::move(name), _type, InstType::Cond), condition(_cond), thenBlock(_then), elseBlock(_else) {}

    Value* getCond() const { return condition; }
    BasicBlock* getThen() const { return thenBlock; }
    BasicBlock* getEls() const { return elseBlock; }

    void dump() const override {
        std::cout << "CondInst: Cond = " << condition->GetName()
                  << ", Then = " << thenBlock->GetName()
                  << ", Else = " << elseBlock->GetName() << "\n";
    }
};

class UnCondInst : public Inst {
    BasicBlock* destBlock;

public:
    UnCondInst(std::string name, Type* _type, BasicBlock* _dest)
        : Inst(std::move(name), _type, InstType::UnCond), destBlock(_dest) {}

    BasicBlock* getDest() const { return destBlock; }

    void dump() const override {
        std::cout << "UnCondInst: Dest = " << destBlock->GetName() << "\n";
    }
};

class RetInst : public Inst {
    Value* returnValue;

public:
    RetInst(std::string name, Type* _type, Value* _value)
        : Inst(std::move(name), _type, InstType::Ret), returnValue(_value) {}

    Value* getValue() const { return returnValue; }

    void dump() const override {
        std::cout << "RetInst: Value = " << (returnValue ? returnValue->GetName() : "void") << "\n";
    }
};

class ZextInst : public Inst {
    Value* value;

public:
    ZextInst(std::string name, Type* _type, Value* _value)
        : Inst(std::move(name), _type, InstType::Zext), value(_value) {}

    Value* getValue() const { return value; }

    void dump() const override {
        std::cout << "ZextInst: Value = " << value->GetName() << "\n";
    }
};

class SextInst : public Inst {
    Value* value;

public:
    SextInst(std::string name, Type* _type, Value* _value)
        : Inst(std::move(name), _type, InstType::Sext), value(_value) {}

    Value* getValue() const { return value; }

    void dump() const override {
        std::cout << "SextInst: Value = " << value->GetName() << "\n";
    }
};

class PhiInst : public Inst {
    std::vector<Value*> incomingValues;
    std::vector<BasicBlock*> incomingBlocks;

public:
    PhiInst(std::string name, Type* _type, std::vector<Value*> _values, std::vector<BasicBlock*> _blocks)
        : Inst(name, _type, InstType::Phi), incomingValues(std::move(_values)), incomingBlocks(std::move(_blocks)) {}

    const std::vector<Value*>& getValues() const { return incomingValues; }
    const std::vector<BasicBlock*>& getBlocks() const { return incomingBlocks; }

    void dump() const override {
        std::cout << "PhiInst: Values = [";
        for (const auto& val : incomingValues) {
            std::cout << val->GetName() << ", ";
        }
        std::cout << "], Blocks = [";
        for (const auto& blk : incomingBlocks) {
            std::cout << blk->GetName() << ", ";
        }
        std::cout << "]\n";
    }
};

class BinaryInst : public Inst {
//TODO need to be modified
    Value* lhs;
    Value* rhs;
};


class Inst:public User{
public:
    enum InstType
    {
        None,
        // Terminators
        UnCond,Cond,Ret,
        // Memory
        Alloca,Load,Store,Memcpy,
        // Binary
        Add,Sub,Mul,Div,Mod,
        And,Or,Xor,Eq,Ne,Ge,L,Le,G,
        // Other
        Gep,Phi,Call,Zext,Sext,Trunc,
        FP2SI,SI2FP,BinaryUnknown,Max,Min,Select,
    };

    InstType itype=InstType::None;

    Inst() = default;  // 默认构造函数
    explicit Inst(std::string name, Type* type, InstType _itype) : User(std::move(name), type), itype(_itype) {}  // 指定类型的构造函数
    virtual ~Inst() = default;  // 虚析构函数保证子类的正确析构

    InstType getType() const { return itype; }

    virtual void dump() const = 0;
    virtual bool HasSideEffect(){return false;};
    
};

class BasicBlock:public Value,public clist<BasicBlock,Inst>{
    bool visited=false;
    bool reachable=true;
    std::vector<std::unique_ptr<Inst>> instList;
    BasicBlock* next=nullptr;
    BasicBlock* prev=nullptr;
public:
    BasicBlock::BasicBlock() : Value(NULL,VoidType::TypeGet()){};
    ~BasicBlock()=default;

    void genAllocaInst();
    void genStoreInst();
    void genLoadInst();
    void genCallInst();
    void genCondInst();
    void genUnCondInst();
    void genRetInst();
    void genZextInst();
    void genSextInst();
    void genPhiInst();

    void addInst(Inst* inst) ;

    void deleteInst(Inst* inst) {
        auto it = std::remove_if(instList.begin(), instList.end(),
                                 [inst](const std::unique_ptr<Inst>& i) { return i.get() == inst; });
        if (it != instList.end())
            instList.erase(it, instList.end());
    }

    void setNext(BasicBlock* _next) { next = _next; }
    void setPrev(BasicBlock* _prev) { prev = _prev; }
    BasicBlock* getNext() const { return next; }
    BasicBlock* getPrev() const { return prev; }

    void dump() const {
        std::cout << "BasicBlock:\n";
        for (const auto& inst : instList) {
            inst->dump();
        }
    }
};

class Func : public Value,public clist<Func, BasicBlock> {
    std::vector<std::unique_ptr<BasicBlock>> bbList;//基本块列表
    std::vector<std::unique_ptr<Value>> paramList;//参数列表
//TODO 后续可能加入内联优化等标记
public:
    Func(TypeID tp,std::string name);

    void addBB(BasicBlock* bb) {
        bbList.emplace_back(bb);
    }
    //在pred后插入ins
    void insertBB(BasicBlock* pred,BasicBlock* ins) ;

    void deleteBB(BasicBlock* bb) {
        auto it = std::remove_if(bbList.begin(), bbList.end(),
                                 [bb](const std::unique_ptr<BasicBlock>& b) { return b.get() == bb; });
        if (it != bbList.end()) {
            bbList.erase(it, bbList.end());
        }
    }

    void addParam(Value* param) {
        paramList.emplace_back(param);
    }

    void dump() const {
        std::cout << "Function:\n";
        for (const auto& bb : bbList) {
            bb->dump();
        }
    }
};

class Module {
    std::vector<std::unique_ptr<Func>> funcList;
    std::vector<std::unique_ptr<Variable>> globalVarList;

public:
    Module() = default;
    Func& genFunc(TypeID tp, std::string name);
    void addFunc(Func* func) ;
    void deleteFunc(Func* func) {
        auto it = std::remove_if(funcList.begin(), funcList.end(),
                                 [func](const std::unique_ptr<Func>& f) { return f.get() == func; });
        if (it != funcList.end())
            funcList.erase(it, funcList.end());
    }

    void addGlobalVariable(Variable* var) {
        globalVarList.emplace_back(var);
    }
    void deleteGlobalVariable(Variable* var) {
        auto it = std::remove_if(globalVarList.begin(), globalVarList.end(),
                                [var](const std::unique_ptr<Variable>& v) { return v.get() == var; });
        if (it != globalVarList.end())
            globalVarList.erase(it, globalVarList.end());
    }

    std::set<Func*> hasInlinedFunc; // Func that has done inlined pass
    std::set<Func*> inlinedFunc; // Func who is inlined by pass
    std::set<Func*> Side_Effect_Funcs; // Func that has side effect

    Func* GetmainFunc();
    void dump() const ;
};



