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
//------ROOT----------------//
#include <TFile.h>
#include <TTree.h>
#include <TThread.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

#define VERBOSITY 0

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

    int newBranchCounterKeeper = 0;


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
    void fileIO_Close();

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

bool readBranch( BetaScope &beta_scope, std::string typeName, std::string key, std::string branchName )
{
  if( typeName.compare("TTreeReaderArray<double>") ==0 ){
    beta_scope.iTreeDoubleArray[beta_scope.iTreeBranchCounter] = new TTreeReaderArray<double>( *beta_scope.treeReader, branchName.c_str() );
    beta_scope.iTreeDoubleArrayMap.insert( std::pair<std::string, TTreeReaderArray<double> * >(key, beta_scope.iTreeDoubleArray[beta_scope.iTreeBranchCounter]) );
    beta_scope.iTreeDoubleArrayMapIndex.insert( std::pair<std::string, int>(key, beta_scope.iTreeBranchCounter) );
    beta_scope.iTreeBranchCounter+=1;
    return true;
  }
  else if( typeName.compare("TTreeReaderArray<int>") ==0 ){
    beta_scope.iTreeIntArray[beta_scope.iTreeBranchCounter] = new TTreeReaderArray<int>( *beta_scope.treeReader, branchName.c_str() );
    beta_scope.iTreeIntArrayMap.insert( std::pair<std::string, TTreeReaderArray<int>*>(key, beta_scope.iTreeIntArray[beta_scope.iTreeBranchCounter]) );
    beta_scope.iTreeIntArrayMapIndex.insert( std::pair<std::string, int>(key, beta_scope.iTreeBranchCounter) );
    beta_scope.iTreeBranchCounter+=1;
    return true;
  }
  else if( typeName.compare("TTreeReaderValue<double>") ==0 ){
    beta_scope.iTreeDoubleValue[beta_scope.iTreeBranchCounter] = new TTreeReaderValue<double>( *beta_scope.treeReader, branchName.c_str() );
    beta_scope.iTreeDoubleValueMap.insert( std::pair<std::string, TTreeReaderValue<double>*>(key, beta_scope.iTreeDoubleValue[beta_scope.iTreeBranchCounter]) );
    beta_scope.iTreeDoubleValueMapIndex.insert( std::pair<std::string, int>(key, beta_scope.iTreeBranchCounter) );
    beta_scope.iTreeBranchCounter+=1;
    return true;
  }
  else if( typeName.compare("TTreeReaderValue<int>") ==0 ){
    beta_scope.iTreeIntValue[beta_scope.iTreeBranchCounter] = new TTreeReaderValue<int>( *beta_scope.treeReader, branchName.c_str() );
    beta_scope.iTreeIntValueMap.insert( std::pair<std::string, TTreeReaderValue<int>*>(key, beta_scope.iTreeIntValue[beta_scope.iTreeBranchCounter]) );
    beta_scope.iTreeIntValueMapIndex.insert( std::pair<std::string, int>(key, beta_scope.iTreeBranchCounter) );
    beta_scope.iTreeBranchCounter+=1;
    return true;
  }
  else{
    return false;
  }
}
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
#endif // BETACOPE_H
