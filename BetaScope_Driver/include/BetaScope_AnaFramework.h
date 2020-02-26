#ifndef BETASCOPE_ANAFRAMEWORK_H
#define BETASCOPE_ANAFRAMEWORK_H

#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "BetaScope_Driver/include/BetaScopeExt_Class.h"


template <typename beta_scope_type> class BetaScope_AnaFramework {
  int event_counter = 0;

protected:
  beta_scope_type beta_scope;

public:
  BetaScope_AnaFramework(){};
  virtual ~BetaScope_AnaFramework(){};

  virtual void Initialize(std::string addBranches = "BetaScope_Driver/src/additionalBranches.ini", std::string rawBranches = "");

  virtual void Analysis()
  {
    logger.info(__PRETTY_FUNCTION__, "this is a virtual analysis().");
  };

  virtual void LoopEvents(void (BetaScope_AnaFramework::*func)());
  virtual void Finalize();
  virtual void FillData();

  virtual void Run()
  {
    BetaScope_AnaFramework::Initialize();
    BetaScope_AnaFramework::LoopEvents(&BetaScope_AnaFramework::Analysis);
    BetaScope_AnaFramework::Finalize();
  };
};

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::Initialize( std::string addBranches, std::string rawBranches)
{
  this->beta_scope.RawTreeReader();
  //if (addBranches.compare("") != 0)
  this->beta_scope.NewTreeMaker(addBranches);
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::LoopEvents( void (BetaScope_AnaFramework::*func)())
{
  logger.info( __PRETTY_FUNCTION__, "is used for driving event looping." );
  while (this->beta_scope.GetInTreeReader()->Next())
  {
    (this->*func)();
    BetaScope_AnaFramework<beta_scope_type>::FillData();
  }
  logger.info( __PRETTY_FUNCTION__, "Finished loopEvents.");
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::FillData()
{
  this->event_counter++;
  this->beta_scope.FillEvent();
  if(this->event_counter % 1000 == 0 || (this->event_counter % 10 == 0 &&  this->event_counter <= 100) )
  {
    logger.info(__PRETTY_FUNCTION__, this->beta_scope.GetInFileNickName() + " Proccessed events: " + std::to_string(this->event_counter) + " /" + std::to_string(this->beta_scope.GetInNumEvent()) );
  }
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::Finalize()
{
  this->beta_scope.FileClose();
}

#endif // BETASCOPE_ANAFRAMEWORK_H
