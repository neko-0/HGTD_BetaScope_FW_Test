#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include <iostream>
#include <string>

#include <TFile.h>
#include <TThread.h>

bool BetaScope::FileOpen(const char *ifile_path) {
  // TThread::Lock();

  std::string cout_prefix = "BetaScope::FileOpen => ";
  logger.info(__PRETTY_FUNCTION__, "Entering" );
  logger.info(__PRETTY_FUNCTION__, "Preparing IO." );

  this->input_tfile_ = new TFile(ifile_path);
  if (this->input_tfile_->IsZombie()) {
    logger.error(__PRETTY_FUNCTION__, "Zombie file. return false.");
    return false;
  }
  logger.info(__PRETTY_FUNCTION__, "Input file looks fine. continue");

  std::string delimiter = "/";
  std::string ofile_name = ifile_path;
  while (int(ofile_name.find(delimiter)) != -1) {
    ofile_name.erase(0, ofile_name.find(delimiter) + delimiter.length());
  }
  this->output_file_name_ = this->output_file_prefix_ += ofile_name;
  this->input_file_name_ = ifile_path;
  this->input_file_nick_name_ = this->output_file_name_;

  logger.info(__PRETTY_FUNCTION__, "Create output file");
  logger.info(__PRETTY_FUNCTION__, "compressionLevel: " + std::to_string(this->compression_level_));

  this->output_tfile_ = new TFile(this->output_file_name_.c_str(), "RECREATE", "", this->compression_level_);
  this->output_tfile_->cd();

  logger.info(__PRETTY_FUNCTION__, "Fininished, exiting");
  // TThread::UnLock();

  return true;
}

//==============================================================================
//==============================================================================

void BetaScope::FileClose() {
  // TThread::Lock();
  std::string cout_prefix = "BetaScope::FileClose => ";
  logger.info(__PRETTY_FUNCTION__, "Entering");
  logger.info(__PRETTY_FUNCTION__, "Writing output files.");

  this->output_ttree_->Write();
  this->output_tfile_->Close();

  logger.info(__PRETTY_FUNCTION__, "Clean up allocated memory");

  int counter = 0;
  for(const auto &val : this->input_branches_buffer_)
  {
    if(val)
    {
      logger.info(__PRETTY_FUNCTION__, "Calling delete");
      delete val;
    }
    counter++;
    if(counter == this->input_branch_counter_){break;}
  }

  logger.info(__PRETTY_FUNCTION__, "Finished, extiting");
  std::time_t _t_end_of_program = std::time(nullptr);
  std::time_t _t_end_of_program_cpu = std::clock();
  this->cpu_time = std::clock() - this->cpu_time;
  logger.info(__PRETTY_FUNCTION__, this->input_file_nick_name_ + " Wall Time used: " + std::to_string(_t_end_of_program - this->kTimeObjCreation) );
  logger.info(__PRETTY_FUNCTION__, this->input_file_nick_name_ + " CPU Time used: " + std::to_string(_t_end_of_program_cpu - this->cpu_time) );
  // TThread::UnLock();
}
