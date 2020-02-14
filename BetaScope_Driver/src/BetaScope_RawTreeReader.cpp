#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "Colorful_Cout/include/Colorful_Cout.h"

#include <iostream>
#include <string>

#include <TThread.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

bool BetaScope::RawTreeReader(const char *itreeName) {
  std::string cout_prefix = "BetaScope::RawTreeReader => ";
  ColorCout::print(cout_prefix, "Entering", BOLDGREEN);
  ColorCout::print(cout_prefix, "Preparing raw tree reader.", YELLOW);

  this->input_tree_reader_ = new TTreeReader(itreeName, this->input_tfile_);
  this->input_num_event_ = this->input_tree_reader_->GetEntries(true);

  ColorCout::print( cout_prefix, "Number of events: " + std::to_string(this->input_num_event_), YELLOW);

  ColorCout::print(cout_prefix, "Looping through raw scope channels.", YELLOW);

  // int branch_counter = 0;
  bool br_check;

  for (int b = 1, max = 5; b < max; b++) {
    bool isBranch = BetaScope::IsBranchExists(Form("t%i", b));
    if (isBranch)
    {
      br_check = BetaScope::SetInBranch<TTreeReaderArray, double>( Form("w%i", b), Form("w%i", b));
      br_check = BetaScope::SetInBranch<TTreeReaderArray, double>( Form("t%i", b), Form("t%i", b));
      this->channel.push_back(b);
    }
  }

  ColorCout::print(cout_prefix, "Finished, exiting", BOLDGREEN);

  return true;
}
