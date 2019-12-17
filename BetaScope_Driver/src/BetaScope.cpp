#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "Colorful_Cout/include/Colorful_Cout.h"

#include <string>
#include <iostream>
#include <map>

//template <typename o_type, typename i_type>
template <>
//void ArgoneXrayAna::copyTTreeReaderArrayToVector( o_type *output_v, TTreeReaderArray<i_type> *input_v){
void BetaScope::copyTTreeReaderArrayToVector<std::vector<double>>( std::string oBranchName, std::string iBranchName, int entry) {
//void copyTTreeReaderArrayToVector( o_type *output_v, TTreeReaderArray<i_type> *input_v){
    //std::cout<< this->beta_scope.iTreeDoubleArrayMap[branchName]->GetSize() << std::endl;
    //std::cout<< this->iTreeDoubleArrayMap[iBranchName]->GetSize() << std::endl;
    //TThread::Lock();
    //TTreeReaderArray<double> *mylocalArray = this->iTreeDoubleArrayMap[iBranchName];
    TTreeReader *mylocalReader =  new TTreeReader( "wfm", this->input_tfile_ );
    TTreeReaderArray<double> *mylocalArray = new TTreeReaderArray<double>( *mylocalReader, iBranchName.c_str());
    mylocalReader->SetEntry(entry);
    //std::cout << "local reader " << entry  << std::endl;
    for(int i = 0, max = mylocalArray->GetSize(); i < max; i++)
    {
        //std::cout<< i << std::endl;
        //std::cout << this->iTreeDoubleArrayMap[iBranchName]->At(i) << std::endl;
        //this->oTreeVecDoubleMap[oBranchName]->push_back( mylocalArray->At(i) );//this->iTreeDoubleArrayMap[iBranchName]->At(i) );
    }
    //TThread::UnLock();
    delete mylocalArray;
    delete mylocalReader;
}

bool BetaScope::IsBranchExists( const char* branchName )
{
    auto branch_checker = ((TTree *) this->input_tfile_->Get(this->input_tree_name_.c_str()))->GetListOfBranches()->FindObject( branchName );
    if( branch_checker!=NULL )return true;
    else return false;
}
