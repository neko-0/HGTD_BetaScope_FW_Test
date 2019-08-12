#include "BetaScope_Driver/include/BetaScope_Class.h"

#include <string>
#include <iostream>
#include <utility>

/*
void BetaScope::_clearVecBuffer()
{
  for( auto const& [key, val] : this->oTreeVecDoubleMap )
  {
    this->oTreeVecDoubleMap[key]->clear();
  }

  for( auto const& [key, val] : this->oTreeVecIntMap )
  {
    this->oTreeVecIntMap[key]->clear();
  }
}
*/

void BetaScope::_clearVecBuffer()
{
  //for( auto & [v1, v2] : zip(this->oTreeVecDoubleMap, this->oTreeVecIntMap) )
  /*
  for(int i =0;i<100;i++)
  {
    //v1->clear();
    //v2->clear();
    //if(this->oTreeVecInt[i]){this->oTreeVecInt[i]->clear();this->oTreeVecInt[i]->reserve(5000);}
    //if(this->oTreeVecDouble[i]){this->oTreeVecDouble[i]->clear();this->oTreeVecDouble[i]->reserve(5000);}
    if(this->oTreeVecInt[i]){int cap = this->oTreeVecInt[i]->size();this->oTreeVecInt[i]->clear();this->oTreeVecInt[i]->reserve(cap);}
    if(this->oTreeVecDouble[i]){int cap = this->oTreeVecDouble[i]->size();this->oTreeVecDouble[i]->clear();this->oTreeVecDouble[i]->reserve(cap);}
  }
  */
  for( auto i : this->reserved_vec_d )
  {
    if(this->oTreeVecDouble[i]){int cap = this->oTreeVecDouble[i]->size();this->oTreeVecDouble[i]->clear();this->oTreeVecDouble[i]->reserve(cap);}
  }
  for( auto i : this->reserved_vec_i )
  {
    if(this->oTreeVecInt[i]){int cap = this->oTreeVecInt[i]->size();this->oTreeVecInt[i]->clear();this->oTreeVecInt[i]->reserve(cap);}
  }
}

void BetaScope::_clearVecBuffer(std::string mode){
  for(int i =0;i<100;i++)
  {
    //v1->clear();
    //v2->clear();
    //if(this->oTreeVecInt[i]){this->oTreeVecInt[i]->clear();this->oTreeVecInt[i]->reserve(5000);}
    //if(this->oTreeVecDouble[i]){this->oTreeVecDouble[i]->clear();this->oTreeVecDouble[i]->reserve(5000);}
    if(this->oTreeVecInt[i]){int cap = this->oTreeVecInt[i]->size();this->oTreeVecInt[i]->clear();this->oTreeVecInt[i]->reserve(cap);}
    if(this->oTreeVecDouble[i]){int cap = this->oTreeVecDouble[i]->size();this->oTreeVecDouble[i]->clear();this->oTreeVecDouble[i]->reserve(cap);}
  }
}
