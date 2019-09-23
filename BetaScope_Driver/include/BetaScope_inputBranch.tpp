#include "BetaScope_Class.h"

template < template<class> class ibranchType, typename dtype>
bool BetaScope::set_iBranch(  std::string branchName, std::string key )
{
  std::string function_name="BetaScope::set_iBranch";
  
  try{
    this->iTree_branch[this->iTreeBranchCounter] = new PrimitiveDataType_TemplateContainer<ibranchType, dtype>();
    this->iTree_branchMap.insert( std::pair<std::string,  PrimitiveDataType_BaseContainer* >(key, this->iTree_branch[this->iTreeBranchCounter]) );
    static_cast<PrimitiveDataType_TemplateContainer<ibranchType,dtype>*>(this->iTree_branchMap[key])->set( this->treeReader, branchName);
    ibranchType<dtype>* my_ibranch = BetaScope::get_iBranch<ibranchType, dtype>(key);
    if(my_ibranch)
    {
      ColorCout::Msg(function_name, "Branch: "+branchName+" is read." );
    }
    this->iTree_branchMapIndex.insert( std::pair<std::string, int>(key, this->iTreeBranchCounter) );
    this->iTreeBranchCounter+=1;

    /*
    if constexpr( std::is_same< ibranchType<dtype>, TTreeReaderValue<int>>::value )
    {
      this->iTree_int_valueReaderKeeper.push_back( my_ibranch);
    }
    else if constexpr( std::is_same< ibranchType<dtype>, TTreeReaderValue<double>>::value )
    {
      this->iTree_double_valueReaderKeeper.push_back( my_ibranch );
    }
    else if constexpr( std::is_same< ibranchType<dtype>, TTreeReaderValue<float>>::value )
    {
      this->iTree_float_valueReaderKeeper.push_back( my_ibranch );
    }
    else if constexpr( std::is_same< ibranchType<dtype>, TTreeReaderArray<int>>::value )
    {
      this->iTree_int_arrayReaderKeeper.push_back( my_ibranch );
    }
    else if constexpr( std::is_same< ibranchType<dtype>, TTreeReaderArray<double>>::value )
    {
      this->iTree_double_arrayReaderKeeper.push_back( my_ibranch );
    }
    else if constexpr( std::is_same< ibranchType<dtype>, TTreeReaderArray<float>>::value )
    {
      this->iTree_float_arrayReaderKeeper.push_back( my_ibranch );
    }
    else{}
    */


    return true;
  }
  catch(...){
    return false;
  }
}


template < template<class> class ibranchType, typename dtype>
ibranchType<dtype>* BetaScope::get_iBranch( std::string key )
{
  return static_cast<PrimitiveDataType_TemplateContainer<ibranchType,dtype>*>(this->iTree_branchMap[key])->get();
}
