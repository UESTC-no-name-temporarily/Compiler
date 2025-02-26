// 这里放的是pass管理器，头文件需要把所有的pass代码引用过来
// 对于analyse和transform的pass，需要分别写两个类，然后在passManager里面写一个enum，然后根据enum选择对应的pass

#include "BasicClass.hpp"
#include "Type.hpp"
#include "CFG.hpp"
#include <iostream>

class AnalysisPassManager {
    private:
        
    public:
        virtual void run() = 0;

};

class TransformPassManager {

    public:
        virtual void run() = 0;

};

class passManager { 
    private:
        enum{
            DCE,
            LICM,
            SCCP,
            Mem2Reg,
            //...
        }passName;
        AnalysisPassManager* APM;
        TransformPassManager* TPM;

};