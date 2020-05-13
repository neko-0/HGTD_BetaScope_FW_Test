#ifndef HISTO_PACKAGE_H
#define HISTO_PACKAGE_H

#include <TH1.h>
#include <RooDataHist.h>
#include <RooDataSet.h>
#include <RooRealVar.h>

class HistoPackage {
private:

  std::string tfile_name;
  std::string observable;
  std::string title;

  int binNum = 100;
  double x_min = -2000.0;
  double x_max = 2000.0;

  std::string histo_name;

  std::string cut_str = "";

  TH1 *histo = NULL;

  std::string tag;
  std::string simple_tag;
  //RooDataHist *dataHist;


public:
  HistoPackage(){};
  virtual ~HistoPackage();

  HistoPackage(
    std::string tfile_name, std::string obs, std::string title,
    int binNum, double x_min, double x_max, std::string tag = "default_tag", std::string type="TH1D"
  );

  HistoPackage( std::string tfile_name, std::string obs, std::string title, std::string tag = "default_tag");

  void CreateTH1(std::string type = "TH1D");

  TH1 *get_histo() const;
  //RooDataHist *get_dataHist() const;

  int get_bin() const;
  double get_xmin() const;
  double get_xmax() const;
  std::string get_cut_str() const;
  std::string get_observable() const;
  std::string get_title() const;
  std::string get_histoName() const;
  std::string get_tfile_name() const;
  double get_max_bin_xvalue() const;
  double get_max_bin_yvalue() const;
  std::string get_tag() const;
  std::string get_simple_tag() const;


  void set_bin( const int &iValue);
  void set_min( const double &xmin);
  void set_max( const double &xmax);
  void set_selection(const std::string &i_cut);

  void Clean();
  void Reset();

  void fillFromTree(TTree *itree, const std::string &selection = "");

};


#endif
