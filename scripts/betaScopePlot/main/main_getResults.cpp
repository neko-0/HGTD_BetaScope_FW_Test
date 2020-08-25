#include <fmt/format.h>
#include <thread>
#include <future>
#include <mutex>
#include <unistd.h>
#include <stdio.h>
#include <experimental/filesystem>


#include <omp.h>

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
#include "betaScopePlot/include/output_format.h"
#include "betaScopePlot/include/dataSelection.h"

namespace fs = std::experimental::filesystem;

struct ResultHolder
{
  std::string biasVoltage;
  std::map<std::string, FitResult> outData;
  int temperature;
  int trigger_bias;
  int cycle;
  float f_biasVoltage;
};

ResultHolder Result( PlotConfigMgr::ConfigSection sec, int dut_channel, int trigger_channel )
{
  fmt::print("Start processing : {}\n", sec.file_name );

  TFile *loadFile = TFile::Open( sec.file_name.c_str() );
  TTree* itree = (TTree*) loadFile->Get("wfm");

  std::string run_plot_dir = fmt::format("plots.{}",sec.file_name);
  fs::create_directories(run_plot_dir);

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

        std::unique_lock<std::mutex> lck(MTX);
        fitResult = my_fitter.fitter_RooLanGausArea( job.histo_pack, frontBaseArea, backBaseArea, savePlot, run_plot_dir);
      }
      else if( job.fitFunc == "LanGaus" )
      {
        std::lock_guard<std::mutex> lck(MTX);
        fitResult = my_fitter.fitter_RooLanGaus( job.histo_pack, savePlot, run_plot_dir);
      }
      else
      {
        std::lock_guard<std::mutex> lck(MTX);
        fitResult = my_fitter.fitter_fit( job.histo_pack, job.fitFunc, savePlot, run_plot_dir);
      }
    }
    //std::pair<double,double> oParams = std::make_pair( std::get<0>(fitResult), std::get<1>(fitResult) );
    oData.insert( std::pair<std::string, FitResult>(job.tag, fitResult) );
  }

  // output data;
  //std::unique_lock<std::mutex> lck(MTX);
  //std::lock_guard<std::mutex> lck(MTX);
  //DataOutputFormat outfile;
  std::string biasVoltage;
  std::string myBuffer = sec.file_name;
  int cycle;
  if(myBuffer.find(".root.")!=std::string::npos)
  {
    std::string fIndex;
    fIndex = myBuffer.substr(myBuffer.find(".root.")+6, myBuffer.length() );
    biasVoltage = sec.bias + "," + fIndex;
    cycle = std::stoi(fIndex);
  }
  else
  {
    biasVoltage = sec.bias+",1";
    cycle = 1;
  }
  //outfile.CreateBetaScopeOutputFile( biasVoltage.c_str(), oData, sec.temperature, sec.trigger_bias );
  //outfile.ParseRawOutputToINI(biasVoltage, oData, sec.temperature );

  fmt::print("{} is Finished.\n", sec.file_name);
  my_cut_v.clear();
  delete selection;
  //lck.unlock();
  return ResultHolder{biasVoltage, oData, sec.temperature, sec.trigger_bias, cycle, std::stof(sec.bias)};
}

bool sorter (ResultHolder a, ResultHolder b){
  if (a.cycle == b.cycle) return (a.f_biasVoltage < b.f_biasVoltage);
  else return (a.cycle < b.cycle);

}

void DumpOutputFiles(const std::vector<ResultHolder> &results, bool sort=true)
{
  fmt::print("Start dumping results...\n");
  DataOutputFormat outfile;

  if(!sort)
  {
    for(auto &result : results)
    {
      outfile.CreateBetaScopeOutputFile( result.biasVoltage.c_str(), result.outData, result.temperature, result.trigger_bias );
      outfile.ParseRawOutputToINI(result.biasVoltage, result.outData, result.temperature );
    }
  }
  else{
    std::vector<ResultHolder> sorted_results(results);
    std::sort(sorted_results.begin(), sorted_results.end(), sorter);

    for(auto &m_result : sorted_results){
      fmt::print("processing output: {}\n", m_result.biasVoltage );
      outfile.CreateBetaScopeOutputFile( m_result.biasVoltage.c_str(), m_result.outData, m_result.temperature, m_result.trigger_bias );
      outfile.ParseRawOutputToINI(m_result.biasVoltage, m_result.outData, m_result.temperature );
    }
  }
}


void GetResults(std::string plotConfig_fname, std::string outDir = "Results/" )
{
  gROOT->SetBatch(true);
  gStyle->SetOptFit(1);
  gErrorIgnoreLevel = kFatal;
  RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR) ;

  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
  ROOT::EnableThreadSafety();
  //ROOT::EnableImplicitMT(std::thread::hardware_concurrency());

  PlotConfigMgr plotConfig = PlotConfigMgr::ParseConfig(plotConfig_fname);

  int dut_channel = plotConfig.header.dut_channel;
	int trigger_channel = plotConfig.header.trigger_channel;

  /*
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
  */

  unsigned numThreads = std::thread::hardware_concurrency();
  if(numThreads!=1){numThreads = numThreads/2;}

  std::vector<ResultHolder> results_holder;

  #pragma omp parallel for num_threads(numThreads)
  for( std::size_t it =0; it<plotConfig.sections.size(); it++ )
  {
    auto &sec = plotConfig.sections.at(it);
    results_holder.push_back( Result(sec, dut_channel, trigger_channel) );
  }

  DumpOutputFiles(results_holder);

  fmt::print("Start dumping plots...\n");
	if(mkdir( outDir.c_str(), ACCESSPERMS ) == 0)
  {
    fmt::print("Directory {} is created.\n", outDir);
  }
	else
  {
    fmt::print("Directory {} already exists! Previous data will be replaced...\n", outDir);
  }

	system( "python3  $BETASCOPE_SCRIPTS/betaScope_pyScript/result_parser/parseBetaResultsToExcel.py");
  system( "python3  $BETASCOPE_SCRIPTS/betaScope_pyScript/result_parser/parseINItoROOT.py");
  system( fmt::format("mkdir plots_{}", outDir).c_str() );
  system( fmt::format("rm plots_{}/*", outDir).c_str() );
  system( fmt::format("mv *.png plots_{}", outDir).c_str() );
  system( fmt::format("mv plots.* plots_{}", outDir).c_str() );
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
	if(argc == 2) GetResults(argv[1]);
	if(argc == 3) GetResults(argv[1], argv[2]);
	if(argc > 4) printf("Error! Check the input arguments!");
}
