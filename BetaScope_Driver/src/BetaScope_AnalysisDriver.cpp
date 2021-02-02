#include "BetaScope_Driver/include/BetaScope_AnalysisDriver.h"

std::mutex BetaScope_AnalysisDriver::mu_lock;

bool BetaScope_AnalysisDriver::Initialize()
{
  std::lock_guard<std::mutex> lck(this->mu_lock);
  LOG_INFO(this->class_name + " Start Initialization.");
  if( this->initialize() )
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool BetaScope_AnalysisDriver::EventLoop()
{
  LOG_INFO("Start EventLoop.");
  while(this->beta_scope.NextEvent())
  {
    this->Execute();
    this->event_counter++;
    if(this->event_counter % 1000 == 0 || (this->event_counter % 10 == 0 &&  this->event_counter <= 100) )
    {
      LOG_INFO(this->class_name + " Proccessed events: " + this->beta_scope.GetInFileNickName() + "=>" + std::to_string(this->event_counter) + "/" + std::to_string(this->beta_scope.GetInNumEvent()) );
    }
    this->beta_scope.ClearVecBuffer();
  }
  LOG_INFO("Finished EventLoop.");
  return true;
}

bool BetaScope_AnalysisDriver::Execute()
{
  return this->execute();
}

bool BetaScope_AnalysisDriver::Finalize()
{
  std::lock_guard<std::mutex> lck(this->mu_lock);
  LOG_INFO("Start Finalizing.")
  this->finalize();
  this->beta_scope.FileClose();
  return true;
}

int BetaScope_AnalysisDriver::Run()
{
  this->Initialize();
  this->EventLoop();
  this->Finalize();
  return 0;
}
