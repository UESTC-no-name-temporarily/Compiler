#include <bits/stdc++.h>

class Value;
class User;
class Use;
class BasicBlock;
class Instruction;

class Value {
    private:
        std::vector<Use*> Uselist;
    protected:
        std::string name;
    public:
        bool HasName() const;
        std::string GetName() const;
        void SetName(std::string name);

        std::vector<Use*> GetUses() const;
        
};
