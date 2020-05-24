#include <fmt/format.h>

#include <RooFit.h>
#include <TTree.h>
#include <TROOT.h>

#include "betaScopePlot/include/histoPackage.h"

HistoPackage::~HistoPackage()
{
  //HistoPackage::Clean();
}

void HistoPackage::Clean()
{
  //delete this->histo;
  //if(this->histo){ delete this->histo; }
  if(gROOT->FindObject(this->histo->GetName())){delete gROOT->FindObject(this->histo->GetName());}
}

void HistoPackage::Reset()
{
  //this->histo->Reset();
  //this->histo->GetXaxis()->SetLimits(this->x_min, this->x_max);
  //this->histo->Rebin(this->binNum);
  HistoPackage::Clean();
  HistoPackage::CreateTH1();
}

void HistoPackage::CreateTH1(std::string type)
{
  if( type == "TH1D" )
  {
    this->histo = new TH1D( this->histo_name.c_str(), "", this->binNum, this->x_min, this->x_max);
  }
  else if( type == "TH1F" )
  {
    this->histo = new TH1F( this->histo_name.c_str(), "", this->binNum, this->x_min, this->x_max);
  }
  else if( type == "TH1I" )
  {
    this->histo = new TH1I( this->histo_name.c_str(), "", this->binNum, this->x_min, this->x_max);
  }
  else
  {
    this->histo = NULL;
  }

  if(this->histo)
  {
    this->histo->GetXaxis()->SetTitle(this->title.c_str());
  }
}

HistoPackage::HistoPackage(std::string tfile_name,
  std::string obs, std::string title,
  int binNum, double x_min, double x_max, std::string tag, std::string type
) : tfile_name(tfile_name), observable(obs), title(title),
    binNum(binNum), x_min(x_min), x_max(x_max), tag(tag)
{
  this->simple_tag = tag;
  this->tag = fmt::format("{}_id{}_{}_", this->tag, std::rand(), tfile_name.substr(0, tfile_name.find(".root.")));
  this->histo_name = fmt::format("{}_{}_id{}_", tfile_name, tag, std::rand());
  HistoPackage::CreateTH1();
}

HistoPackage::HistoPackage(std::string tfile_name, std::string obs, std::string title, std::string tag)
  : HistoPackage::HistoPackage(tfile_name, obs, title, 100, 1, 1, tag){}


TH1 *HistoPackage::get_histo() const { return this->histo; }

//RooDataHist *HistoPackage::get_dataHist() const { return this->dataHist; }

int HistoPackage::get_bin() const { return this->binNum; }

double HistoPackage::get_xmin() const { return this->x_min; }

double HistoPackage::get_xmax() const { return this->x_max; }

std::string HistoPackage::get_cut_str() const { return this->cut_str; }

std::string HistoPackage::get_observable() const { return this->observable; }

std::string HistoPackage::get_title() const { return this->title; }

std::string HistoPackage::get_histoName() const { return this->histo_name; }

std::string HistoPackage::get_tfile_name() const { return this->tfile_name; }

std::string HistoPackage::get_tag() const { return this->tag; }

std::string HistoPackage::get_simple_tag() const { return this->simple_tag; }

double HistoPackage::get_max_bin_xvalue() const
{
  if(this->histo)
  {
    return this->histo->GetBinContent(this->histo->GetMaximumBin());
  }
  else
  {
    return -10e11;
  }
}

double HistoPackage::get_max_bin_yvalue() const
{
  auto xbin = this->histo->GetMaximumBin();
  if(this->histo)
  {
    return this->histo->GetXaxis()->GetBinCenter(xbin);
  }
  else
  {
    return -10e11;
  }
}

void HistoPackage::set_bin( const int &iValue) { this->binNum = iValue; };

void HistoPackage::set_min( const double &xmin){ this->x_min = xmin; };

void HistoPackage::set_max( const double &xmax){ this->x_max = xmax; };

void HistoPackage::set_selection( const std::string &i_cut) { this->cut_str = i_cut; };

void HistoPackage::fillFromTree( TTree *itree, const std::string &selection )
{
  if(!selection.empty()){ this->cut_str = selection; }
  itree->Project( this->histo->GetName(), this->observable.c_str(), this->cut_str.c_str());
}
