/*==============================================================================
Generic template method implementation for the BetaScopeExt Class
==============================================================================*/

#include "BetaScopeExt_Class.h"

/*==============================================================================
template <typename TH_Type>
bool BetaScope_Ext::buildTH1Branch

  Usage:

    template method for building output TH1 branch.

  Parameters:

    std::string
      branchName := name for the outputing branch.


  return
    true if new branch is created. else return false.
==============================================================================*/
template <typename TH_Type>
bool BetaScope_Ext::BuildTH1Branch(std::string branchName)
{
  try
  {
    this->oTree_TH[this->new_th_branch_counter_] = new TH_Container<TH_Type>();
    TH_Type *my_th = static_cast<TH_Container<TH_Type> *>( this->oTree_TH[this->new_th_branch_counter_])->get();
    this->oTree_TH_Map.insert(std::pair<std::string, TH_BaseContainer *>(branchName, this->oTree_TH[this->new_th_branch_counter_]));
    this->output_ttree_->Branch(branchName.c_str(), my_th);
    this->new_th_branch_counter_++;
    LOG_INFO("TH Branch " + branchName + " is created for ouput.");
    return true;
  }
  catch (...)
  {
    LOG_ERROR("Fail creating TH1 branch.");
    return false;
  }
}

template <typename TH_Type>
typename DataType<TH_Type>::type *
BetaScope_Ext::GetOutTH1(std::string branchName)
{
  return static_cast<TH_Container<TH_Type> *>(this->oTree_TH_Map[branchName])->get();
}
