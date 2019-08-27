    ///////////////////////////////
   //                           //
  // Argument class header     //
 //                           //
///////////////////////////////

#ifndef BETACOPE_H
#define BETACOPE_H

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
};

template <typename dtype>
struct PrimitiveDataType_Container : public PrimitiveDataType_BaseContainer
{
  private:
    dtype *data_type = new dtype;
  public:
    PrimitiveDataType_Container(){};
    virtual ~PrimitiveDataType_Container(){};

    dtype *get(){return this->data_type; };
};

class BetaScope
{
  public:

    std::time_t cpuTime = std::clock();
    std::time_t _t_object_creation = std::time(nullptr);

    std::string ifileName;
    std::string ifileNickName;
    TFile *iFile = new TFile;
    TTreeReader *treeReader = new TTreeReader;
    unsigned int numEvent;
    static const int numCh = 100;
    //TTreeReaderArray<double> *voltageReader[numCh] = {};
    //TTreeReaderArray<double> *timeReader[numCh] = {};
    TTreeReaderArray<double> *iTreeDoubleArray[numCh] = {};
    TTreeReaderValue<double> *iTreeDoubleValue[numCh] = {};
    TTreeReaderArray<int> *iTreeIntArray[numCh] = {};
    TTreeReaderValue<int> *iTreeIntValue[numCh] = {};
    std::vector<unsigned int> channel = {};
    std::vector<int> invertPulse = {};
    std::map<std::string, TTreeReaderArray<double>*> iTreeDoubleArrayMap;
    std::map<std::string, TTreeReaderValue<double>*> iTreeDoubleValueMap;
    std::map<std::string, TTreeReaderArray<int>*> iTreeIntArrayMap;
    std::map<std::string, TTreeReaderValue<int>*> iTreeIntValueMap;
    std::map<std::string, int> iTreeDoubleArrayMapIndex;
    std::map<std::string, int> iTreeDoubleValueMapIndex;
    std::map<std::string, int> iTreeIntArrayMapIndex;
    std::map<std::string, int> iTreeIntValueMapIndex;
    int iTreeBranchCounter = 0;

    bool currentFromDAQ = false;
    bool timestampFromDAQ = false;
    bool temperatureFromDAQ = false;
    bool humidityFromDAQ = false;
    bool ieventFromDAQ = false;

    std::string iTreeName = "wfm";

    TFile *oFile = new TFile;
    TTree *oTree = new TTree;
    std::string filePrefix = "stats_";
    std::string ofileName;
    int compressionLevel = 8;

    std::vector<double> *oTreeVecDouble[100] = {};
    std::vector<int> *oTreeVecInt[100] = {};
    double *oTreeDouble[100] = {};
    int *oTreeInt[100] = {};
    std::map<std::string, std::vector<double> *> oTreeVecDoubleMap;
    std::map<std::string, std::vector<int> *> oTreeVecIntMap;
    std::map<std::string, double * > oTreeDoubleMap;
    std::map<std::string, int * > oTreeIntMap;
    std::map<std::string, int> oTreeVecDoubleMapIndex;
    std::map<std::string, int> oTreeDoubleMapIndex;
    std::map<std::string, int> oTreeIntMapIndex;
    std::map<std::string, int> oTreeVecIntMapIndex;
    std::vector<int> reserved_vec_d = {};
    std::vector<int> reserved_d = {};
    std::vector<int> reserved_vec_i = {};
    std::vector<int> reserved_i = {};

    int newBranchCounterKeeper = 0;


    //Experiment
    PrimitiveDataType_BaseContainer *oTreePrimitiveBranches[500];
    std::map<std::string, PrimitiveDataType_BaseContainer * > oTreePrimitiveBranchesMap;
    std::map<std::string, int > oTreePrimitiveBranchesMapIndex;
    int oTreePrimitiveBranchCounter = 0;
    std::vector<int> oTreeSTLVectorReservedIndex = {};
    std::vector< std::vector<int>* > oTreeSTLVecotr_Int_keeper = {};
    std::vector< std::vector<double>* > oTreeSTLVecotr_Double_keeper = {};
    std::vector< std::vector<float>* > oTreeSTLVecotr_Float_keeper = {};



    bool skipBadVector = false;
    bool ifile_status = true;
    std::string objectLocation = "default";

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

    TFile *get_ofile(){ return this->oFile; }
    std::string get_ofile_name(){ return this->ofileName; }

    int get_newBranchCounterKeeper(){return this->newBranchCounterKeeper;}
    void set_newBranchCounterKeeper(int value){this->newBranchCounterKeeper = value;}

    virtual bool setBranch( std::string typeName, std::string key, std::string branchName );

    template <typename type>
    bool buildBranch( std::string branchName );

    template <typename dtype>
    bool buildPrimitiveBranch( std::string branchName, int vector = 0 );

    template <typename dtype>
    typename DataType<dtype>::type *get_oTree_PrimitiveBranch(std::string branchName);

    template <typename dataType>
    dataType *get(std::string key, std::string dtype);

    std::vector<double> *getDV(std::string key );
    std::vector<int> *getIV(std::string key );
    double *getD(std::string key);
    int *getI(std::string key);

    template <typename o_type>
    void copyTTreeReaderArrayToVector( std::string oBranchName, std::string iBranchName, int entry);

    template <class branchType>
    typename DataType<branchType>::type *get_oTreeBranch( std::string branchName );

