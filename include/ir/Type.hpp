#pragma once
#include <string>
#include <iostream>
enum TypeID {
        Void,
        Int,
        Float,
        Pointer,
        Array,
    };
    
class Type {
    TypeID id;
    int size;
public:
    Type(TypeID _id) : id(_id) {};
    virtual ~Type() = default;
    virtual bool isSameType(const Type& other) const ;
    virtual TypeID getTypeID() const;
    void setSize(int _size) {size=_size;};
    int getSize() const {return size;};
    virtual void dump() = 0;
};

class IntType:public Type
{
    IntType():Type(Int){setSize(4);}
public:
    static IntType* TypeGet(){
        static IntType single;
        return &single;
        };
    void dump()final{std::cout<<"int";};
};

class FloatType:public Type
{
    FloatType():Type(Float){setSize(4);}
public:
    static FloatType* TypeGet(){
        static FloatType single;
        return &single;
        };
    void dump()final{std::cout<<"float";};
};

class VoidType:public Type
{
    VoidType():Type(Void){setSize(0);}
public:
    static VoidType* TypeGet(){
        static VoidType single;
        return &single;
        };
    void dump()final{std::cout<<"void";};
};

class PointerType:public Type
{
    Type* subType;
    PointerType(Type* _subType):Type(Pointer),subType(_subType){setSize(4);}
public:
    static PointerType* TypeGet(Type* _subType){
        static PointerType single(_subType);
        return &single;
        };
    void dump()final{subType->dump();std::cout<<"*";};
    Type* GetSubType() const {return subType;};
};