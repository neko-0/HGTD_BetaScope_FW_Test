#ifndef BETASCOPE_DRIVER_H
#define BETASCOPE_DRIVER_H

#include "BetaScope_Driver/include/BetaScope.h"

/*
Pure virtual class for setting up basic interfaces.
*/
class BetaScope_Driver
{
  private:

    virtual bool Initialize() = 0;
    virtual bool EventLoop() = 0;
    virtual bool Execute() = 0;
    virtual bool Finalize() = 0;
    virtual int Run() = 0;

  protected:

    BetaScope beta_scope = BetaScope();

  public:

    BetaScope_Driver(){};
    virtual ~BetaScope_Driver(){};
};

#endif // BETASCOPE_DRIVER_H
