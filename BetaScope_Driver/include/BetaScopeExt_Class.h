///////////////////////////////
//                           //
// Argument class header     //
//                           //
///////////////////////////////

#ifndef BETACOPE_EXT_H
#define BETACOPE_EXT_H


struct TH1_BaseContainer;
template <typename th1_type> struct TH1_Container;

#include "BetaScope_Class.h"
#include <TH1.h>

struct TH1_BaseContainer
{
  TH1_BaseContainer(){};
  virtual ~TH1_BaseContainer(){};

  //template <typename th1_type>
  //virtual void get(){std::cout<<"hi\n";};

  //void set(){};

  //virtual void get(){std::cout<<"get() of TH1_BaseContainer"<<std::endl;};
};

template <typename th1_type>
struct TH1_Container : public TH1_BaseContainer
{
  private:
    th1_type *th1 = new th1_type;
  public:

    TH1_Container(){};
    virtual ~TH1_Container(){};
    th1_type *get(){return this->th1;};

    /*
    void initialize( std::string histoName ){
      this->th1 = new th1_type(histoName.c_str(), "", 100, 1, 1);
    }
    */
};



class BetaScope_Ext : public BetaScope
{
  std::string class_name = "BetaScope_Ext";

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

    TH1_BaseContainer *oTree_TH1[numCh] = {};
    std::map<std::string, TH1_BaseContainer* > oTree_TH1_Map;


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

    template <typename th1_type>
    bool buildTH1Branch(std::string branchName );

    bool setBranch( std::string typeName, std::string key, std::string branchName);

    template <typename th1_type>
    typename DataType<th1_type>::type *get_oHisto1D( std::string branchName );

    template <typename th1_type>
    int fill_oHisto1D( double value );

    //void test();
    //oTree_TH1[0] = new TH1_Container<TH1D>;
};

template <typename th1_type>
bool BetaScope_Ext::buildTH1Branch( std::string branchName )
{
  try{
    this->oTree_TH1[newBranchCounterKeeper] = new TH1_Container<th1_type>();
    //th1_type *my_new_th1 = new th1_type;
    th1_type *my_th1 = static_cast<TH1_Container<th1_type>*>(this->oTree_TH1[newBranchCounterKeeper])->get();
    //my_th1 = my_new_th1;
    this->oTree_TH1_Map.insert( std::pair<std::string, TH1_BaseContainer* >(branchName, this->oTree_TH1[newBranchCounterKeeper]) );
    //std::cout<<"setting branch\n";
    //std::cout<< my_th1 << std::endl;
    //std::cout<< static_cast<TH1_Container<th1_type>*>(this->oTree_TH1[newBranchCounterKeeper])->get() << std::endl;
    //std::cout<<"setting branch2\n";
    oTree->Branch( branchName.c_str(), my_th1 );
    //this->oTreeVecIntMapIndex.insert( std::pair<std::string , int>(branchName, newBranchCounterKeeper) );
    //this->reserved_vec_i.push_back( this->newBranchCounterKeeper );
    newBranchCounterKeeper++;
    return true;
  }
  catch(...){
    //std::cout<<"ss branch2\n";
    return false;
  }
}

template <typename th1_type>
typename DataType<th1_type>::type *BetaScope_Ext::get_oHisto1D( std::string branchName )
{
  return static_cast<TH1_Container<th1_type>*>(this->oTree_TH1_Map[branchName])->get();
}


#endif // BETACOPE_EXT_H
