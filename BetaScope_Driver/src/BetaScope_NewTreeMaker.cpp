#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "Colorful_Cout/include/Colorful_Cout.h"

#include <iostream>
#include <string>
#include <utility> // std::pair, std::make_pair

#include <TThread.h> //multi-thread from ROOT.
#include <TTree.h>

/*
making new ttree.
*/
bool BetaScope::NewTreeMaker(std::string additional_branch_list) {
  // TThread::Lock();

  std::string cout_prefix = "BetaScope::NewTreeMaker => ";
  ColorCout::print(cout_prefix, "Entering", BOLDGREEN);
  ColorCout::print(cout_prefix, "Preparing new tree. Default name: wfm",
                   YELLOW);

  int branch_counter = 0;

  this->output_ttree_ = new TTree("wfm", "BetaScope waveform ana.");
  this->output_ttree_->SetDirectory(this->output_tfile_);

  ColorCout::print(cout_prefix, "Creating default branches", YELLOW);

  bool branch_checker;

  /*
  ColorCout::print(cout_prefix, "Scope channels from input file: ", YELLOW);
  for( auto ch : this->channel )
  {
    ColorCout::print("  CH:", std::to_string(ch), CYAN);

    branch_checker = makeBranch<std::vector<double>>(this->output_ttree_,
  Form("w%i", ch ), Form("w%i", ch ), &this->output_ttree_VecDoubleMap,
  this->output_ttree_VecDouble[branch_counter], branch_counter,
  &this->output_ttree_VecDoubleMapIndex, this->newBranchCounterKeeper );
    this->output_ttree_VecDouble[branch_counter-1]->reserve(1000000);

    branch_checker = makeBranch<std::vector<double>>(this->output_ttree_,
  Form("t%i", ch ), Form("t%i", ch ), &this->output_ttree_VecDoubleMap,
  this->output_ttree_VecDouble[branch_counter], branch_counter,
  &this->output_ttree_VecDoubleMapIndex, this->newBranchCounterKeeper );
    this->output_ttree_VecDouble[branch_counter-1]->reserve(1000000);

    if(branch_checker)
    {
      ColorCout::print("  Successful:", std::to_string(ch), CYAN);
    }
  }
  */

  if (VERBOSITY != 0)
    this->output_ttree_->Print();

  ColorCout::print(cout_prefix, "Creating custom branches", YELLOW);
  if (additional_branch_list.compare("") != 0)
  {


    auto additonal_branches = BetaScope::BranchConfigReader(additional_branch_list);

    for (auto br : additonal_branches)
    {
      if (std::get<0>(br) == 0)
      {
        ColorCout::print("  > ", "mode 0, create for all channels", YELLOW);
        for (auto ch : this->channel)
        {
          std::string data_type = std::get<1>(br);

          if (data_type.compare("VD") == 0)
          {
            branch_checker = BetaScope::BuildOutBranch<std::vector<double>>( Form("%s%i", std::get<2>(br).c_str(), ch));

            if (branch_checker)
              ColorCout::print("  Successful type VD: ", std::get<2>(br) + std::to_string(ch), CYAN);
          }
          else if (data_type.compare("D") == 0)
          {
            branch_checker = BetaScope::BuildOutBranch<double>( Form("%s%i", std::get<2>(br).c_str(), ch));

            if (branch_checker)
              ColorCout::print("  Successful: type D ", std::get<2>(br) + std::to_string(ch), CYAN);
          }
          else
          {
            ColorCout::print("  Fail: type ? ", std::get<2>(br) + std::to_string(ch), RED);
          }
        }
      }
      else
      {
        ColorCout::print("  > ", "mode !0, create single branch", YELLOW);
        std::string data_type = std::get<1>(br);
        if (data_type.compare("VD") == 0)
        {
          branch_checker = BetaScope::BuildOutBranch<std::vector<double>>( Form("%s", std::get<2>(br).c_str()));

          if (branch_checker)
            ColorCout::print("  Successful type VD: ", std::get<2>(br), CYAN);
        }
        else if (data_type.compare("D") == 0)
        {
          branch_checker = BetaScope::BuildOutBranch<double>(Form("%s", std::get<2>(br).c_str()));

          if (branch_checker)
            ColorCout::print("  Successful: type D ", std::get<2>(br), CYAN);
        }
        else
        {
          ColorCout::print("  Fail: type ? ", std::get<2>(br), RED);
        }
      }
    }

    if (VERBOSITY != 0)
      this->output_ttree_->Print();

    ColorCout::print(cout_prefix, "Fininished, exiting", BOLDGREEN);
  }

  // TThread::UnLock();

  return true;
}
