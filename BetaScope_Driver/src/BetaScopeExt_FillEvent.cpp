#include "BetaScope_Driver/include/BetaScopeExt_Class.h"

//-------c++----------------//
#include <iostream>


void BetaScope_Ext::FillEvent()
{
    BetaScope::FillEvent();

    /*
    for(int i =0;i<100;i++)
    {
      if(this->oTree_TH1F[i]){int cap = this->oTree_TH1F[i]->size();this->oTree_TH1F[i]->clear();this->oTree_TH1F[i]->reserve(cap);}
      if(this->oTree_TH1D[i]){int cap = this->oTree_TH1D[i]->size();this->oTree_TH1D[i]->clear();this->oTree_TH1D[i]->reserve(cap);}
      if(this->oTree_TH1I[i]){int cap = this->oTree_TH1I[i]->size();this->oTree_TH1I[i]->clear();this->oTree_TH1I[i]->reserve(cap);}
    }
    */
    for( auto &i : this->oTree_TH1I_Array ){if(i)i->clear(); }
    for( auto &i : this->oTree_TH1F_Array ){if(i)i->clear(); }
    for( auto &i : this->oTree_TH1D_Array ){if(i)i->clear(); }
}
