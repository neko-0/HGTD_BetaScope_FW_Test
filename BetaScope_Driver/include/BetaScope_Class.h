    ///////////////////////////////
   //                           //
  // Argument class header     //
 //                           //
///////////////////////////////

#ifndef BETACOPE_H
#define BETACOPE_H

#include "General/Colorful_Cout/include/Colorful_Cout.h"

//-------c++----------------//
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <utility>      // std::pair, std::make_pair
//#include <boost/unordered_map.hpp>
#include <unordered_map>

#include <boost/type_traits/is_same.hpp>

//------ROOT----------------//
#include <TFile.h>
#include <TTree.h>
#include <TThread.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

#define VERBOSITY 0

//helper function from stack overflow

template <class N>
struct is_vector{
  static const bool value = false;
  using T = N;
};

template <class N, class A>
struct is_vector<std::vector<N,A>>{
  static const bool value = true;
  using T = std::vector<N,A>;
};



//==============================================================================

template<typename T>
struct DataType{using type = T;};

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
    dtype *data_type = new dtype;
    is_vector<dtype> _isVec;
  public:
    PrimitiveDataType_Container(){};
    virtual ~PrimitiveDataType_Container()
    {
      if(this->data_type){delete this->data_type;}
    };

    dtype *get(){ return this->data_type; };

    void clear(){ if constexpr(_isVec.value){this->data_type->clear();} }
};

template <template<class> class c, typename dtype>
struct PrimitiveDataType_TemplateContainer : public PrimitiveDataType_BaseContainer
{
  private:
    c<dtype> *data_type;
    std::string class_name = "PrimitiveDataType_TemplateContainer";
  public:
    PrimitiveDataType_TemplateContainer(){};
    virtual ~PrimitiveDataType_TemplateContainer()
    {
      ColorCout::print( class_name, "clean up.", YELLOW);
      if(this->data_type){delete this->data_type;}
    };

    c<dtype> *get(){return this->data_type; };
    void set( TTreeReader* itree, std::string branchName)
    {
      this->data_type = new c<dtype>( *itree, branchName.c_str());
    };

    void del(){ if(this->data_type){ delete this->data_type;} };
};

//==============================================================================


//==============================================================================

class BetaScope
{
  private:

    // output stuff are here.

    TFile *oFile = new TFile;
    TTree *oTree = new TTree;
    std::string filePrefix = "stats_";
    std::string ofileName;
    int compressionLevel = 8;
    int newBranchCountKeeper = 0;

    PrimitiveDataType_BaseContainer *oTreePrimitiveBranches[500];
    std::map<std::string, PrimitiveDataType_BaseContainer * > oTreePrimitiveBranchesMap;
    std::map<std::string, int > oTreePrimitiveBranchesMapIndex;
    int oTreePrimitiveBranchCounter = 0;

    // output stl vector keepers
    std::vector<int> oTreeSTLVectorReservedIndex = {};
    std::vector<PrimitiveDataType_BaseContainer *> oTree_STLVecotrKeeper = {};
    std::vector< std::vector<int>* > oTreeSTLVecotr_Int_keeper = {};
    std::vector< std::vector<double>* > oTreeSTLVecotr_Double_keeper = {};
    std::vector< std::vector<float>* > oTreeSTLVecotr_Float_keeper = {};
    std::vector< std::vector<bool>* > oTreeSTLVecotr_Bool_keeper = {};
    std::vector< std::vector<char>* > oTreeSTLVecotr_Char_keeper = {};

    // input stuff are here

    unsigned int i_numEvent;
    std::string i_fileName;
    std::string i_fileNickName;
    bool skipBadVector = false;
    bool i_ifileOpened = false;
    std::string objectLocation = "default";

    TFile *iFile = new TFile;
    TTreeReader *treeReader = new TTreeReader;
    std::string iTreeName = "wfm";


