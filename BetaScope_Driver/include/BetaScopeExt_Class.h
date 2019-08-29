///////////////////////////////
//                           //
// Argument class header     //
//                           //
///////////////////////////////

#ifndef BETACOPE_EXT_H
#define BETACOPE_EXT_H


//struct TH1_BaseContainer;
//template <typename th1_type> struct TH1_Container;

#include "BetaScope_Class.h"
#include <TH1.h>
#include <TH2.h>

/*==============================================================================
TH Base container for TH1 and TH2 histograms from ROOT.
==============================================================================*/
struct TH_BaseContainer
{
  TH_BaseContainer(){};
  virtual ~TH_BaseContainer(){};
};

template <typename TH_Type>
struct TH_Container : public TH_BaseContainer
{
  private:
    TH_Type *th = new TH_Type;
  public:
    TH_Container(){};
    virtual ~TH_Container(){};
    TH_Type *get(){return this->th;};
};

/*==============================================================================

==============================================================================*/

class BetaScope_Ext : public BetaScope
{
  std::string class_name = "BetaScope_Ext";

  int newTHBranchCounter = 0;

  public:
    //TH1 for input
    TTreeReaderArray<TH1I> *iTree_TH1I_Array[numCh] = {};
    TTreeReaderArray<TH1F> *iTree_TH1F_Array[numCh] = {};
    TTreeReaderArray<TH1D> *iTree_TH1D_Array[numCh] = {};

    std::map<std::string, TTreeReaderArray<TH1I>*> iTree_TH1I_Array_Map;
    std::map<std::string, TTreeReaderArray<TH1F>*> iTree_TH1F_Array_Map;
    std::map<std::string, TTreeReaderArray<TH1D>*> iTree_TH1D_Array_Map;

    TTreeReaderValue<TH1I> *iTree_TH1I[numCh] = {};
    TTreeReaderValue<TH1F> *iTree_TH1F[numCh] = {};
    TTreeReaderValue<TH1D> *iTree_TH1D[numCh] = {};

    std::map<std::string, TTreeReaderValue<TH1I>*> iTree_TH1I_Map;
    std::map<std::string, TTreeReaderValue<TH1F>*> iTree_TH1F_Map;
    std::map<std::string, TTreeReaderValue<TH1D>*> iTree_TH1D_Map;

    //TH1 for output
    std::vector<TH1I> *oTree_TH1I_Array[numCh] = {};
    std::vector<TH1F> *oTree_TH1F_Array[numCh] = {};
    std::vector<TH1D> *oTree_TH1D_Array[numCh] = {};

    std::map<std::string, std::vector<TH1I>*> oTree_TH1I_Array_Map;
    std::map<std::string, std::vector<TH1F>*> oTree_TH1F_Array_Map;
    std::map<std::string, std::vector<TH1D>*> oTree_TH1D_Array_Map;

    TH1I *oTree_TH1I[numCh] = {};
    TH1F *oTree_TH1F[numCh] = {};
    TH1D *oTree_TH1D[numCh] = {};

    TH_BaseContainer *oTree_TH[numCh] = {};
    std::map<std::string, TH_BaseContainer* > oTree_TH_Map;

    std::map<std::string, TH1I* > oTree_TH1I_Map;
    std::map<std::string, TH1F* > oTree_TH1F_Map;
    std::map<std::string, TH1D* > oTree_TH1D_Map;

    BetaScope_Ext(){ std::cout << this->class_name << std::endl; };
    ~BetaScope_Ext()
    {
      ColorCout::Msg(this->class_name, "call destructor at");
    };

    void fillEvent();

    template <typename dtype>
    bool buildBranch(std::string branchName );

    template <typename TH1_Type>
    bool buildTH1Branch(std::string branchName );

    bool setBranch( std::string typeName, std::string key, std::string branchName);

    template <typename TH1_Type>
    typename DataType<TH1_Type>::type *get_oHisto1D( std::string branchName );

    template <typename th1_type>
    int fill_oHisto1D( double value );

    //void test();
    //oTree_TH1[0] = new TH1_Container<TH1D>;
};



#endif // BETACOPE_EXT_H
