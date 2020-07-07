#include "BetaScope_Class.h"


template <template <class> class ibranchType, typename dtype>
bool BetaScope::SetInBranch(std::string branchName, std::string key)
{
  std::string function_name = "BetaScope::SetInBranch";

  try
  {
    //this->input_branches_buffer_[this->input_branch_counter_] = new PrimitiveDataType_TemplateContainer<ibranchType, dtype>();
    //BETA_LOG::NEW_=true;
    auto my_ptr = new PrimitiveDataType_TemplateContainer<ibranchType, dtype>();
    this->input_branches_buffer_.push_back( my_ptr );
    this->input_branch_map_.insert( std::pair<std::string, PrimitiveDataType_BaseContainer *>( key, this->input_branches_buffer_[this->input_branch_counter_]));
    static_cast<PrimitiveDataType_TemplateContainer<ibranchType, dtype> *>( this->input_branch_map_[key])->set(this->input_tree_reader_, branchName);
    ibranchType<dtype> *my_ibranch = BetaScope::GetInBranch<ibranchType, dtype>(key);
    if (my_ibranch)
    {
      LOG_INFO("Branch: " + branchName + " is read.");
    }
    this->input_branch_map_index_.insert( std::pair<std::string, int>(key, this->input_branch_counter_));
    this->input_branch_counter_++;

    /*
    if constexpr( std::is_same< ibranchType<dtype>,
    TTreeReaderValue<int>>::value )
    {
      this->iTree_int_valueReaderKeeper.push_back( my_ibranch);
    }
    else if constexpr( std::is_same< ibranchType<dtype>,
    TTreeReaderValue<double>>::value )
    {
      this->iTree_double_valueReaderKeeper.push_back( my_ibranch );
    }
    else if constexpr( std::is_same< ibranchType<dtype>,
    TTreeReaderValue<float>>::value )
    {
      this->iTree_float_valueReaderKeeper.push_back( my_ibranch );
    }
    else if constexpr( std::is_same< ibranchType<dtype>,
    TTreeReaderArray<int>>::value )
    {
      this->iTree_int_arrayReaderKeeper.push_back( my_ibranch );
    }
    else if constexpr( std::is_same< ibranchType<dtype>,
    TTreeReaderArray<double>>::value )
    {
      this->iTree_double_arrayReaderKeeper.push_back( my_ibranch );
    }
    else if constexpr( std::is_same< ibranchType<dtype>,
    TTreeReaderArray<float>>::value )
    {
      this->iTree_float_arrayReaderKeeper.push_back( my_ibranch );
    }
    else{}
    */

    return true;
  }
  catch (...)
  {
    return false;
  }
}

template <template <class> class ibranchType, typename dtype>
ibranchType<dtype> *BetaScope::GetInBranch(std::string key)
{
  return static_cast<PrimitiveDataType_TemplateContainer<ibranchType, dtype> *>(this->input_branch_map_[key])->get();
}
