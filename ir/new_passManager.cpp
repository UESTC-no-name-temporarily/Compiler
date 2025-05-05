#include "../include/ir//New_passManager.hpp"
#include "../include/ir/CFG.hpp"
#include "../include/utils/Singleton.hpp"

void _AnalysisManager::Run() { return ; }
void _PassManager::DecodeArgs(int argc, char *argv[]) {
  int optionIndex, option;
  while ((option = getopt_long(argc, argv, "", long_options, &optionIndex)) !=
         -1) {
    switch (option) {
    default:
      std::cerr << "No Such Opt!" << std::endl;
      exit(0);
    case mem2reg:
      AddPass(mem2reg);
      break;
    case pre:
      AddPass(pre);
      break;
    case constprop:
      AddPass(constprop);
      break;
    case dce:
      AddPass(dce);
      break;
    case loopsimplify:
      AddPass(loopsimplify);
      break;
    case simplifycfg:
      AddPass(simplifycfg);
      break;
    case ece:
      AddPass(ece);
      break;
    case Inline:
      AddPass(Inline);
      break;
    case global2local:
      AddPass(global2local);
      break;
    case reassociate:
      AddPass(reassociate);
      break;
    case cse:
      AddPass(cse);
      break;
    case lcssa:
      AddPass(lcssa);
      break;
    case licm:
      AddPass(licm);
      break;
    case looprotate:
      AddPass(looprotate);
      break;
    case loopdeletion:
      AddPass(loopdeletion);
      break;
    case deadargselimination:
      AddPass(deadargselimination);
      break;
    case storeonlyglobalelimination:
      AddPass(storeonlyglobalelimination);
      break;
    case local2global:
      AddPass(local2global);
      break;
    case parallel:
      AddPass(parallel);
      break;
    case loopUnroll:
      AddPass(loopUnroll);
      break;
    case gepcombine:
      AddPass(gepcombine);
      break;
    case tailrecurseEliminator:
      AddPass(tailrecurseEliminator);
      break;
    case condmerge:
      AddPass(condmerge);
      break;
    case gepevaluate:
      AddPass(gepevaluate);
      break;
    case blockmerge:
      AddPass(blockmerge);
      break;
    case Dse:
      AddPass(Dse);
      break;
    case cachelookup:
      AddPass(cachelookup);
      break;
    case scalarstrengthreduce:
      AddPass(scalarstrengthreduce);
      break;
    case consthoist:
      AddPass(consthoist);
      break;
    case select2branch:
      AddPass(select2branch);
      break;
    case O0:
      level = O0;
      break;
    case O1:
      level = O1;
      break;
    case O2:
      level = O2;
      break;
    case O3:
      level = O3;
      break;
    }
  }
}

void _PassManager::Run() { return ; }

///@brief 执行Pass顺序会按照给定参数的顺序来
void _PassManager::RunOnTest() {
  _AnalysisManager AM;
  while (!EnablePass.empty()) {
   auto name = EnablePass.front();
   EnablePass.pop();
   switch (name) {
    //case: module优化
    default: {
     for (int i = 0; i < module->GetFuncTion().size(); i++) {
      auto &func = module->GetFuncTion()[i];
      if (func->tag == Function::BuildIn)
       continue;
      curfunc = func.get();
      // 维护bbs关系
      curfunc->bb_num = 0;
      curfunc->GetBasicBlock().clear();
      for (auto bb : *curfunc) {
       bb->num = curfunc->bb_num++;
       curfunc->GetBasicBlock().push_back(bb);
      }
      switch (name) {
       //case func优化
       case mem2reg: {
        curfunc = func.get();
        // 维护bbs关系
        curfunc->bb_num = 0;
        curfunc->GetBasicBlock().clear();
        for (auto bb : *curfunc) {
         bb->num = curfunc->bb_num++;
         curfunc->GetBasicBlock().push_back(bb);
        }
        auto Mem2regRes = RunImpl<Mem2reg>(curfunc, AM);
        break;
       }
       default: {
        assert(0);
       }
      }
     }
     break;
    }
   }
  }
 }

void _PassManager::Init() { return; }