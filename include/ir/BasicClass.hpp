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
class Func;
class Use{
public:
    Use(User* _user,Value* _value){
        user=_user;
        value=_value;
        //next=nullptr;
    }
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
    Use* head=nullptr;
    int size=0;
public:
    void push_back(Use* use);
    bool is_empty();
    int Size();
    Use* begin();
};
class Value{
    std::string name;
    Type* type=nullptr;
    Userlist* userlist=nullptr;
public:
    ~Value(){
        if(userlist!=nullptr){
            //TODO delete userlist
        }
    }
    void SetName(std::string newname);
    virtual std::string GetName();
    void SetType(Type* _type){ type = _type;}
    virtual Type* GetType(){return type;}

    virtual bool isGlobal(){return false;}
    virtual bool isParam(){return false;}
    
    void replaceAllUsesWith(Value* newvalue);
    int getUserlistSize();
    void addUse(User* user,Value* value);
    void removeUse(User* user);

    void dump();
};
class User:public Value{
    using Useptr=std::unique_ptr<Use>;
    std::vector<Useptr> UseList;
public:
    ~User(){
        for(auto& use:UseList){
        //TODO delete use
            use->remove_from_Userlist(this);
        }
    }

    Value* getOperand(int i);
    void addUse(Value* val);
    void removeUse(Value* val);
    void replaceUseOfWith(Value* oldval,Value* newval);

};
