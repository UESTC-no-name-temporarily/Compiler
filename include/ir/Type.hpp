#pragma once
#include <string>

enum TypeID {
        Void,
        Integer,
        FloatingPoint,
        Pointer,
        Array,
        Struct,
        Function,
        StackStorage,
        Invalid,
    };
    
class Type {
private:
    virtual TypeID getType() const = 0;
public:
    Type() = default;
    Type(const Type&) = delete;
    Type(Type&&) = delete;
    Type& operator=(const Type&) = delete;
    Type& operator=(Type&&) = delete;
    virtual ~Type() = default;
    bool isVoid() const {
        return getType() == TypeID::Void;
    }
    bool isInteger() const {
        return getType() == TypeID::Integer;
    }
    bool isFloatingPoint() const {
        return getType() == TypeID::FloatingPoint;
    }   
    bool isPointer() const {
        return getType() == TypeID::Pointer;
    }
    bool isArray() const {
        return getType() == TypeID::Array;
    }
    bool isStruct() const {
        return getType() == TypeID::Struct;
    }       
    bool isFunction() const {
        return getType() == TypeID::Function;
    }
    bool isStackStorage() const {
        return getType() == TypeID::StackStorage;
    }
    bool isInvalid() const {
        return getType() == TypeID::Invalid;
    }
    bool isPrimitive() const {
        return isInteger() || isFloatingPoint();
    }
    virtual bool isSameType(const Type& other) const = 0;
    
};

class IntegerType : public Type {
public:
    TypeID getType() const override {
        return TypeID::Integer;
    }
    bool isSameType(const Type& other) const override;
};

class FloatingPointType : public Type {
public:
    TypeID getType() const override {
        return TypeID::FloatingPoint;
    }
    bool isSameType(const Type& other) const override;
};

class PointerType : public Type {
public:
    TypeID getType() const override {
        return TypeID::Pointer;
    }
    bool isSameType(const Type& other) const override;
};

class ArrayType : public Type {
public:
    TypeID getType() const override {
        return TypeID::Array;
    }
    bool isSameType(const Type& other) const override;
};