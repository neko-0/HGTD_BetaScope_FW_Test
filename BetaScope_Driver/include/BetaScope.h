///////////////////////////////
//                           //
// Argument class header     //
//                           //
///////////////////////////////

#ifndef BETACOPE_H
#define BETACOPE_H

#include "General/logger/include/logger.h"

//-------c++----------------//
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <utility> // std::pair, std::make_pair
#include <vector>
//#include <boost/unordered_map.hpp>
#include <unordered_map>

#include <boost/type_traits/is_same.hpp>
#include <boost/format.hpp>

//------ROOT----------------//
#include <TROOT.h>
#include <TFile.h>
#include <TThread.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

#define VERBOSITY 0

// helper function from stack overflow

template <class N> struct is_vector
{
  static const bool value = false;
  using T = N;
};


template <class N, class A> struct is_vector<std::vector<N, A>>
{
  static const bool value = true;
  using T = std::vector<N, A>;
};


//==============================================================================
struct TypeWrapperBase
{
  TypeWrapperBase(){};
  virtual ~TypeWrapperBase(){};

  virtual void Clear(){};
  virtual void Set(){};
};

template <typename T>
struct TypeWrapper : public TypeWrapperBase
{
  private:
    std::string name_;
    T *data_ = nullptr;

  public:
    TypeWrapper(const std::string name) : name_(name)
    {
      LOG_INFO("Building " + this->name_);
    }

    ~TypeWrapper()
    {
      if(this->data_)
      {
        delete this->data_;
      }
    }

    T *Get(){ return this->data_; }
    void Set( T *data){ this->data_ = data; }

    void Clear()
    {
      if constexpr(is_vector<T>::value)
      {
          this->data_->clear();
      }
    };
};


//==============================================================================
struct BranchConfig
{
  int mode;
  std::string type;
  std::string key;
  std::string branch_name;
};


//==============================================================================

//==============================================================================

class BetaScope {
  protected:
    // output stuff are here.
    TFile *output_tfile_ = nullptr;
    TTree *output_ttree_ = nullptr;
    TTree *skim_output_tree_ = nullptr;
    std::string output_file_prefix_ = "stats_";
    std::string output_file_name_ = "output";
    std::string output_tree_name_ = "wfm";
    int compression_level_ = 1;
    int new_branch_counter_ = 0;
    bool write_skim_ = false;

    std::vector<TypeWrapperBase*> output_branch_buffer_ = {};
    std::map<std::string, TypeWrapperBase*> output_branch_map_;
    std::map<std::string, int> output_branch_map_index_;
    int output_branch_counter_ = 0;
    std::vector<int> output_vector_reserved_index_ = {};

    // output stl vector keepers
    std::vector<TypeWrapperBase*> output_vector_keeper_ = {};

    // input stuff are here
    unsigned int input_num_event_ = 0;
    std::string input_file_name_ = "";
    std::string input_file_nick_name_ = "";
    bool skip_bad_vector_ = false;
    bool is_file_opened_ = false;
    std::string object_location_ = "default";

    TFile *input_tfile_ = nullptr;
    TTreeReader *input_tree_reader_ = nullptr;
    std::string input_tree_name_ = "wfm";

    std::vector<TypeWrapperBase*> input_branch_buffer_ = {};
    std::map<std::string, TypeWrapperBase*> input_branch_map_;
    std::map<std::string, int> input_branch_map_index_;
    int input_branch_counter_ = 0;

  public:
    std::vector<unsigned int> channel = {};
    std::vector<int> invert_pulse = {};

    static const int kNumChannels = 100; // reserving number of input channels.

    std::time_t cpu_time = std::clock();
    const std::time_t kTimeObjCreation = std::time(nullptr);

    BetaScope(){};

    BetaScope(const std::string &ifile);

    ~BetaScope();

    void FileClose();

    //=========================================================================
    // methods for input tfile and ttree

    bool IsBranchExists(const std::string &branchName);

    template <typename dtype>
    dtype *GetInBranch(const std::string &key);

    TFile *GetInFile() const;

    TTreeReader *GetInTreeReader() const;

    int GetInNumEvent() const;

    std::string GetInFileName() const;

    std::string GetInFileNickName() const;

    std::string GetInTreeName() const;

    template <typename dtype>
    bool SetInBranch(const std::string &branchName, const std::string &key);

    void SetInFileName(const std::string &value);

    void SetInFileNickName(const std::string &value);

    void SetInTreeName(const std::string &value);

