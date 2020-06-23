#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include <iostream>
#include <string>

#include <TObject.h>
#include <TFile.h>
#include <TThread.h>

bool BetaScope::FileOpen(const char *ifile_path) {
  // TThread::Lock();

  std::string cout_prefix = "BetaScope::FileOpen => ";
  LOG_INFO("Entering" );
  LOG_INFO("Preparing IO." );

  this->input_tfile_ = TFile::Open(ifile_path);
  if (this->input_tfile_->IsZombie())
  {
    LOG_ERROR("Zombie file. return false.");
    this->is_file_opened_ = false;
    return false;
  }
  LOG_INFO("Input file looks fine. continue");

  std::string delimiter = "/";
  std::string ofile_name = ifile_path;
  while (int(ofile_name.find(delimiter)) != -1)
  {
    ofile_name.erase(0, ofile_name.find(delimiter) + delimiter.length());
  }
  this->output_file_name_ = this->output_file_prefix_ += ofile_name;
  this->input_file_name_ = ifile_path;
  this->input_file_nick_name_ = this->output_file_name_;

  LOG_INFO( "Create output file");
  LOG_INFO( "compressionLevel: " + std::to_string(this->compression_level_));

  this->output_tfile_ = TFile::Open(this->output_file_name_.c_str(), "RECREATE", this->output_file_name_.c_str(), this->compression_level_);
  //this->output_ttree_->SetDirectory(this->output_tfile_);

  LOG_INFO("Fininished, exiting");
  // TThread::UnLock();

  this->is_file_opened_ = true;
  return true;
}

//==============================================================================
//==============================================================================

void BetaScope::FileClose() {
  // TThread::Lock();
  std::string cout_prefix = "BetaScope::FileClose => ";
  LOG_INFO("Entering");
  LOG_INFO("Writing output files.");

  //this->output_ttree_->Write("", TObject::kOverwrite);
  this->output_ttree_->AutoSave();
  delete this->output_ttree_;
  this->output_tfile_->Close();
  this->input_tfile_->Close();

  LOG_INFO("file " + this->input_file_name_ + " is finished, extiting.");
  std::time_t _t_end_of_program = std::time(nullptr);
  std::time_t _t_end_of_program_cpu = std::clock();
  this->cpu_time = std::clock() - this->cpu_time;
  LOG_INFO(this->input_file_nick_name_ + " Wall Time used: " + std::to_string(_t_end_of_program - this->kTimeObjCreation) );
  LOG_INFO(this->input_file_nick_name_ + " CPU Time used: " + std::to_string(_t_end_of_program_cpu - this->cpu_time) );
  this->write_skim_ = true;
  // TThread::UnLock();
}
