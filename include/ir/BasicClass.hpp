//TODO COPY FROM GPT5.0,NEED TO BE MODIFIED
#pragma once
#include <vector>
#include <cassert>
#include "List.hpp"
#include <variant>
#include <functional>
#include <iostream>
#include <cxxabi.h>
#include "Type.hpp"
#include <string>
#include <set>
#include <unordered_set>
class User;
class Value;
class BasicBlock;
class Function;
class Use{
public:
    ~Use(){
        remove_from_Userlist(user);
    }
    User* user;
    Value* value;
    Use* next;
    User* getUser(){
        return user;
    }
    Value* getUsee(){
        return value;
    }
    void setUser(User* _user){
        user=_user;
    }
    void setUsee(Value* _value){
        value=_value;
    }
    void remove_from_Userlist(User* _user);

};
class Userlist{
//TODO finish this class
};
class Value{
    std::string name;
    void SetName(std::string newname);
    virtual std::string GetName();

    Type* type;
    void SetType(Type* _type){ type = _type;}
    virtual Type* GetType(){return type;}
    Userlist* userlist;

};
class User{

};
