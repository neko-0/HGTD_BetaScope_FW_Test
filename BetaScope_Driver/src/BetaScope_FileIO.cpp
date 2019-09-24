#include "BetaScope_Driver/include/BetaScope_Class.h"
#include "BetaScope_Driver/include/BetaScope_Templates.h"

#include "Colorful_Cout/include/Colorful_Cout.h"

#include <string>
#include <iostream>

#include <TFile.h>
#include <TThread.h>

bool BetaScope::fileIO_Open( const char *ifile_path )
{
  //TThread::Lock();

  std::string coutPrefix = "BetaScope::fileIO_Open => ";
  ColorCout::print(coutPrefix, "Entering", BOLDGREEN);
  ColorCout::print(coutPrefix, "Preparing IO.", YELLOW);

  this->iFile = new TFile( ifile_path );
  if( this->iFile->IsZombie() )
  {
    ColorCout::print(coutPrefix, "Zombie file. return false.", RED);
    return false;
  }
  ColorCout::print(coutPrefix, "Input file looks fine. continue", YELLOW);

  std::string delimiter = "/";
  std::string ofileName = ifile_path;
  while( int(ofileName.find( delimiter )) != -1 )
	{
		ofileName.erase(0, ofileName.find( delimiter ) + delimiter.length() );
	}
	this->ofileName = this->filePrefix += ofileName;
	this->i_fileName = ifile_path;
  this->i_fileNickName = this->ofileName;

  ColorCout::print(coutPrefix, "Create output file", YELLOW);
  ColorCout::print(coutPrefix, "compressionLevel: "+std::to_string(this->compressionLevel), YELLOW);

  this->oFile = new TFile( this->ofileName.c_str(), "RECREATE", "", this->compressionLevel );
  this->oFile->cd();

  ColorCout::print(coutPrefix, "Fininished, exiting", BOLDGREEN);
  //TThread::UnLock();

  return true;
}

void BetaScope::fileIO_Close()
{
	//TThread::Lock();
  std::string coutPrefix = "BetaScope::fileIO_Close => ";
  ColorCout::print(coutPrefix, "Entering", BOLDGREEN);
  ColorCout::print(coutPrefix, "Writing output files.", YELLOW);

	this->oTree->Write();
	this->oFile->Close();

  ColorCout::print(coutPrefix, "Clean up allocated memory", YELLOW);

  for( auto const& val : this->iTree_branch )
  {
    delete val;
  }

  ColorCout::print(coutPrefix, "Finished, extiting", BOLDGREEN);
  std::time_t _t_end_of_program = std::time(nullptr);
  std::time_t _t_end_of_program_cpu = std::clock();
  this->cpuTime = std::clock() - this->cpuTime;
  ColorCout::print( "  "+ this->i_fileNickName +" Wall Time used: ", std::to_string(_t_end_of_program-this->_t_object_creation) , BOLDYELLOW);
  ColorCout::print( "  "+ this->i_fileNickName +" CPU Time used: ", std::to_string(_t_end_of_program_cpu-this->cpuTime) , BOLDYELLOW);
	//TThread::UnLock();
}
