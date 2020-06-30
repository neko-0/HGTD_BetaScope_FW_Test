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

template <typename T> struct DataType { using type = T; };

struct PrimitiveDataType_BaseContainer
{
  PrimitiveDataType_BaseContainer(){};
  virtual ~PrimitiveDataType_BaseContainer(){};

  virtual void clear(){};
};

template <typename dtype>
struct PrimitiveDataType_Container : public PrimitiveDataType_BaseContainer
{
private:
  dtype *data_type = nullptr;
  is_vector<dtype> _isVec;
  bool __isVec = _isVec.value;

  std::string class_name = "PrimitiveDataType_Container";

  template <class vClass>
  typename std::enable_if<is_vector<vClass>::value>::type _clear()
  {
    this->data_type->clear();
  };

  template <class vClass>
  typename std::enable_if<!is_vector<vClass>::value>::type _clear(){};

public:
  PrimitiveDataType_Container(){ this->data_type = new dtype; };
  ~PrimitiveDataType_Container(){
    //LOG_INFO(this->class_name+" delete ");
    delete this->data_type;
    this->data_type=nullptr;
  };

  dtype *get() { return this->data_type; };

  inline bool isVec() { return __isVec; };

  // inline void clear(){ if constexpr( __isVec ){this->data_type->clear();}
  // else{} }

  inline void clear() { _clear<dtype>(); };
};

template <template <class> class c, typename dtype>
struct PrimitiveDataType_TemplateContainer : public PrimitiveDataType_BaseContainer
{
private:
  c<dtype> *data_type = nullptr;
  std::string class_name = "PrimitiveDataType_TemplateContainer";

public:
  PrimitiveDataType_TemplateContainer(){};
  ~PrimitiveDataType_TemplateContainer(){
    //LOG_INFO(this->class_name+" delete ");
    delete this->data_type;
    this->data_type=nullptr;
  };

  c<dtype> *get() { return this->data_type; };
  void set(TTreeReader *itree, std::string branchName)
  {
    this->data_type = new c<dtype>(*itree, branchName.c_str());
  };

  dtype GetValue() { return **this->data_type; }

  void del()
  {
    if (this->data_type)
    {
      delete this->data_type;
    }
  };
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

  //PrimitiveDataType_BaseContainer *output_branches_buffer_[500] = {nullptr};
  std::vector<PrimitiveDataType_BaseContainer*> output_branches_buffer_ = {};
  std::map<std::string, PrimitiveDataType_BaseContainer *> output_branch_map_;
  std::map<std::string, int> output_branch_map_index_;
  int output_branch_counter_ = 0;

  // output stl vector keepers

  std::vector<int> output_vector_reserved_index_ = {};
  std::vector<PrimitiveDataType_BaseContainer *> output_vector_keeper_ = {};
  std::vector<std::vector<int> *> output_int_vector_keeper_ = {};
  std::vector<std::vector<double> *> output_double_vector_keeper_ = {};
  std::vector<std::vector<float> *> output_float_vector_keeper_ = {};
  std::vector<std::vector<bool> *> output_bool_vector_keeper_ = {};
  std::vector<std::vector<char> *> output_char_vector_keeper_ = {};

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

  //PrimitiveDataType_BaseContainer *input_branches_buffer_[500] = {nullptr};
  std::vector<PrimitiveDataType_BaseContainer*> input_branches_buffer_ = {};
  std::map<std::string, PrimitiveDataType_BaseContainer *> input_branch_map_;
  std::map<std::string, int> input_branch_map_index_;
  int input_branch_counter_ = 0;

public:
  std::vector<unsigned int> channel = {};
  std::vector<int> invert_pulse = {};

  static const int kNumChannels = 100; // reserving number of input channels.

  std::time_t cpu_time = std::clock();
  const std::time_t kTimeObjCreation = std::time(nullptr);

  BetaScope(){};
  BetaScope(const char *ipath);
  BetaScope(const char *ipath, const char *iTreeBranch_config, const char *oTreeBranch_config);
  ~BetaScope();

  bool FileOpen(const char *ifileName);
  void FileClose();
  bool RawTreeReader(const char *itreeName = "wfm");
  bool NewTreeMaker(std::string additional_branch_list);
  void FillEvent();
  bool IsBranchExists(const char *branchName);

  bool NextEvent();

  void _ClearVecBuffer();
  void _ClearVecBuffer(std::string mode);

  //=========================================================================
  // reading and getting branches methods for input ttree

  template <template <class> class ibranchType, typename dtype>
  bool SetInBranch(std::string my_branchName, std::string my_key);

  template <template <class> class ibranchType, typename dtype>
  bool SetInBranch(const char *my_branchName, const char *my_key) {
    std::string branchName = my_branchName;
    std::string key = my_key;
    return SetInBranch<ibranchType, dtype>(branchName, key);
  }

  template <template <class> class ibranchType, typename dtype>
  ibranchType<dtype> *GetInBranch(std::string key);

  template <template <class> class ibranchType, typename dtype>
  dtype GetInBranchValue(std::string key) {
    return **GetInBranch<ibranchType, dtype>(key);
  }

  //=========================================================================
  // building and getting branches methods for ouput ttree

  template <typename dtype>
  bool BuildOutBranch(const std::string &branchName, const int size=2000);

  template <typename dtype>
  typename DataType<dtype>::type *GetOutBranch(const std::string &branchName);

  template <typename dtype>
  void SetOutBranchValue(const std::string &branchName, const dtype i_value)
  {
    *GetOutBranch<dtype>(branchName) = i_value;
  }

  //==========================================================================

  template <typename o_type>
  void copyTTreeReaderArrayToVector(std::string oBranchName,
                                    std::string iBranchName, int entry);

  std::vector<std::tuple<int, std::string, std::string, std::string>>
  BranchConfigReader(std::string configName);

  //==========================================================================
  // class getter methods

  std::string GetInFileName() const { return this->input_file_name_; };

  std::string GetInFileNickName() const { return this->input_file_nick_name_; };

  std::string GetInTreeName() const { return this->input_tree_name_; };

  TTreeReader *GetInTreeReader() const { return this->input_tree_reader_; };

  int GetInNumEvent() const { return this->input_num_event_; };

  TFile *GetOutFile() const { return this->output_tfile_; };

  TTree *GetOutTree() const { return this->output_ttree_; };

  std::string GetOutFilePrefix() const { return this->output_file_prefix_; };

  std::string GetOutFileName() const { return this->output_file_name_; };

  int GetCompLevel() const { return this->compression_level_; };

  //==========================================================================
  // class setter methods

  void SetInFileName(std::string i_value) { this->input_file_name_ = i_value; };

  void SetInFileNickName(std::string i_value) {
    this->input_file_nick_name_ = i_value;
  };

  void SetInTreeName(std::string i_value) { this->input_tree_name_ = i_value; };

  void SetOutFilePrefix(std::string i_value) {
    this->output_file_prefix_ = i_value;
  };

  void SetOutFileName(std::string i_value) {
    this->output_file_name_ = i_value;
  };

  void SetCompLevel(int i_value) { this->compression_level_ = i_value; };
};

#endif // BETACOPE_H
