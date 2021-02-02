#ifndef BETASCOPE_ANALYSIS_DRIVER_H
#define BETASCOPE_ANALYSIS_DRIVER_H

#include <mutex>
#include <functional>

#include "BetaScope_Driver/include/BetaScope_Driver.h"


class BetaScope_AnalysisDriver : public BetaScope_Driver
{
  int event_counter = 0;
  std::string class_name = "BetaScope_AnalysisDriver";

  bool Initialize () override;
  bool EventLoop () override;
  bool Execute() override;
  bool Finalize() override;

  virtual bool initialize () = 0;
  virtual bool finalize() = 0;
  virtual bool execute() = 0;

  protected:
    static std::mutex mu_lock;

  public:
    BetaScope_AnalysisDriver(){};
    virtual ~BetaScope_AnalysisDriver(){};

  int Run() override;

};

#endif // BETASCOPE_ANALYSIS_DRIVER_H
