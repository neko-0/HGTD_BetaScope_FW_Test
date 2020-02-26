#include "BetaScope_Class.h"


template <typename dtype>
bool BetaScope::BuildOutBranch(std::string branchName, int size) {
  try
  {
    this->output_branches_buffer_[this->output_branch_counter_] = new PrimitiveDataType_Container<dtype>();
    this->output_branch_map_.insert( std::pair<std::string, PrimitiveDataType_BaseContainer *>( branchName, this->output_branches_buffer_[this->output_branch_counter_]));
    this->output_ttree_->Branch( branchName.c_str(), static_cast<PrimitiveDataType_Container<dtype> *>( this->output_branches_buffer_[this->output_branch_counter_])->get());
    this->output_branch_map_index_.insert(std::pair<std::string, int>(branchName, this->output_branch_counter_));

    if (is_vector<dtype>::value)
    {
      this->output_vector_reserved_index_.push_back(this->new_branch_counter_);


      typedef typename is_vector<dtype>::T v;
      ///*
      if (std::is_same<v, std::vector<int>>::value)
      {
        logger.info( "BetaScope::BuildOutBranch", "Branch:" + branchName + " is std::vector<int>, Handle buffer internally");
        static_cast<PrimitiveDataType_Container<std::vector<int>> *>(this->output_branches_buffer_[this->output_branch_counter_])->get()->reserve(size);
        this->output_int_vector_keeper_.push_back( static_cast<PrimitiveDataType_Container<std::vector<int>> *>( this->output_branches_buffer_[this->output_branch_counter_])->get());
        this->output_vector_keeper_.push_back( static_cast<PrimitiveDataType_Container<std::vector<int>> *>( this->output_branches_buffer_[this->output_branch_counter_]));
      }
      else if (std::is_same<v, std::vector<double>>::value)
      {
        logger.info( "BetaScope::BuildOutBranch", "Branch:" + branchName + " is std::vector<double>, Handle buffer internally");
        static_cast<PrimitiveDataType_Container<std::vector<double>>*>(this->output_branches_buffer_[this->output_branch_counter_])->get()->reserve(size);
        this->output_double_vector_keeper_.push_back( static_cast<PrimitiveDataType_Container<std::vector<double>> *>(this->output_branches_buffer_[this->output_branch_counter_])->get());
        this->output_vector_keeper_.push_back( static_cast<PrimitiveDataType_Container<std::vector<double>> *>( this->output_branches_buffer_[this->output_branch_counter_]));

      }
      else if (std::is_same<v, std::vector<float>>::value)
      {
        logger.info( "BetaScope::BuildOutBranch", "Branch:" + branchName + " is std::vector<float>, Handle buffer internally");
        static_cast<PrimitiveDataType_Container<std::vector<float>>*>(this->output_branches_buffer_[this->output_branch_counter_])->get()->reserve(size);
        this->output_float_vector_keeper_.push_back( static_cast<PrimitiveDataType_Container<std::vector<float>> *>(this->output_branches_buffer_[this->output_branch_counter_])->get());
        this->output_vector_keeper_.push_back(static_cast<PrimitiveDataType_Container<std::vector<float>> *>( this->output_branches_buffer_[this->output_branch_counter_]));
      }
      else if (std::is_same<v, std::vector<bool>>::value)
      {
        logger.info( "BetaScope::BuildOutBranch", "Branch:" + branchName + " is std::vector<bool>, Handle buffer internally");
        static_cast<PrimitiveDataType_Container<std::vector<bool>>*>(this->output_branches_buffer_[this->output_branch_counter_])->get()->reserve(size);
        this->output_bool_vector_keeper_.push_back( static_cast<PrimitiveDataType_Container<std::vector<bool>> *>( this->output_branches_buffer_[this->output_branch_counter_])->get());
        this->output_vector_keeper_.push_back(static_cast<PrimitiveDataType_Container<std::vector<bool>> *>(this->output_branches_buffer_[this->output_branch_counter_]));
      }
      else if (std::is_same<v, std::vector<char>>::value)
      {
        logger.info( "BetaScope::BuildOutBranch", "Branch:" + branchName + " is std::vector<char>, Handle buffer internally");
        static_cast<PrimitiveDataType_Container<std::vector<char>>*>(this->output_branches_buffer_[this->output_branch_counter_])->get()->reserve(size);
        this->output_char_vector_keeper_.push_back( static_cast<PrimitiveDataType_Container<std::vector<char>> *>( this->output_branches_buffer_[this->output_branch_counter_])->get());
        this->output_vector_keeper_.push_back( static_cast<PrimitiveDataType_Container<std::vector<char>> *>( this->output_branches_buffer_[this->output_branch_counter_]));
      }
      else
      {
        logger.info( "BetaScope::BuildOutBranch", "Branch:" + branchName + " is std::vector<" + typeid(dtype).name() + ">, Handle buffer internally");
        this->output_vector_keeper_.push_back( static_cast<PrimitiveDataType_Container<std::vector<dtype>> *>( this->output_branches_buffer_[this->output_branch_counter_]));
      }
    }
    else
    {
      logger.info( "BetaScope::BuildOutBranch", "Branch:" + branchName + " is NOT std::vector. No action is needed.");
    }

    this->output_branch_counter_++;
    return true;
  }
  catch (...)
  {
    return false;
  }
}

template <typename dtype>
typename DataType<dtype>::type *
BetaScope::GetOutBranch(std::string branchName)
{
  return static_cast<PrimitiveDataType_Container<dtype> *>(this->output_branch_map_[branchName])->get();
}
