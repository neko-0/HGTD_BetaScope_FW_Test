#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "Colorful_Cout/include/Colorful_Cout.h"

#include <string>
#include <iostream>

#include <TFile.h>
#include <TThread.h>

bool BetaScope::FileOpen( const char *ifile_path )
{
    //TThread::Lock();

    std::string cout_prefix = "BetaScope::FileOpen => ";
    ColorCout::print(cout_prefix, "Entering", BOLDGREEN);
    ColorCout::print(cout_prefix, "Preparing IO.", YELLOW);

    this->input_tfile_ = new TFile( ifile_path );
    if( this->input_tfile_->IsZombie() )
    {
        ColorCout::print(cout_prefix, "Zombie file. return false.", RED);
        return false;
    }
    ColorCout::print(cout_prefix, "Input file looks fine. continue", YELLOW);

    std::string delimiter = "/";
    std::string ofile_name = ifile_path;
    while( int(ofile_name.find( delimiter )) != -1 )
    {
        ofile_name.erase(0, ofile_name.find( delimiter ) + delimiter.length() );
    }
    this->output_file_name_ = this->output_file_prefix_ += ofile_name;
    this->input_file_name_ = ifile_path;
    this->input_file_nick_name_ = this->output_file_name_;

    ColorCout::print(cout_prefix, "Create output file", YELLOW);
    ColorCout::print(cout_prefix, "compressionLevel: "+std::to_string(this->compression_level_), YELLOW);

    this->output_tfile_ = new TFile( this->output_file_name_.c_str(), "RECREATE", "", this->compression_level_ );
    this->output_tfile_->cd();

    ColorCout::print(cout_prefix, "Fininished, exiting", BOLDGREEN);
    //TThread::UnLock();

    return true;
}

//==============================================================================
//==============================================================================

void BetaScope::FileClose()
{
    //TThread::Lock();
    std::string cout_prefix = "BetaScope::FileClose => ";
    ColorCout::print(cout_prefix, "Entering", BOLDGREEN);
    ColorCout::print(cout_prefix, "Writing output files.", YELLOW);

    this->output_ttree_->Write();
    this->output_tfile_->Close();

    ColorCout::print(cout_prefix, "Clean up allocated memory", YELLOW);

    for( auto const& val : this->input_branches_buffer_ )
    {
        if(val) {
            delete val;
        }
    }

    ColorCout::print(cout_prefix, "Finished, extiting", BOLDGREEN);
    std::time_t _t_end_of_program = std::time(nullptr);
    std::time_t _t_end_of_program_cpu = std::clock();
    this->cpu_time = std::clock() - this->cpu_time;
    ColorCout::print( "  "+ this->input_file_nick_name_ +" Wall Time used: ", std::to_string(_t_end_of_program-this->kTimeObjCreation), BOLDYELLOW);
    ColorCout::print( "  "+ this->input_file_nick_name_ +" CPU Time used: ", std::to_string(_t_end_of_program_cpu-this->cpu_time), BOLDYELLOW);
    //TThread::UnLock();
}
