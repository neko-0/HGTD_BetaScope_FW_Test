#include "fitter.h"
#include "histoPackage.h"
#include "dataOutputFormat.h"
#include "plotList.h"
#include "dataSelection.h"

#include <fmt/format.h>

#include "betaScopePlot/include/plotConfigMgr.h"

void getResults(std::string plotConfig_fname, std::string outDir = "Results/" )
{
  gROOT->SetBatch(true);

  PlotConfigMgr plotConfig = PlotConfigMgr::ParseConfig(plotConfig_fname);

  int dut_channel = plotConfig.header.dut_channel;
	int trigger_channel = plotConfig.header.trigger_channel;

  // looping through files
  for( auto &sec : plotConfig.sections )
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
    std::vector<PlotList> my_plot_jobs = create_job(); // from plotList.cpp
    for( const auto job : my_plot_jobs )
    {
      HistoPackage myHisto;
      myHisto.set_cut_str( selection->cuts.GetTitle() );
      myHisto.set_fname( sec.file_name );
      myHisto.set_bin( job.binNum );
      myHisto.set_min( job.x_min );
      myHisto.set_max( job.x_max );

      FitResult fitResult;
      Fitter my_fitter;
      bool savePlot = false;
      // fit multiple times for better ranges;
      for(int k = 0; k < 5; k++)
      {
        myHisto.set_histo( job.histo_name, job.histo_title, job.xTitle);
        myHisto.fillFromTree(itree, job.observable_name );
        if(k==4)savePlot=true;
        if( job.fitFunc.compare("LanGausArea")==0 )
        {
          // some baseline histos
          HistoPackage frontBaseArea;
          frontBaseArea.set_histo("frontBase", "frontBase", "front base area");
          frontBaseArea.fillFromTree( itree, Form("frontBaseLineInt%i[0]/1.0E-15", job.channel ) );
          HistoPackage backBaseArea;
          backBaseArea.set_histo("backBase", "backBase", "back base area");
          frontBaseArea.fillFromTree( itree, Form("backBaseLineInt%i[0]/1.0E-15", job.channel ) );
          fitResult = my_fitter.fitter_RooLanGausArea(myHisto, frontBaseArea, backBaseArea, savePlot);
        }
        else if( job.fitFunc.compare("LanGaus") == 0 )
        {
          fitResult = my_fitter.fitter_RooLanGaus(myHisto, savePlot);
        }
        else
        {
          my_fitter.set_fitter( job.fitFunc );
          fitResult = my_fitter.fitter_fit(myHisto, savePlot);
        }
        myHisto.clean();
      }
      //std::pair<double,double> oParams = std::make_pair( std::get<0>(fitResult), std::get<1>(fitResult) );
      oData.insert( std::pair<std::string, FitResult>(job.tag, fitResult) );
    }

    // output data;
    DataOutputFormat outfile;
    std::string biasVoltage;
    std::string myBuffer = sec.file_name;
    if(myBuffer.find(".root.")!=std::string::npos)
    {
      std::string fIndex;
      fIndex = myBuffer.substr(myBuffer.find(".root.")+5, myBuffer.length() );
      biasVoltage = sec.bias + "." + fIndex;
    }
    else
    {
      biasVoltage = sec.bias;
    }
    outfile.CreateBetaScopeOutputFile( biasVoltage.c_str(), oData, sec.temperature, sec.trigger_bias );

    fmt::print("{} is Finished.\n", sec.file_name);
    my_cut_v.clear();
    delete selection;
  }

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
