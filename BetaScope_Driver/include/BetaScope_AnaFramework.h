#ifndef BETASCOPE_ANAFRAMEWORK_H
#define BETASCOPE_ANAFRAMEWORK_H

#include "BetaScope_Driver/include/BetaScope_Class.h"

class BetaScope_AnaFramework
{
  int event_counter = 0;

  public:
    BetaScope beta_scope;

    BetaScope_AnaFramework(){};
    ~BetaScope_AnaFramework(){};

    virtual void initialize( std::string addBranches="BetaScope_Driver/src/additionalBranches.ini", std::string rawBranches="" );
    virtual void loopEvents( );
    virtual void finalize( );
    void filldata( );

    void copyBranch( );
};

#endif // TEST_H
