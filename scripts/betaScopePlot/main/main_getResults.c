#include <fmt/format.h>
#include <thread>
#include <future>
#include <mutex>
#include <unistd.h>
#include <stdio.h>

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>

#include <TROOT.h>
#include <TThread.h>
#include <Math/MinimizerOptions.h>

#include "betaScopePlot/include/fitter.h"
#include "betaScopePlot/include/plotConfigMgr.h"
#include "betaScopePlot/include/plotJobMgr.h"
#include "betaScopePlot/include/histoPackage.h"
#include "betaScopePlot/include/dataOutputFormat.h"
#include "betaScopePlot/include/dataSelection.h"

void result( PlotConfigMgr::ConfigSection sec, int dut_channel, int trigger_channel )
{
  fmt::print("Start processing : {}\n", sec.file_name );

  TFile *loadFile = TFile::Open( sec.file_name.c_str() );
  TTree* itree = (TTree*) loadFile->Get("wfm");

  // parsing cuts
  std::string delimiter = " ";
  std::string my_cut_str = sec.cut[dut_channel-1];
  std::vector<std::string> my_cut_v;
  while( int(my_cut_str.find(delimiter)) != -1 )
  {
    my_cut_v.push_back( my_cut_str.substr(0, my_cut_str.find(delimiter) ) );
    my_cut_str.erase(0, my_cut_str.find(delimiter) + delimiter.length() );
  }
  my_cut_v.push_back( my_cut_str );

  DataSelection *selection = new DataSelection(my_cut_v, dut_channel, trigger_channel );

  // output result ;
  std::map<std::string, FitResult> oData;

  // getting booked plot list
  PlotJobMgr plotMgr = PlotJobMgr::Create_Default_List( sec.file_name );
  PlotJobMgr addition_plotMgr = PlotJobMgr::Read_List( sec.file_name );
  plotMgr += addition_plotMgr;
  for( auto &job : plotMgr.jobs )
  {

    FitResult fitResult;
    Fitter my_fitter;
    bool savePlot = false;

    // fit multiple times for better ranges;
    for(int k = 0; k < 5; k++)
    {
      job.histo_pack.Reset();
      job.histo_pack.fillFromTree(itree, selection->cuts );
      if( k==4 )
      {
        savePlot=true;
      }
      if( job.fitFunc == "LanGausArea" )
      {
        // some baseline histos
        HistoPackage frontBaseArea( sec.file_name, Form("frontBaselineInt_indepBaseCorr%i[0]/1.0E-15", job.channel ), "front base area");
        frontBaseArea.fillFromTree( itree );

        HistoPackage backBaseArea( sec.file_name, Form("backBaselineInt_indepBaseCorr%i[0]/1.0E-15", job.channel ), "back base area");
        frontBaseArea.fillFromTree( itree );

        std::lock_guard<std::mutex> lck(MTX);
        fitResult = my_fitter.fitter_RooLanGausArea( job.histo_pack, frontBaseArea, backBaseArea, savePlot);
      }
      else if( job.fitFunc == "LanGaus" )
      {
        std::lock_guard<std::mutex> lck(MTX);
        fitResult = my_fitter.fitter_RooLanGaus( job.histo_pack, savePlot);
      }
      else
      {
        std::lock_guard<std::mutex> lck(MTX);
        fitResult = my_fitter.fitter_fit( job.histo_pack, job.fitFunc, savePlot);
      }
    }
    //std::pair<double,double> oParams = std::make_pair( std::get<0>(fitResult), std::get<1>(fitResult) );
    oData.insert( std::pair<std::string, FitResult>(job.tag, fitResult) );
  }

  // output data;
  //std::unique_lock<std::mutex> lck(MTX);
  std::lock_guard<std::mutex> lck(MTX);
  DataOutputFormat outfile;
  std::string biasVoltage;
  std::string biasVoltage2;
  std::string myBuffer = sec.file_name;
  if(myBuffer.find(".root.")!=std::string::npos)
  {
    std::string fIndex;
    fIndex = myBuffer.substr(myBuffer.find(".root.")+5, myBuffer.length() );
    biasVoltage = sec.bias + "." + fIndex;
    biasVoltage2 = sec.bias + "findex" + fIndex;
  }
  else
  {
    biasVoltage = sec.bias;
    biasVoltage2 = sec.bias;
  }
  outfile.CreateBetaScopeOutputFile( biasVoltage.c_str(), oData, sec.temperature, sec.trigger_bias );
  outfile.ParseRawOutputToINI(biasVoltage2, oData, sec.temperature );

  fmt::print("{} is Finished.\n", sec.file_name);
  my_cut_v.clear();
  delete selection;
  //lck.unlock();
}


void getResults(std::string plotConfig_fname, std::string outDir = "Results/" )
{
  gROOT->SetBatch(true);

  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
  ROOT::EnableThreadSafety();
  //ROOT::EnableImplicitMT(std::thread::hardware_concurrency());

  PlotConfigMgr plotConfig = PlotConfigMgr::ParseConfig(plotConfig_fname);

  int dut_channel = plotConfig.header.dut_channel;
	int trigger_channel = plotConfig.header.trigger_channel;

  //std::vector<std::future<void>> workers;
  boost::asio::thread_pool pool(5);

  // looping through files
  for( auto &sec : plotConfig.sections )
  {
    //result(sec, dut_channel, trigger_channel);
    //workers.emplace_back( std::async(std::launch::async | std::launch::deferred, result, sec, dut_channel, trigger_channel) );
    boost::asio::post(pool, boost::bind(result, sec, dut_channel, trigger_channel));
  }

  //for( std::size_t id = 0; id < workers.size(); id++ ){ workers[id].wait(); }
  pool.join();

  fmt::print("Start dumping plots...\n");
	if(mkdir( outDir.c_str(), ACCESSPERMS ) == 0)
  {
    fmt::print("Directory {} is created.\n", outDir);
  }
	else
  {
    fmt::print("Directory {} already exists! Previous data will be replaced...\n", outDir);
  }

	system( "python3  $BETASCOPE_SCRIPTS/betaScope_pyScript/parseBetaResultsToExcel.py");
  system( "python3  $BETASCOPE_SCRIPTS/betaScope_pyScript/parseINItoROOT.py");
  system( fmt::format("mkdir plots_{}", outDir).c_str() );
  system( fmt::format("mv *.png plots_{}", outDir).c_str() );
  system( fmt::format("mv *_results.ini {}", outDir).c_str() );
  system( fmt::format("mv *_results.xlsx {}", outDir).c_str() );
  system( fmt::format("cp *Description*.ini {}", outDir).c_str() );
  system( fmt::format("cp *_results.root {}", outDir).c_str() );

  fmt::print("Finished!\n");

	gROOT->SetBatch(false);
}

int main(int argc, const char *argv[])
{
	if(argc == 1) printf("Error! Runlist Requires\n");
	if(argc == 2) getResults(argv[1]);
	if(argc == 3) getResults(argv[1], argv[2]);
	if(argc > 4) printf("Error! Check the input arguments!");
}
