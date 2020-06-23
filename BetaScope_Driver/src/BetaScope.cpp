#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include <iostream>
#include <map>
#include <string>

BetaScope::~BetaScope()
{
  LOG_INFO( boost::str(boost::format("%1% call destructor at location %2%")%this%this->object_location_ ) );
  LOG_INFO("Clean up allocated memory");
  BetaScope::_ClearVecBuffer();
  int i_count = 0;
  int o_count = 0;
  for(auto &val : this->input_branches_buffer_){ delete val; val = nullptr; i_count++; }
  for(auto &val : this->output_branches_buffer_){ delete val; val = nullptr; o_count++; }
  //for(auto &[key, val] : this->output_branch_map_){ LOG_INFO(key); BETA_LOG::DELETE_=true; delete val; val = nullptr; o_count++; }
  //BETA_LOG::DELETE_=true;
  //delete this->output_tfile_;
  //this->output_tfile_ = nullptr;
  //BETA_LOG::DELETE_=true;
  delete this->input_tree_reader_;
  this->input_tree_reader_ = nullptr;
  //BETA_LOG::DELETE_=true;
  //delete this->input_tfile_;
  //this->input_tfile_ = nullptr;
  /*
  LOG_INFO("new " + std::to_string(BETA_LOG::NEW_COUNTER));
  LOG_INFO("delete " + std::to_string(BETA_LOG::DELETE_COUNTER));
  LOG_INFO("other new " + std::to_string(BETA_LOG::OTHER_NEW_COUNTER));
  LOG_INFO("other delete " + std::to_string(BETA_LOG::OTHER_DELETE_COUNTER));
  LOG_INFO("diff " + std::to_string(BETA_LOG::NEW_COUNTER-BETA_LOG::DELETE_COUNTER));
  LOG_INFO("other diff " + std::to_string(BETA_LOG::OTHER_NEW_COUNTER-BETA_LOG::OTHER_DELETE_COUNTER));
  LOG_INFO("output counter " + std::to_string(this->input_branch_counter_))
  LOG_INFO("input counter " + std::to_string(this->output_branch_counter_))
  LOG_INFO("delete output counter " + std::to_string(i_count))
  LOG_INFO("delete input counter " + std::to_string(o_count))
  */
}


// template <typename o_type, typename i_type>
template <>
// void ArgoneXrayAna::copyTTreeReaderArrayToVector( o_type *output_v,
// TTreeReaderArray<i_type> *input_v){
void BetaScope::copyTTreeReaderArrayToVector<std::vector<double>>(
    std::string oBranchName, std::string iBranchName, int entry) {
  // void copyTTreeReaderArrayToVector( o_type *output_v,
  // TTreeReaderArray<i_type> *input_v){ std::cout<<
  // this->beta_scope.iTreeDoubleArrayMap[branchName]->GetSize() << std::endl;
  // std::cout<< this->iTreeDoubleArrayMap[iBranchName]->GetSize() << std::endl;
  // TThread::Lock();
  // TTreeReaderArray<double> *mylocalArray =
  // this->iTreeDoubleArrayMap[iBranchName];
  TTreeReader *mylocalReader = new TTreeReader("wfm", this->input_tfile_);
  TTreeReaderArray<double> *mylocalArray = new TTreeReaderArray<double>(*mylocalReader, iBranchName.c_str());
  mylocalReader->SetEntry(entry);
  // std::cout << "local reader " << entry  << std::endl;
  for (int i = 0, max = mylocalArray->GetSize(); i < max; i++) {
    // std::cout<< i << std::endl;
    // std::cout << this->iTreeDoubleArrayMap[iBranchName]->At(i) << std::endl;
    // this->oTreeVecDoubleMap[oBranchName]->push_back( mylocalArray->At(i)
    // );//this->iTreeDoubleArrayMap[iBranchName]->At(i) );
  }
  // TThread::UnLock();
  delete mylocalArray;
  delete mylocalReader;
}

bool BetaScope::IsBranchExists(const char *branchName) {
  auto branch_checker = ((TTree *)this->input_tfile_->Get(this->input_tree_name_.c_str()))->GetListOfBranches()->FindObject(branchName);
  if (branch_checker != NULL)
    return true;
  else
    return false;
}
