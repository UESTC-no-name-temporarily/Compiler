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
    bool isGlobalVar; // 标记是否是全局变量
    Value* initializer; // 对于全局变量，可能会有初始值
public:
    // 构造函数
    Variable(std::string name, Type* type, bool isGlobal = false, Value* init = nullptr)
        : User(std::move(name), type), isGlobalVar(isGlobal), initializer(init) {}
    // 判断是否是全局变量
    bool isGlobal() const  {
        return isGlobalVar;
    }
    // 获取初始化值
    Value* getInitializer() const {
        return initializer;
    }
    // 设置初始化值
    void setInitializer(Value* init) {
        initializer = init;
    }
    // 打印变量信息
    void dump() override {
        std::cout << "Variable: Name = " << GetName()
                  << ", Type = " << getType()->getTypeID()
                  << ", Global = " << (isGlobalVar ? "true" : "false");
        if (initializer) {
            std::cout << ", Initializer = " << initializer->GetName();
        }
        std::cout << "\n";
    }
    // 获取变量类型
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

    void dump() const override {
        std::cout << "AllocaInst: Name = " << GetName()
                  << ", Type = " << getType()->getTypeID()
                  << ", Num = " << num << "\n";
    }
};

class StoreInst : public Inst {
    Value* value;   // 要存储的值
    Value* pointer; // 存储的地址

public:
    StoreInst(std::string name, Type* _type, Value* _value, Value* _pointer)
        : Inst(std::move(name), _type, InstType::Store), value(_value), pointer(_pointer) {}

    Value* getValue() const { return value; }
    Value* getPointer() const { return pointer; }

    void dump() const override {
        std::cout << "StoreInst: Value = " << value->GetName()
                  << ", Pointer = " << pointer->GetName() << "\n";
    }
};

class LoadInst : public Inst {
    Value* pointer; // 要加载的地址

public:
    LoadInst(std::string name, Type* _type, Value* _pointer)
        : Inst(std::move(name), _type, InstType::Load), pointer(_pointer) {}

    Value* getPointer() const { return pointer; }

    void dump() const override {
        std::cout << "LoadInst: Pointer = " << pointer->GetName() << "\n";
    }
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
        : Inst(std::move(name), _type, InstType::Phi), incomingValues(std::move(_values)), incomingBlocks(std::move(_blocks)) {}

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

    virtual void dump() const {
        std::cout << "Inst: " << static_cast<int>(itype) << std::endl;
    }
    virtual bool HasSideEffect(){return false;};
    
};

class BasicBlock:public Value,public clist<BasicBlock,Inst>{
    
    std::vector<std::unique_ptr<Inst>> instList;
    BasicBlock* next=nullptr;
    BasicBlock* prev=nullptr;
public:
    BasicBlock()=default;
    ~BasicBlock()=default;
    void addInst(Inst* inst) {
        instList.emplace_back(inst);
    }

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
    int bbCount = 0;
    Func* next = nullptr;
    Func* prev = nullptr;
    std::vector<std::unique_ptr<BasicBlock>> bbList;
    std::vector<std::unique_ptr<Value>> paramList;

public:
    enum Tag {
        Normal, UnrollBody, LoopBody, ParallelBody, BuiltIn
    };

private:
    Tag funcTag = Tag::Normal;

public:
    Func() = default;
    ~Func() = default;

    void addBB(BasicBlock* bb) {
        bbList.emplace_back(bb);
        ++bbCount;
    }

    void deleteBB(BasicBlock* bb) {
        auto it = std::remove_if(bbList.begin(), bbList.end(),
                                 [bb](const std::unique_ptr<BasicBlock>& b) { return b.get() == bb; });
        if (it != bbList.end()) {
            bbList.erase(it, bbList.end());
            --bbCount;
        }
    }

    void addParam(Value* param) {
        paramList.emplace_back(param);
    }

    void setTag(Tag _tag) { funcTag = _tag; }
    Tag getTag() const { return funcTag; }

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

    void addFunc(Func* func) {
        funcList.emplace_back(func);
    }

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



