#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "Colorful_Cout/include/Colorful_Cout.h"

#include <string>
#include <iostream>
#include <utility>      // std::pair, std::make_pair

#include <TTree.h>
#include <TThread.h>    //multi-thread from ROOT.

/*
making new ttree.
*/
bool BetaScope::newTreeMaker( std::string additional_branch_list )
{
  //TThread::Lock();

  std::string coutPrefix = "BetaScope::newTreeMaker => ";
  ColorCout::print(coutPrefix, "Entering", BOLDGREEN);
  ColorCout::print(coutPrefix, "Preparing new tree. Default name: wfm", YELLOW);

  int branch_counter = 0;

  this->oTree = new TTree( "wfm", "BetaScope waveform ana.");
  this->oTree->SetDirectory(this->oFile);

  ColorCout::print(coutPrefix, "Creating default branches", YELLOW);

  bool branch_checker;

  /*
  ColorCout::print(coutPrefix, "Scope channels from input file: ", YELLOW);
  for( auto ch : this->channel )
  {
    ColorCout::print("  CH:", std::to_string(ch), CYAN);

    branch_checker = makeBranch<std::vector<double>>(this->oTree, Form("w%i", ch ), Form("w%i", ch ), &this->oTreeVecDoubleMap, this->oTreeVecDouble[branch_counter], branch_counter, &this->oTreeVecDoubleMapIndex, this->newBranchCounterKeeper );
    this->oTreeVecDouble[branch_counter-1]->reserve(1000000);

    branch_checker = makeBranch<std::vector<double>>(this->oTree, Form("t%i", ch ), Form("t%i", ch ), &this->oTreeVecDoubleMap, this->oTreeVecDouble[branch_counter], branch_counter, &this->oTreeVecDoubleMapIndex, this->newBranchCounterKeeper );
    this->oTreeVecDouble[branch_counter-1]->reserve(1000000);

    if(branch_checker)
    {
      ColorCout::print("  Successful:", std::to_string(ch), CYAN);
    }
  }
  */

  if(VERBOSITY!=0)this->oTree->Print();

  ColorCout::print(coutPrefix, "Creating custom branches", YELLOW);
  auto additonal_branches = BetaScope::branchConfigReader( additional_branch_list );

  for( auto br : additonal_branches )
  {
    if( std::get<0>(br) == 0)
    {
      ColorCout::print("  > ", "mode 0, create for all channels", YELLOW);
      for( auto ch : this->channel )
      {
        std::string data_type = std::get<1>(br);

        if( data_type.compare("VD")==0 )
        {
          branch_checker = BetaScope::buildPrimitiveBranch<std::vector<double>>( Form("%s%i", std::get<2>(br).c_str(), ch) );

          if(branch_checker)ColorCout::print("  Successful type VD: ", std::get<2>(br)+std::to_string(ch), CYAN);
        }
        else if( data_type.compare("D")==0 )
        {
          branch_checker = BetaScope::buildPrimitiveBranch<double>( Form("%s%i", std::get<2>(br).c_str(), ch) );

          if(branch_checker)ColorCout::print("  Successful: type D ", std::get<2>(br)+std::to_string(ch), CYAN);
        }
        else
        {
          ColorCout::print("  Fail: type ? ", std::get<2>(br)+std::to_string(ch), RED);
        }
      }
    }
    else
    {
      ColorCout::print("  > ", "mode !0, create single branch", YELLOW);
      std::string data_type = std::get<1>(br);
      if( data_type.compare("VD")==0 )
      {
        branch_checker = BetaScope::buildPrimitiveBranch<std::vector<double>>( Form("%s", std::get<2>(br).c_str()) );

        if(branch_checker)ColorCout::print("  Successful type VD: ", std::get<2>(br), CYAN);
      }
      else if( data_type.compare("D")==0 )
      {
        branch_checker = BetaScope::buildPrimitiveBranch<double>( Form("%s", std::get<2>(br).c_str()) );

        if(branch_checker)ColorCout::print("  Successful: type D ", std::get<2>(br), CYAN);
      }
      else
      {
        ColorCout::print("  Fail: type ? ", std::get<2>(br), RED);
      }
    }
  }

  if(VERBOSITY!=0)this->oTree->Print();

  ColorCout::print(coutPrefix, "Fininished, exiting", BOLDGREEN);

  //TThread::UnLock();

  return true;
}
