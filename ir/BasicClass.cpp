#include "../include/ir/Type.hpp"
#include "../include/ir/BasicClass.hpp"

void Value::setName(std::string newname){
    name=newname;
}
void Value::addUse(User* user) {
    Use *use = new Use(user, this);
    userlist.push_back(use);
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

void Value::replaceAllUsesWith(Value* newValue) {
    while (!userlist.empty()) {
        Use* use = *userlist.begin();
        User* user = use->getUser();
        user->replaceUseOfWith(this, newValue);
    }
}

void User::addUse(Value* val) {
    if (!val) return;
    auto use = std::make_unique<Use>(this, val);
    val->addUse(this);  // 更新对应的 Value 的 userlist
    UseList.push_back(std::move(use));  // 添加到自己的 UseList
}

void User::removeUse(Value* val) {
    for (auto it = UseList.begin(); it != UseList.end(); ++it) {
        if ((*it)->getUsee() == val) {
            val->removeUse(this);  // 从对应的 Value 的 userlist 移除
            it = UseList.erase(it); // 从自己的 UseList 移除
            break;
        }
    }
}

void User::replaceUseOfWith(Value* oldValue, Value* newValue) {
    for (auto& use : UseList) {
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
