#pragma once
#include "./PassBase.hpp"
#include <any>
#include <getopt.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <queue>

enum OptLevel { O0 = 0, O1 = 1, O2 = 2, O3 = 3 };
enum PassName {
  mem2reg = 4,
  pre,
  constprop,
  dce,
  loopsimplify,
  simplifycfg,
  ece,
  Inline,
  global2local,
  reassociate,
  cse,
  lcssa,
  licm,
  looprotate,
  loopdeletion,
  deadargselimination,
  storeonlyglobalelimination,
  local2global,
  parallel,
  loopUnroll,
  gepcombine,
  tailrecurseEliminator,
  condmerge,
  gepevaluate,
  blockmerge,
  Dse,
  loadeliminaion,
  selfstoreelimination,
  cachelookup,
  scalarstrengthreduce,
  consthoist,
  select2branch
};

static struct option long_options[] = {
    {"mem2reg", no_argument, 0, 4},
    {"pre", no_argument, 0, 5},
    {"constprop", no_argument, 0, 6},
    {"dce", no_argument, 0, 7},
    {"loopsimplify", no_argument, 0, 8},
    {"simplifycfg", no_argument, 0, 9},
    {"ece", no_argument, 0, 10},
    {"inline", no_argument, 0, 11},
    {"global2local", no_argument, 0, 12},
    {"reassociate", no_argument, 0, 13},
    {"cse", no_argument, 0, 14},
    {"lcssa", no_argument, 0, 15},
    {"licm", no_argument, 0, 16},
    {"loop-rotate", no_argument, 0, 17},
    {"loop-deletion", no_argument, 0, 18},
    {"deadargselimination", no_argument, 0, 19},
    {"storeonlyglobalelimination", no_argument, 0, 20},
    {"local2global", no_argument, 0, 21},
    {"parallel", no_argument, 0, 22},
    {"loopUnroll", no_argument, 0, 23},
    {"GepCombine", no_argument, 0, 24},
    {"TailRecurseEliminator", no_argument, 0, 25},
    {"CondMerge", no_argument, 0, 26},
    {"GepEvaluate", no_argument, 0, 27},
    {"BlockMerge", no_argument, 0, 28},
    {"dse", no_argument, 0, 29},
    {"LoadElimination", no_argument, 0, 30},
    {"SelfStoreElimination", no_argument, 0, 31},
    {"CacheLookUp", no_argument, 0, 32},
    {"ScalarStrengthReduce", no_argument, 0, 33},
    {"consthoist", no_argument, 0, 34},
    {"select2branch", no_argument, 0, 35},
    {"O0", no_argument, 0, 0},
    {"O1", no_argument, 0, 1},
    {"O2", no_argument, 0, 2},
    {"O3", no_argument, 0, 3},
    {0, 0, 0, 0}};

class _AnalysisManager
    : public AnalysisBase<_AnalysisManager, Func> {
private:
  std::vector<std::any> Contain;

public:
  _AnalysisManager() = default;
  void Run();
  virtual ~_AnalysisManager() = default;
  template <typename Pass, typename... Args,
            typename name = std::enable_if_t<
                std::is_base_of_v<AnalysisBase<Pass, Func>, Pass>>>
  Pass *get(Func *func, Args &&...args) {
    auto it =
        std::find_if(Contain.begin(), Contain.end(), [&](const std::any &ele) {
          return ele.type() == typeid(Pass *);
        });
    if (it != Contain.end()) {
      delete std::any_cast<Pass *>(*it);
      Contain.erase(it);
    }
    auto pass = new Pass(func, std::forward<Args>(args)...);
    auto *result = pass->GetResult(func);
    Contain.emplace_back(pass);
    return static_cast<Pass *>(result);
  }

  template <typename Pass, typename... Args,
            typename name = std::enable_if_t<
                std::is_base_of_v<AnalysisBase<Pass, Module>, Pass>>>
  Pass *get(Module *mod, Args &&...args) {
    auto pass = new Pass(mod, std::forward<Args>(args)...);
    auto *result = pass->GetResult();
    Contain.emplace_back(pass);
    return static_cast<Pass *>(result);
  }
};

class _PassManager : public PassBase<_PassManager, Func> {
public:
  _PassManager() { module = &Singleton<Module>(); }
  virtual ~_PassManager() = default;
  void Run();
  void RunOnTest();
  template <typename Pass, typename name = std::enable_if_t<std::is_base_of_v<
                               PassBase<Pass, Func>, Pass>>>
  bool RunImpl(Func *func, _AnalysisManager &AM) {
    auto pass = std::make_unique<Pass>(func, AM);
    return pass->Run();
  }
  template <typename Pass, typename name = std::enable_if_t<std::is_base_of_v<
                               PassBase<Pass, Module>, Pass>>>
  bool RunImpl(Module *mod, _AnalysisManager &AM) {
    auto pass = std::make_unique<Pass>(mod, AM);
    return pass->Run();
  }
  void DecodeArgs(int argc, char *argv[]);
private:
  void Init();
  void AddPass(PassName pass) { EnablePass.push(pass); }
  std::queue<PassName> EnablePass;
  Module *module;
  Func *curfunc;
  bool modified = true;
  bool other = false;
  bool HasRunCondMerge = false;
};