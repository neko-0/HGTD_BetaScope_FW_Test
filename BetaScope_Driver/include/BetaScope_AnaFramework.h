#ifndef BETASCOPE_ANAFRAMEWORK_H
#define BETASCOPE_ANAFRAMEWORK_H

#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "BetaScope_Driver/include/BetaScopeExt_Class.h"
#include "Colorful_Cout/include/Colorful_Cout.h"

template <typename beta_scope_type>
class BetaScope_AnaFramework
{
  int event_counter = 0;

  public:
    beta_scope_type beta_scope;

    BetaScope_AnaFramework(){};
    ~BetaScope_AnaFramework(){};

    virtual void initialize( std::string addBranches="BetaScope_Driver/src/additionalBranches.ini", std::string rawBranches="" );
    virtual void loopEvents( );
    virtual void finalize( );
    void filldata( );

    void copyBranch( );
};


template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::initialize( std::string addBranches, std::string rawBranches )
{
  beta_scope.rawTreeReader();
  if( addBranches.compare("")!=0 )beta_scope.newTreeMaker( addBranches );
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::loopEvents( ){}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::filldata()
{
  this->event_counter++;
  beta_scope.fillEvent();
  if(this->event_counter%1000==0 || (this->event_counter%10==0 && this->event_counter <= 100) )
  {
    ColorCout::print( "   "+beta_scope.get_ifile_nickName(), " Proccessed events: "+std::to_string(this->event_counter)+" /" + std::to_string(beta_scope.get_iNumEvent()), BOLDYELLOW);
  }
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::finalize()
{
  beta_scope.fileIO_Close();
}

/*
template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::copyBranch()
{
  for( int b = 0, max_b = beta_scope.channel.size(); b < max_b; b++ )
  {
    for(int i = 0, maxPnt = beta_scope.iTreeDoubleArrayMap[Form("w%d",beta_scope.channel.at(b))]->GetSize() ; i < maxPnt; i++)
    {
      //std::cout<<itreeReaderBranch[Form("w%d",this->channel.at(b))]->At(i)<<std::endl;
      beta_scope.oTreeVecDoubleMap[Form("w%d",beta_scope.channel.at(b))]->push_back( beta_scope.iTreeDoubleArrayMap[Form("w%d",beta_scope.channel.at(b))]->At(i) * 1000.0 );
      //std::cout<<this->vecDoubleBranch[Form("w%d",this->channel.at(b))][i]<<std::endl;
      beta_scope.oTreeVecDoubleMap[Form("t%d",beta_scope.channel.at(b))]->push_back( beta_scope.iTreeDoubleArrayMap[Form("t%d",beta_scope.channel.at(b))]->At(i) * 1.0e12 );
    }
  }
}
*/

#endif // TEST_H
