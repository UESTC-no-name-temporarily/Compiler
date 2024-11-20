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
    User* user;
    Value* value;
    Use* next=nullptr;
    Use* prev=nullptr;
public:
    Use(User* _user, Value* _value) : user(_user), value(_value) {}
    ~Use(){
        if(user)
            remove_from_Userlist(user);
    }
    
    User* getUser(){return user;}
    Value* getUsee(){return value;}

    void setUser(User* _user){ user=_user;}
    void setUsee(Value* _value){value=_value;}
    void setNext(Use* _next){next=_next;}
    Use* getNext(){return next;}

    void remove_from_Userlist(User* _user){
        if (prev)
            prev->next = next;
        if (next)
            next->prev = prev;
        // 清理自己
        next = nullptr;
        prev = nullptr;
    };

};
class Userlist{
    struct Iterator {
        Use* current;

        Iterator(Use* node) : current(node) {}

        Use* operator*() const { return current; }
        Iterator& operator++() {
            if (current) current = current->getNext();
            return *this;
        }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    Use* head=nullptr;
    int Size=0;
public:
    ~Userlist() {
        while (head) {
            Use* temp = head;
            head = head->getNext();
            delete temp;
        }
        Size = 0;
    }
    void push_back(Use* use);
    bool empty() const { return Size == 0; }
    int size() const { return Size; }
    Use* begin() { return head; }
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
    void push_back(Use* use) {
        if (!use) return;

        if (!head) {
            head = use;
        } else {
            Use* current = head;
            while (current->getNext()) {
                current = current->getNext();
            }
            current->setNext(use);
        }
    ++Size;
    }
};
class Value{
    std::string name;
    Type* type = nullptr;
    Userlist userlist;
public:
    Value(std::string _name, Type* _type) : name(std::move(_name)), type(_type) {};
    ~Value(){
            //遍历userlist，删除所有use
            
    }
    void setName(std::string newname);
    std::string GetName() const;

    virtual Type* getType(){return type;}

    virtual bool isGlobal(){return false;}
    virtual bool isParam(){return false;}
    
    void replaceAllUsesWith(Value* newvalue);
    int getUserlistSize()const { return userlist.size(); };
    void addUse(User* user);
    void removeUse(User* user);

    virtual void dump();

    virtual int getValueID() const = 0;
};
class User:public Value{
    using Useptr=std::unique_ptr<Use>;
    std::vector<Useptr> UseList;
public:
    User(std::string name, Type* type) : Value(std::move(name), type) {};
    virtual ~User(){
        for(auto& use:UseList){
        //TODO delete use
            use->remove_from_Userlist(this);
        }
    }

    Value* getOperand(int i) const {
        return UseList[i]->getUsee();
    };
    void addUse(Value* val);
    void removeUse(Value* val);
    void replaceUseOfWith(Value* oldval,Value* newval);
    int getUseListsize() const { return UseList.size(); }
};
