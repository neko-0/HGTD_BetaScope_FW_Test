#ifndef BETASCOPE_ANAFRAMEWORK_H
#define BETASCOPE_ANAFRAMEWORK_H

#include <mutex>
#include <functional>

#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "BetaScope_Driver/include/BetaScopeExt_Class.h"


template <typename beta_scope_type> class BetaScope_AnaFramework {
  int event_counter = 0;

protected:
  beta_scope_type beta_scope;
  static std::mutex mu;


public:
  BetaScope_AnaFramework(){};
  virtual ~BetaScope_AnaFramework(){};

  virtual bool Initialize(std::string addBranches = "BetaScope_Driver/src/additionalBranches.ini", std::string rawBranches = "");
  virtual void Finalize();

  virtual void Analysis()
  {
    LOG_INFO("this is a virtual Analysis().");
  };

  virtual void LoopEvents( void (BetaScope_AnaFramework::*func)() );
  virtual void LoopEvents( void (BetaScope_AnaFramework::*func)(), std::function<bool()> selector );

  virtual void FillData();
  virtual void FillData( bool fill );

  virtual void Run()
  {
    if(BetaScope_AnaFramework::Initialize())
    {
      BetaScope_AnaFramework::LoopEvents(&BetaScope_AnaFramework::Analysis);
      BetaScope_AnaFramework::Finalize();
    }
  };
};

template <typename beta_scope_type>
bool BetaScope_AnaFramework<beta_scope_type>::Initialize( std::string addBranches, std::string rawBranches)
{
  std::lock_guard<std::mutex> lck(this->mu);
  this->beta_scope.RawTreeReader();
  this->beta_scope.NewTreeMaker(addBranches);
  return true;
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::LoopEvents( void (BetaScope_AnaFramework::*func)() )
{
  LOG_INFO("BetaScope_AnaFramework::LoopEvents is used for driving event looping." );
  while (this->beta_scope.GetInTreeReader()->Next())
  {
    (this->*func)();
    BetaScope_AnaFramework<beta_scope_type>::FillData();
  }
  LOG_INFO("Finished loopEvents.");
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::LoopEvents( void (BetaScope_AnaFramework::*func)(), std::function<bool()> selector)
{
  LOG_INFO("BetaScope_AnaFramework::LoopEvents is used for driving event looping." );
  while( this->beta_scope.GetInTreeReader()->Next() )
  {
    (this->*func)();
    BetaScope_AnaFramework<beta_scope_type>::FillData( selector() );
  }
  LOG_INFO("Finished loopEvents.");
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::FillData()
{
  this->event_counter++;
  this->beta_scope.FillEvent();
  if(this->event_counter % 1000 == 0 || (this->event_counter % 10 == 0 &&  this->event_counter <= 100) )
  {
    LOG_INFO(this->beta_scope.GetInFileNickName() + " Proccessed events: " + std::to_string(this->event_counter) + " /" + std::to_string(this->beta_scope.GetInNumEvent()) );
  }
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::FillData( bool fill )
{
  this->event_counter++;
  if(fill)
  {
      this->beta_scope.FillEvent();
  }
  if(this->event_counter % 1000 == 0 || (this->event_counter % 10 == 0 &&  this->event_counter <= 100) )
  {
    LOG_INFO(this->beta_scope.GetInFileNickName() + " Proccessed events: " + std::to_string(this->event_counter) + " /" + std::to_string(this->beta_scope.GetInNumEvent()) );
  }
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::Finalize()
{
  std::lock_guard<std::mutex> lck(this->mu);
  this->beta_scope.FileClose();
}

template <typename beta_scope_type>
std::mutex BetaScope_AnaFramework<beta_scope_type>::mu;

#endif // BETASCOPE_ANAFRAMEWORK_H
