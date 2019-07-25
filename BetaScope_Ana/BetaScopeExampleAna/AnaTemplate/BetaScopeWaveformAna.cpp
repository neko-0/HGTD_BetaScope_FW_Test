#include "BetaScopeWaveformAna.h"

void BetaScopeWaveformAna::initialize( std::string ifile )
{
  this->beta_scope.fileIO_Open( ifile.c_str() );
  BetaScope_AnaFramework::initialize( this->beta_scope, "BetaScope_Ana/BetaScopeWaveformAna/AnaTemplate/myOwnTree.ini" );

  //do your own stuffs here


}

void BetaScopeWaveformAna::loopEvents()
{
  while( this->beta_scope.treeReader->Next() )
  {







    BetaScope_AnaFramework::filldata( this->beta_scope );
  }
}

void BetaScopeWaveformAna::finalize()
{
  //do your own stuffs here


  BetaScope_AnaFramework::finalize(this->beta_scope);
}