    bool SetTreeReader();

    bool FileOpen(const std::string &ifileName);

    bool NextEvent();

    //=========================================================================
    // methods for output tfile and ttree

    template <typename dtype>
    bool BuildOutBranch(const std::string &branchName, const int &size=2000);

    bool BuildOutTree(const std::string &additional_branch_list);

    template <typename dtype>
    dtype *GetOutBranch(const std::string &branchName);

    TFile *GetOutFile() const;

    TTree *GetOutTree() const;

    std::string GetOutFilePrefix() const;

    std::string GetOutFileName() const;

    int GetCompLevel() const;

    void SetOutFilePrefix(const std::string &value);

    void SetOutFileName(const std::string &value);

    template <typename dtype>
    void SetOutBranchValue(const std::string &branchName, const dtype &value);

    void SetCompLevel(const int &i_value);

    void FillEvent();

    void ClearVecBuffer();

    //==========================================================================

    std::vector<BranchConfig> BranchConfigReader(const std::string &configName);
};


template <typename dtype>
bool BetaScope::SetInBranch(const std::string &branchName, const std::string &key)
{
  std::string function_name = "BetaScope::SetInBranch";

  try
  {
    auto my_ptr = new TypeWrapper<dtype>(branchName);
    this->input_branch_buffer_.push_back(my_ptr);
    this->input_branch_map_.insert(std::pair<std::string, TypeWrapperBase*>(key, this->input_branch_buffer_[this->input_branch_counter_]));
    this->input_branch_map_index_.insert( std::pair<std::string, int>(key, this->input_branch_counter_));

    static_cast<TypeWrapper<dtype>*>(this->input_branch_map_[key])->Set(new dtype(*this->input_tree_reader_, branchName.c_str()));

    dtype *my_ibranch = BetaScope::GetInBranch<dtype>(key);
    if(my_ibranch)
    {
      LOG_INFO("Read Branch: " + branchName);
    }

    this->input_branch_counter_++;
    return true;
  }
  catch (...)
  {
    LOG_WARNING("Unable to read branch: " + branchName);
    this->input_branch_counter_++;
    return false;
  }
}

template <typename dtype>
dtype *BetaScope::GetInBranch(const std::string &key)
{
  return static_cast<TypeWrapper<dtype>*>(this->input_branch_map_[key])->Get();
}

template <typename dtype>
bool BetaScope::BuildOutBranch(const std::string &branchName, const int &size) {
  try
  {
    auto my_ptr = new TypeWrapper<dtype>(branchName);
    this->output_branch_buffer_.push_back( my_ptr );
    this->output_branch_map_.insert(std::pair<std::string, TypeWrapperBase*>(branchName, this->output_branch_buffer_[this->output_branch_counter_]));
    this->output_branch_map_index_.insert(std::pair<std::string, int>(branchName, this->output_branch_counter_));

    this->output_ttree_->Branch(branchName.c_str(), static_cast<TypeWrapper<dtype>*>(this->output_branch_buffer_[this->output_branch_counter_])->Get());

    if constexpr(is_vector<dtype>::value)
    {
      this->output_vector_reserved_index_.push_back(this->new_branch_counter_);

      LOG_INFO( "BetaScope::BuildOutBranch Branch:" + branchName + " is std::vector, Handle buffer internally");

      static_cast<TypeWrapper<dtype>*>(this->output_branch_buffer_[this->output_branch_counter_])->Get()->reserve(size);

      this->output_vector_keeper_.push_back( this->output_branch_buffer_[this->output_branch_counter_] );
    }
    else
    {
      LOG_INFO( "BetaScope::BuildOutBranch Branch:" + branchName + " is NOT std::vector. No action is needed.");
    }

    this->output_branch_counter_++;
    return true;
  }
  catch (...)
  {
    this->input_branch_counter_++;
    return false;
  }
}

template <typename dtype>
dtype *BetaScope::GetOutBranch(const std::string &branchName)
{
  if(this->output_branch_map_.find(branchName) == this->output_branch_map_.end() )
  {
    return nullptr;
  }
  else
  {
    return static_cast<TypeWrapper<dtype>*>(this->output_branch_map_[branchName])->Get();
  }
}

template <typename dtype>
void BetaScope::SetOutBranchValue(const std::string &branchName, const dtype &value)
{
  dtype *m_ptr = this->GetOutBranch<dtype>(branchName);
  if(m_ptr)
  {
    *m_ptr = value;
  }
}

#endif // BETACOPE_H
