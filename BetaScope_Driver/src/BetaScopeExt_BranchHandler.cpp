#include "BetaScope_Driver/include/BetaScopeExt_Class.h"

template <>
bool BetaScope_Ext::BuildBranch<double>(std::string branchName ) {
    return BetaScope::BuildOutBranch<double>(branchName);
}

template <>
bool BetaScope_Ext::BuildBranch<int>(std::string branchName ) {
    return BetaScope::BuildOutBranch<int>(branchName);
}

template <>
bool BetaScope_Ext::BuildBranch<std::vector<double>>(std::string branchName ) {
    return BetaScope::BuildOutBranch<std::vector<double>>(branchName);
}

template <>
bool BetaScope_Ext::BuildBranch<std::vector<int>>(std::string branchName ) {
    return BetaScope::BuildOutBranch<std::vector<int>>(branchName);
}

template <>
bool BetaScope_Ext::BuildBranch<TH1I>( std::string branchName )
{
    try {
        this->oTree_TH1I[this->new_branch_counter_keeper_] = new TH1I;
        this->oTree_TH1I_Map.insert( std::pair<std::string, TH1I*>(branchName, this->oTree_TH1I[this->new_branch_counter_keeper_]) );
        this->output_ttree_->Branch( branchName.c_str(), this->oTree_TH1I[this->new_branch_counter_keeper_] );
        //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, this->new_branch_counter_keeper_) );
        //this->reserved_vec_i.push_back( this->this->new_branch_counter_keeper_ );
        this->new_branch_counter_keeper_++;
        return true;
    }
    catch(...) {
        return false;
    }
}

template <>
bool BetaScope_Ext::BuildBranch<TH1F>( std::string branchName )
{
    try {
        this->oTree_TH1F[this->new_branch_counter_keeper_] = new TH1F;
        this->oTree_TH1F_Map.insert( std::pair<std::string, TH1F*>(branchName, this->oTree_TH1F[this->new_branch_counter_keeper_]) );
        this->output_ttree_->Branch( branchName.c_str(), this->oTree_TH1F[this->new_branch_counter_keeper_] );
        //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, this->new_branch_counter_keeper_) );
        //this->reserved_vec_i.push_back( this->this->new_branch_counter_keeper_ );
        this->new_branch_counter_keeper_++;
        return true;
    }
    catch(...) {
        return false;
    }
}

template <>
bool BetaScope_Ext::BuildBranch<TH1D>( std::string branchName )
{
    try {
        this->oTree_TH1D[this->new_branch_counter_keeper_] = new TH1D;
        this->oTree_TH1D_Map.insert( std::pair<std::string, TH1D*>(branchName, this->oTree_TH1D[this->new_branch_counter_keeper_]) );
        this->output_ttree_->Branch( branchName.c_str(), this->oTree_TH1D[this->new_branch_counter_keeper_] );
        //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, this->new_branch_counter_keeper_) );
        //this->reserved_vec_i.push_back( this->this->new_branch_counter_keeper_ );
        this->new_branch_counter_keeper_++;
        return true;
    }
    catch(...) {
        return false;
    }
}

template <>
bool BetaScope_Ext::BuildBranch<std::vector<TH1I>>( std::string branchName )
{
    try {
        this->oTree_TH1I_Array[this->new_branch_counter_keeper_] = new std::vector<TH1I>;
        this->oTree_TH1I_Array_Map.insert( std::pair<std::string, std::vector<TH1I>*>(branchName, this->oTree_TH1I_Array[this->new_branch_counter_keeper_]) );
        this->output_ttree_->Branch( branchName.c_str(), this->oTree_TH1I_Array[this->new_branch_counter_keeper_] );
        //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, this->new_branch_counter_keeper_) );
        //this->reserved_vec_i.push_back( this->this->new_branch_counter_keeper_ );
        this->new_branch_counter_keeper_++;
        return true;
    }
    catch(...) {
        return false;
    }
}

template <>
bool BetaScope_Ext::BuildBranch<std::vector<TH1F>>( std::string branchName )
{
    try {
        this->oTree_TH1F_Array[this->new_branch_counter_keeper_] = new std::vector<TH1F>;
        this->oTree_TH1F_Array_Map.insert( std::pair<std::string, std::vector<TH1F>*>(branchName, this->oTree_TH1F_Array[this->new_branch_counter_keeper_]) );
        this->output_ttree_->Branch( branchName.c_str(), this->oTree_TH1F_Array[this->new_branch_counter_keeper_] );
        //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, this->new_branch_counter_keeper_) );
        //this->reserved_vec_i.push_back( this->this->new_branch_counter_keeper_ );
        this->new_branch_counter_keeper_++;
        return true;
    }
    catch(...) {
        return false;
    }
}

