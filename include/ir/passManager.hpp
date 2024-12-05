#include "BasicClass.hpp"
#include "Type.hpp"
#include "CFG.hpp"
#include <iostream>

class AnalysisPassManager {

public:
    virtual void run() = 0;
};
class TransformPassManager {

public:
    virtual void run() = 0;
};
class passManager { 
    enum{
        DCE,
        LICM,
        SCCP,
        Mem2Reg,
        DeadStoreElimination,
        DeadCodeElimination,
        ConstantPropagation,
        ConstantFolding,
        LoopInvariantCodeMotion
    }passName;
    AnalysisPassManager* APM;
    TransformPassManager* TPM;

};