    std::vector<
      std::tuple<
        int, std::string, std::string, std::string
      >
    > branchConfigReader( std::string configName );

};

template <class dataType>
bool makeBranch(
  TTree *tree,
  std::string key,
  std::string branchName,
  std::map<std::string, dataType *> *vecDoubleBranch,
  dataType *&vecDouble,
  int &counter,
  std::map<std::string, int> *mapIndex
  )
  {
    //tree->Branch( Form("%s",branchName.c_str()), vecDouble) ;
    vecDouble = new dataType;
    vecDoubleBranch->insert( std::pair<std::string, dataType * >( key, vecDouble) );
    //vecDoubleBranch->insert( std::pair<std::string, dataType * >( key, temp) );
    tree->Branch( Form("%s",branchName.c_str()), vecDouble) ;
    mapIndex->insert( std::pair<std::string, int>(key, counter) );
    counter++;
    return true;
  };

template <class dataType>
bool makeBranch(
  TTree *tree,
  char *key_ctype,
  char *branchName_ctype,
  std::map<std::string, dataType *> *vecDoubleBranch,
  dataType *&vecDouble,
  int &counter,
  std::map<std::string, int> *mapIndex,
  int &counterKeeper
  )
  {
    std::string key = key_ctype;
    std::string branchName = branchName_ctype;
    //tree->Branch( Form("%s",branchName.c_str()), vecDouble) ;
    vecDouble = new dataType;
    vecDoubleBranch->insert( std::pair<std::string, dataType * >( key, vecDouble) );
    //vecDoubleBranch->insert( std::pair<std::string, dataType * >( key, temp) );
    tree->Branch( Form("%s",branchName.c_str()), vecDouble) ;
    mapIndex->insert( std::pair<std::string,int>(key, counter) );
    counter++;
    counterKeeper = counter;
    return true;
  };

template <class dataType>
bool readBranch(
  TTreeReader *iTreeReader,
  std::string key,
  std::string branchName,
  std::map<std::string, dataType *> *vecDoubleBranch,
  dataType *& vecDouble,
  int &counter,
  std::map<std::string, int> *mapIndex
  )
  {
    vecDouble = new dataType( *iTreeReader, branchName.c_str());
    vecDoubleBranch->insert( std::pair<std::string, dataType * >( key, vecDouble) );
    mapIndex->insert( std::pair<std::string,int>(key, counter) );
    counter++;
    return true;
  };

/*
template <class dataType>
bool readBranch(
  TTreeReader *iTreeReader,
  char *key_ctype,
  char *branchName_ctype,
  std::map<std::string, dataType *> *vecDoubleBranch,
  dataType *vecDouble,
  int &counter,
  std::map<std::string, int> *mapIndex
  )
  {
    std::string key = key_ctype;
    std::string branchName = branchName_ctype;
    vecDouble = new dataType( *iTreeReader, branchName.c_str());
    vecDoubleBranch->insert( std::pair<std::string, dataType * >( key, vecDouble) );
    mapIndex->insert( std::pair<std::string,int>(key, counter) );
    counter++;
    return true;
  };
*/

template <typename dtype>
bool BetaScope::buildPrimitiveBranch( std::string branchName, int ISvector)
{
  try{
    this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter] = new PrimitiveDataType_Container<dtype>();
    this->oTreePrimitiveBranchesMap.insert( std::pair<std::string, PrimitiveDataType_BaseContainer*>( branchName, this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter] ) );
    oTree->Branch( branchName.c_str(), static_cast<PrimitiveDataType_Container<dtype>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
    this->oTreePrimitiveBranchesMapIndex.insert( std::pair<std::string , int>(branchName, this->oTreePrimitiveBranchCounter) );

    if( is_vector<dtype>::value )
    {
      this->oTreeSTLVectorReservedIndex.push_back( this->newBranchCounterKeeper );

      typedef typename is_vector<dtype>::T v;
      ///*
      if( std::is_same< v, std::vector<int>>::value )
      {
        std::cout<< " Branch: " << branchName << " is std::vector<int>. Handle buffer internally\n";
        this->oTreeSTLVecotr_Int_keeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<int>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
      }
      if( std::is_same< v, std::vector<double>>::value )
      {
        std::cout<< " Branch: " << branchName << " is std::vector<double>. Handle buffer internally\n";
        this->oTreeSTLVecotr_Double_keeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<double>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
      }
      if( std::is_same< v, std::vector<float>>::value)
      {
        std::cout<< " Branch: " << branchName << " is std::vector<float>. Handle buffer internally\n";
        this->oTreeSTLVecotr_Float_keeper.push_back( static_cast<PrimitiveDataType_Container<std::vector<float>>*>(this->oTreePrimitiveBranches[this->oTreePrimitiveBranchCounter])->get() );
      }
      //*/
    }
    else{
      std::cout<< " Branch: " << branchName << " is not vector. No action needed.\n";
    }

    this->oTreePrimitiveBranchCounter++;
    return true;
  }
  catch(...){
    return false;
  }
}

template <typename dtype>
typename DataType<dtype>::type *BetaScope::get_oTree_PrimitiveBranch( std::string branchName )
{
  return static_cast<PrimitiveDataType_Container<dtype>*>(this->oTreePrimitiveBranchesMap[branchName])->get();
}


#endif // BETACOPE_H
