#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "Colorful_Cout/include/Colorful_Cout.h"

#include <string>
#include <iostream>

#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include <TThread.h>

bool BetaScope::rawTreeReader( const char* itreeName )
{
  std::string coutPrefix = "BetaScope::rawTreeReader => ";
  ColorCout::print(coutPrefix, "Entering", BOLDGREEN);
  ColorCout::print(coutPrefix, "Preparing raw tree reader.", YELLOW);

  this->treeReader = new TTreeReader( itreeName, this->iFile );
  this->i_numEvent = this->treeReader->GetEntries(true);

  ColorCout::print(coutPrefix, "Number of events: "+std::to_string(this->i_numEvent), YELLOW);

  ColorCout::print(coutPrefix, "Looping through raw scope channels.", YELLOW);

  //int branch_counter = 0;
  bool br_check;

  for( int b = 1, max = 5; b < max; b++ )
  {
    bool isBranch = BetaScope::isBranchExists( Form("t%i",b) );
    if(isBranch){
      br_check = BetaScope::set_iBranch<TTreeReaderArray, double>( Form("w%i", b ),  Form("w%i", b ) );
      br_check = BetaScope::set_iBranch<TTreeReaderArray, double>( Form("t%i", b ),  Form("t%i", b ) );
      this->channel.push_back(b);
    }
  }

  ColorCout::print(coutPrefix, "Finished, exiting", BOLDGREEN);

  return true;
}
