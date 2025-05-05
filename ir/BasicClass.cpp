#include "../include/ir/Type.hpp"
#include "../include/ir/BasicClass.hpp"
#include "./include/ir/CFG.hpp"
void Value::setName(std::string newname){
    name=newname;
}
void Value::addUse(User* user) {
    Use *use = new Use(user, this);
    userlist.push_back(use);
    user->addUse(this);
}

void Value::removeUse(User* user) {
    for (auto it = userlist.begin(); it != userlist.end(); ++it) {
        if ((*it)->getUser() == user) {
            auto use = *it;
            userlist.erase(*it);  // 从 `Userlist` 移除
            delete use;          // 删除 Use 节点
            break;
        }
    }
}
void Value::dump() {
  if (isConst())
    std::cout << GetName();
  else if (isGlobal())
    std::cout << "@" << GetName();
  else if (auto tmp = dynamic_cast<Func *>(this))
    std::cout << "@" << tmp->GetName();
  else if (GetName() == "undef")
    std::cout << GetName();
  else
    std::cout << "%" << GetName();
}
void Value::replaceAllUsesWith(Value* newValue) {
    while (!userlist.empty()) {
        Use* use = *userlist.begin();
        User* user = use->getUser();
        user->replaceUseOfWith(this, newValue);
    }
}

void User::addUse(Value* val) {
    uselist.push_back(std::make_unique<Use>(this, val));
}

void User::removeUse(Value* val) {
    for (auto it = uselist.begin(); it != uselist.end(); ++it) {
        if ((*it)->getUsee() == val) {
            val->removeUse(this);  // 从对应的 Value 的 userlist 移除
            it = uselist.erase(it); // 从自己的 uselist 移除
            break;
        }
    }
}

void User::replaceUseOfWith(Value* oldValue, Value* newValue) {
    for (auto& use : uselist) {
        if (use->getUsee() == oldValue) {
            oldValue->removeUse(this);  // 解除旧的 Value 的关联
            use->setUsee(newValue);     // 设置新的 Value
            newValue->addUse(this);     // 更新新的 Value 的 userlist
        }
    }
}

void Userlist::erase(Use* use) {
    if (!use) return;

    // 从链表中移除节点
    if (use == head) {
        head = use->getNext();
    } else {
        use->getPrev()->setNext(use->getNext());
    }

    if (use->getNext()) {
        use->getNext()->setPrev(use->getPrev());
    }

    delete use;  // 删除 Use 节点
    --Size;
}
