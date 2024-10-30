// TODO created by AI,needed to be fixed 
#pragma once
#include <string>

enum TypeID {
        VoidTyID,
        IntegerTyID,
        FloatTyID,
        DoubleTyID,
        // Add more types as needed
    };
    
class Type {
public:
    
private:
    TypeID id;

protected:
    Type(TypeID id) : id(id) {}

public:
    virtual ~Type() = default;

    TypeID getTypeID() const { return id; }

    virtual std::string getName() const = 0;

    static bool isIntegerType(TypeID id) {
        return id == IntegerTyID;
    }

    static bool isFloatingPointType(TypeID id) {
        return id == FloatTyID || id == DoubleTyID;
    }
};

class IntegerType : public Type {
public:
    IntegerType() : Type(IntegerTyID) {}

    std::string getName() const override {
        return "Integer";
    }
};

class FloatType : public Type {
public:
    FloatType() : Type(FloatTyID) {}

    std::string getName() const override {
        return "Float";
    }
};

class DoubleType : public Type {
public:
    DoubleType() : Type(DoubleTyID) {}

    std::string getName() const override {
        return "Double";
    }
};