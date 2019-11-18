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

  protected:
    beta_scope_type beta_scope;

  public:

    BetaScope_AnaFramework(){};
    virtual ~BetaScope_AnaFramework(){};

    virtual void initialize( std::string addBranches="BetaScope_Driver/src/additionalBranches.ini", std::string rawBranches="" );
    virtual void analysis(){  ColorCout::Msg("BetaScope_AnaFramework::analysis",  " this is a virtual analysis()." ); };
    virtual void loopEvents( void (BetaScope_AnaFramework::*func)() );
    virtual void finalize( );
    virtual void filldata( );

    virtual void run()
    {
      BetaScope_AnaFramework::initialize();
      BetaScope_AnaFramework::loopEvents( &BetaScope_AnaFramework::analysis );
      BetaScope_AnaFramework::finalize();
    };

};


template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::initialize( std::string addBranches, std::string rawBranches )
{
  this->beta_scope.rawTreeReader();
  if( addBranches.compare("")!=0 )this->beta_scope.newTreeMaker( addBranches );
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::loopEvents( void (BetaScope_AnaFramework::*func)() )
{
  std::string function_name = "BetaScope_AnaFramework::loopEvents";
  ColorCout::Msg(function_name,  " is used for driving event looping." );
  while( this->beta_scope.get_treeReader()->Next() )
  {
    (this->*func)();
    BetaScope_AnaFramework<beta_scope_type>::filldata();
  }
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::filldata()
{
  this->event_counter++;
  this->beta_scope.fillEvent();
  if(this->event_counter%1000==0 || (this->event_counter%10==0 && this->event_counter <= 100) )
  {
    ColorCout::print( "   " + this->beta_scope.get_ifile_nickName(), " Proccessed events: "+std::to_string(this->event_counter)+" /" + std::to_string(this->beta_scope.get_iNumEvent()), BOLDYELLOW);
  }
}

template <typename beta_scope_type>
void BetaScope_AnaFramework<beta_scope_type>::finalize()
{
  this->beta_scope.fileIO_Close();
}


#endif // BETASCOPE_ANAFRAMEWORK_H
