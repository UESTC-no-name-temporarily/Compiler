#pragma once
#include "./CFG.hpp"
#include "./mem2reg.hpp"
template <typename Pass, typename DerivedT> class AnalysisBase {
public:
  virtual void Run() = 0;
  AnalysisBase() = default;
  virtual const Pass *GetResult(DerivedT *func) const { return nullptr; }
  const Pass *derived_this() const { return static_cast<const Pass *>(this); }
};

template <typename Pass, typename Scope> class PassBase {
public:
  virtual void Run() = 0;
  PassBase() = default;
  const Pass *derived_this() const { return static_cast<const Pass *>(this); }
};

