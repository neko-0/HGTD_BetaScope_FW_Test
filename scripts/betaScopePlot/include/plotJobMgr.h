#ifndef PLOT_LIST_H
#define PLOT_LIST_H

#include <map>
#include <string>
#include <vector>

#include "betaScopePlot/include/histoPackage.h"

struct PlotList
{
  int channel;
  std::string fitFunc;
  HistoPackage histo_pack;
  std::string tag;

};


struct PlotJobMgr
{
  std::vector<PlotList> jobs;

  PlotJobMgr(){};
  virtual ~PlotJobMgr(){};

  PlotJobMgr operator +(const PlotJobMgr &inputConfigMgr);
  PlotJobMgr &operator +=(const PlotJobMgr &inputConfigMgr);

  void Fill(
    const int &channel, const std::string &fitFunc,
    const HistoPackage &histoPack, const std::string &tag
   );

  static PlotJobMgr Create_Default_List( std::string tfile_name );

  static PlotJobMgr Read_List(std::string tfile_name, std::string job_list="");
};



#endif
