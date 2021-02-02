#include "BetaScope_Driver/include/BetaScope.h"

#include <iostream>
#include <map>
#include <string>

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

BetaScope::BetaScope(const std::string &ifile)
  : input_file_name_(ifile)
{
    bool is_open = this->FileOpen(this->input_file_name_);
    if(is_open)
    {
      LOG_INFO("After BetaScope Constructor");
    }
}

BetaScope::~BetaScope()
{
  LOG_INFO( boost::str(boost::format("%1% call destructor at location %2%")%this%this->object_location_ ) );
  LOG_INFO("Clean up allocated memory");
  this->ClearVecBuffer();
  int i_count = 0;
  int o_count = 0;
  for(auto &val : this->input_branch_buffer_){ delete val; val = nullptr; i_count++; }
  for(auto &val : this->output_branch_buffer_){ delete val; val = nullptr; o_count++; }
  delete this->input_tree_reader_;
  this->input_tree_reader_ = nullptr;
}

bool BetaScope::FileOpen(const std::string &ifile_path)
{

  std::string cout_prefix = "BetaScope::FileOpen => ";
  LOG_INFO("Preparing input file." );

  this->input_tfile_ = TFile::Open(ifile_path.c_str());
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

  this->is_file_opened_ = true;
  return true;
}

//==============================================================================
//==============================================================================

void BetaScope::FileClose()
{
  std::string cout_prefix = "BetaScope::FileClose => ";
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
}

bool BetaScope::SetTreeReader()
{
  std::string cout_prefix = "BetaScope::RawTreeReader => ";
  LOG_INFO("Preparing TTree Reader from " + this->input_tree_name_);

  this->input_tree_reader_ = new TTreeReader(this->input_tree_name_.c_str(), this->input_tfile_);
  this->input_num_event_ = this->input_tree_reader_->GetEntries(true);

  LOG_INFO("Found number of events: " + std::to_string(this->input_num_event_));
  LOG_INFO("Attempt looping through raw scope channels.");
  for( int b = 1, max = 5; b < max; b++)
  {
    if(BetaScope::IsBranchExists(Form("t%i", b)))
    {
      BetaScope::SetInBranch<TTreeReaderArray<double>>( Form("w%i", b), Form("w%i", b));
      BetaScope::SetInBranch<TTreeReaderArray<double>>( Form("t%i", b), Form("t%i", b));
      this->channel.push_back(b);
    }
  }

  LOG_INFO("Finished, exiting");

  return true;
}

bool BetaScope::BuildOutTree(const std::string &additional_branch_list)
{

  std::string cout_prefix = "BetaScope::NewTreeMaker => ";
  LOG_INFO("Preparing new tree: " + this->output_tree_name_);

  this->output_ttree_ = new TTree("wfm", this->input_file_nick_name_.c_str());
  this->output_ttree_->SetDirectory(this->output_tfile_);

  bool branch_checker = false;

  if(VERBOSITY != 0)
  {
      this->output_ttree_->Print();
  }

  if(additional_branch_list.compare("") != 0)
  {
    LOG_INFO("Found provided custom branches list" );

    auto additonal_branches = BetaScope::BranchConfigReader(additional_branch_list);

    for(auto br : additonal_branches)
    {
      if(br.mode == 0)
      {
        LOG_INFO("mode 0, create for all channels");
        for(auto ch : this->channel)
        {
          std::string data_type = br.type;

          if(data_type.compare("VD") == 0)
          {
            branch_checker = BetaScope::BuildOutBranch<std::vector<double>>( Form("%s%i", br.key.c_str(), ch));
            if(branch_checker)
            {
                LOG_INFO("Successful type VD: " + br.key + std::to_string(ch) );
            }
          }
          else if(data_type.compare("D") == 0)
          {
            branch_checker = BetaScope::BuildOutBranch<double>( Form("%s%i", br.key.c_str(), ch));
            if(branch_checker)
            {
                LOG_WARNING("Successful: type D " + br.key + std::to_string(ch) );
            }
          }
          else
          {
            LOG_WARNING( "  Fail: type ? " + br.key + std::to_string(ch) );
          }
        }
      }
      else
      {
        LOG_INFO( " mode !0, create single branch");
        std::string data_type = br.type;
        if(data_type.compare("VD") == 0)
        {
          branch_checker = BetaScope::BuildOutBranch<std::vector<double>>( Form("%s", br.key.c_str()));

          if (branch_checker)
            LOG_INFO( "  Successful type VD: " + br.key );
        }
        else if (data_type.compare("D") == 0)
        {
          branch_checker = BetaScope::BuildOutBranch<double>(Form("%s", br.key.c_str()));

          if (branch_checker)
            LOG_INFO( " Successful: type D " + br.key);
        }
        else
        {
          LOG_INFO( "  Fail: type ? "+ br.key);
        }
      }
    }

    if(VERBOSITY != 0)
    {
        this->output_ttree_->Print();
    }

    LOG_INFO("Fininished, exiting" );
  }

  return true;
}


bool BetaScope::IsBranchExists(const std::string &branchName)
{
  auto branch_checker = ((TTree *)this->input_tfile_->Get(this->input_tree_name_.c_str()))->GetListOfBranches()->FindObject(branchName.c_str());
  if(branch_checker != NULL)
  {
      return true;
  }
  else
  {
      return false;
  }
}

void BetaScope::ClearVecBuffer() {
  for(auto &val : this->output_vector_keeper_)
  {
    val->Clear();
  }
}

bool BetaScope::NextEvent()
{
  return this->input_tree_reader_->Next();
}

void BetaScope::FillEvent()
{
  this->output_ttree_->Fill();
}

TFile *BetaScope::GetInFile() const
{
  return this->input_tfile_;
}

std::string BetaScope::GetInFileName() const
{
  return this->input_file_name_;
}

std::string BetaScope::GetInFileNickName() const
{
  return this->input_file_nick_name_;
}

std::string BetaScope::GetInTreeName() const
{
  return this->input_tree_name_;
}

int BetaScope::GetInNumEvent() const
{
  return this->input_num_event_;
}

TFile *BetaScope::GetOutFile() const
{
  return this->output_tfile_;
}

TTree *BetaScope::GetOutTree() const
{
  return this->output_ttree_;
}

std::string BetaScope::GetOutFilePrefix() const
{
  return this->output_file_prefix_;
}

std::string BetaScope::GetOutFileName() const
{
  return this->output_file_name_;
}

int BetaScope::GetCompLevel() const
{
  return this->compression_level_;
}

void BetaScope::SetInFileName(const std::string &value)
{
  this->input_file_name_ = value;
}

void BetaScope::SetInFileNickName(const std::string &value)
{
  this->input_file_nick_name_ = value;
}

void BetaScope::SetInTreeName(const std::string &value)
{
  this->input_tree_name_ = value;
}

void BetaScope::SetOutFilePrefix(const std::string &value)
{
  this->output_file_prefix_ = value;
}

void BetaScope::SetOutFileName(const std::string &value)
{
  this->output_file_name_ = value;
}

void BetaScope::SetCompLevel(const int &value)
{
  this->compression_level_ = value;
}

std::vector<BranchConfig> BetaScope::BranchConfigReader(const std::string &configName)
{
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini(configName, pt);

  std::vector<BranchConfig> output;

  for(auto section : pt)
  {
    output.push_back(
        BranchConfig{
          pt.get<int>(section.first + ".mode"),
          pt.get<std::string>(section.first + ".type"),
          pt.get<std::string>(section.first + ".key"),
          pt.get<std::string>(section.first + ".branch_name")
        }
      );
  }

  return output;
}