    PrimitiveDataType_BaseContainer *iTree_branch[500];
    std::map<std::string, PrimitiveDataType_BaseContainer *> iTree_branchMap;
    std::map<std::string, int> iTree_branchMapIndex;
    /*
    std::vector<TTreeReaderArray<int>*> iTree_int_arrayReaderKeeper = {};
    std::vector<TTreeReaderArray<double>*> iTree_double_arrayReaderKeeper = {};
    std::vector<TTreeReaderArray<float>*> iTree_float_arrayReaderKeeper = {};
    std::vector<TTreeReaderValue<int>*> iTree_int_valueReaderKeeper = {};
    std::vector<TTreeReaderValue<double>*> iTree_double_valueReaderKeeper = {};
    std::vector<TTreeReaderValue<float>*> iTree_float_valueReaderKeeper = {};
    */
    int iTreeBranchCounter = 0;

  public:

    std::vector<unsigned int> channel = {};
    std::vector<int> invertPulse = {};

    static const int numCh = 100; // reserving number of input channels.

    std::time_t cpuTime = std::clock();
    std::time_t _t_object_creation = std::time(nullptr);

    BetaScope(){};
    BetaScope(const char* ipath);
    BetaScope(const char* ipath, const char* iTreeBranch_config, const char* oTreeBranch_config);
    ~BetaScope()
    {
      std::cout << this << " call destructor at " << this->objectLocation <<std::endl;
      //delete this->iTree;
      //delete this->iFile;
      //delete this->oTree;
      //delete this->oFile;
    };

    bool fileIO_Open( const char *ifileName);
    bool rawTreeReader( const char* itreeName = "wfm" );
    bool newTreeMaker( std::string additional_branch_list );
    void fillEvent();
    void _clearVecBuffer();
    void _clearVecBuffer(std::string mode);
    void fileIO_Close();

    //=========================================================================
    // reading and getting branches methods for input ttree

    template < template<class> class ibranchType, typename dtype>
    bool set_iBranch( std::string my_branchName, std::string my_key );

    template < template<class> class ibranchType, typename dtype>
    bool set_iBranch( const char* my_branchName, const char* my_key )
    {
      std::string branchName = my_branchName;
      std::string key = my_key;
      return set_iBranch<ibranchType, dtype>(branchName, key);
    }

    template < template<class> class ibranchType, typename dtype>
    ibranchType<dtype> *get_iBranch( std::string key );

    //=========================================================================
    // building and getting branches methods for ouput ttree

    template <typename dtype>
    bool buildPrimitiveBranch( std::string branchName );

    template <typename dtype>
    typename DataType<dtype>::type *get_oTree_PrimitiveBranch(std::string branchName);

    //==========================================================================


    template <typename o_type>
    void copyTTreeReaderArrayToVector( std::string oBranchName, std::string iBranchName, int entry);

    std::vector< std::tuple<int, std::string, std::string, std::string> > branchConfigReader( std::string configName );

    //==========================================================================
    // class getter methods

    std::string get_ifile_name()const{ return this->i_fileName; }
    std::string get_ifile_nickName()const{ return this->i_fileNickName; }
    std::string get_iTreeName()const{ return this->iTreeName; }
    TTreeReader *get_treeReader()const{return this->treeReader;}
    int get_iNumEvent()const{ return this->i_numEvent; }

    TFile *get_ofile()const{ return this->oFile; }
    TTree *get_otree()const{ return this->oTree; }
    std::string get_ofile_prefix()const{ return this->filePrefix; }
    std::string get_ofile_name()const{ return this->ofileName; }
    int get_ofile_compressionLevel()const{ return this->compressionLevel; }

    //==========================================================================
    // class setter methods

    void set_ifile_name( std::string i_value ){ this->i_fileName = i_value; }
    void set_ifile_nickName( std::string i_value ){ this->i_fileNickName = i_value; }
    void set_iTreeName( std::string i_value ){ this->iTreeName = i_value; }

    void set_ofile_prefix( std::string i_value ){ this->filePrefix = i_value; }
    void set_ofile_name( std::string i_value ){ this->ofileName = i_value; }
    void set_ofile_compressionLevel( int i_value ){ this->compressionLevel = i_value; }


};



#endif // BETACOPE_H