template <>
bool BetaScope_Ext::BuildBranch<std::vector<TH1D>>( std::string branchName )
{
    try {
        this->oTree_TH1D_Array[this->new_branch_counter_keeper_] = new std::vector<TH1D>;
        this->oTree_TH1D_Array_Map.insert( std::pair<std::string, std::vector<TH1D>*>(branchName, this->oTree_TH1D_Array[this->new_branch_counter_keeper_]) );
        this->output_ttree_->Branch( branchName.c_str(), this->oTree_TH1D_Array[this->new_branch_counter_keeper_] );
        //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, this->new_branch_counter_keeper_) );
        //this->reserved_vec_i.push_back( this->this->new_branch_counter_keeper_ );
        this->new_branch_counter_keeper_++;
        return true;
    }
    catch(...) {
        return false;
    }
}


bool BetaScope_Ext::SetBranch( std::string typeName, std::string key, std::string branchName)
{
    if( typeName.compare("TTreeReaderArray<double>") ==0 ) {
        return BetaScope::SetInBranch<TTreeReaderArray,double>( branchName.c_str(), key.c_str());
    }
    else if( typeName.compare("TTreeReaderArray<int>") ==0 ) {
        return BetaScope::SetInBranch<TTreeReaderArray,int>( branchName.c_str(), key.c_str());
    }
    else if( typeName.compare("TTreeReaderValue<double>") ==0 ) {
        return BetaScope::SetInBranch<TTreeReaderValue,double>( branchName.c_str(), key.c_str());
    }
    else if( typeName.compare("TTreeReaderValue<int>") ==0 ) {
        return BetaScope::SetInBranch<TTreeReaderValue,int>( branchName.c_str(), key.c_str());
    }
    else if( typeName.compare("TTreeReaderArray<TH1I>") == 0 )
    {
        this->iTree_TH1I_Array[this->input_branch_counter_] = new TTreeReaderArray<TH1I>( *this->input_tree_reader_, branchName.c_str() );
        this->iTree_TH1I_Array_Map.insert( std::pair<std::string, TTreeReaderArray<TH1I>*>(key, this->iTree_TH1I_Array[this->input_branch_counter_]) );
        //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->this->input_branch_counter_) );
        this->input_branch_counter_+=1;
        return true;
    }
    else if( typeName.compare("TTreeReaderArray<TH1F>") == 0 )
    {
        this->iTree_TH1F_Array[this->input_branch_counter_] = new TTreeReaderArray<TH1F>( *this->input_tree_reader_, branchName.c_str() );
        this->iTree_TH1F_Array_Map.insert( std::pair<std::string, TTreeReaderArray<TH1F>*>(key, this->iTree_TH1F_Array[this->input_branch_counter_]) );
        //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->this->input_branch_counter_) );
        this->input_branch_counter_+=1;
        return true;
    }
    else if( typeName.compare("TTreeReaderArray<TH1D>") == 0 )
    {
        this->iTree_TH1D_Array[this->input_branch_counter_] = new TTreeReaderArray<TH1D>( *this->input_tree_reader_, branchName.c_str() );
        this->iTree_TH1D_Array_Map.insert( std::pair<std::string, TTreeReaderArray<TH1D>*>(key, this->iTree_TH1D_Array[this->input_branch_counter_]) );
        //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->this->input_branch_counter_) );
        this->input_branch_counter_+=1;
        return true;
    }
    else if( typeName.compare("TTreeReaderValue<TH1I>") == 0 )
    {
        this->iTree_TH1I[this->input_branch_counter_] = new TTreeReaderValue<TH1I>( *this->input_tree_reader_, branchName.c_str() );
        this->iTree_TH1I_Map.insert( std::pair<std::string, TTreeReaderValue<TH1I>*>(key, this->iTree_TH1I[this->input_branch_counter_]) );
        //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->this->input_branch_counter_) );
        this->input_branch_counter_+=1;
        return true;
    }
    else if( typeName.compare("TTreeReaderValue<TH1F>") == 0 )
    {
        this->iTree_TH1F[this->input_branch_counter_] = new TTreeReaderValue<TH1F>( *this->input_tree_reader_, branchName.c_str() );
        this->iTree_TH1F_Map.insert( std::pair<std::string, TTreeReaderValue<TH1F>*>(key, this->iTree_TH1F[this->input_branch_counter_]) );
        //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->this->input_branch_counter_) );
        this->input_branch_counter_+=1;
        return true;
    }
    else if( typeName.compare("TTreeReaderValue<TH1D>") == 0 )
    {
        this->iTree_TH1D[this->input_branch_counter_] = new TTreeReaderValue<TH1D>( *this->input_tree_reader_, branchName.c_str() );
        this->iTree_TH1D_Map.insert( std::pair<std::string, TTreeReaderValue<TH1D>*>(key, this->iTree_TH1D[this->input_branch_counter_]) );
        //this->iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, this->this->input_branch_counter_) );
        this->input_branch_counter_+=1;
        return true;
    }
    else {
        return false;
    }
}
