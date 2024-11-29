#pragma once
#include <vector>
#include <memory>
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
// Use类，可以双向访问到User和Value
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
    void setPrev(Use* _prev){prev=_prev;}
    Use* getPrev(){return prev;}
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

        explicit Iterator(Use* node) : current(node) {}

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
    bool empty() const { return Size == 0; }
    int size() const { return Size; }
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
    Use* front() const { return head; }
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
    void erase(Use* use);

};
class Value{
    std::string name;
    Type* type = nullptr;
    Userlist userlist;
public:
    Value(std::string _name, Type* _type) : name(_name), type(_type) {};
    ~Value(){
        while (!userlist.empty())
        delete userlist.front();
    }
    void setName(std::string newname);
    std::string GetName() const{return name;};

    virtual Type* getType(){return type;}

    virtual bool isGlobal(){return false;}
    virtual bool isParam(){return false;}
    virtual bool isConst(){return false;}
    
    void replaceAllUsesWith(Value* newvalue);
    Userlist& getUserlist(){return userlist;}
    int getUserlistSize()const { return userlist.size(); };
    void addUse(User* user);
    void removeUse(User* user);

    virtual void dump();

    virtual int getValueID() const = 0;
};
class User:public Value{
    using Useptr=std::unique_ptr<Use>;
    std::vector<Useptr> uselist;
public:
    User(std::string name, Type* type) : Value(std::move(name), type) {};
    
    Value* getOperand(int i) const {return uselist[i]->getUsee();};
    void addUse(Value* val);
    void removeUse(Value* val);
    void replaceUseOfWith(Value* oldval,Value* newval);
    void ReplacePercificUseWith(Use* olduse,Value* newval);

    inline Value* GetOperand(int i){return uselist[i]->getUsee();}
    std::vector<Useptr>& getuselist() {return this->uselist; }
    int getuselistsize() const { return uselist.size(); }
};
