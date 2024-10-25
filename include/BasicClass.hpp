#include <bits/stdc++.h>

class Value;
class User;
class Use;
class BasicBlock;
class Instruction;

class Value {
    private:
        std::string name;
    public:
        bool hasName() const;
        std::string getName() const;
        void setName(std::string name);
        
};
