#ifndef PLOT_LIST_H
#define PLOT_LIST_H

#include <map>
#include <string>
#include <vector>

struct PlotList {
  std::string histo_name;
  std::string histo_title;
  std::string observable_name;
  std::string xTitle;
  std::string fitFunc;
  int binNum;
  double x_min;
  double x_max;
  int channel;
  std::string tag;
};

class BookPlot {
private:
  std::vector<PlotList> my_plot_list;

public:
  BookPlot(){};
  virtual ~BookPlot(){};

  void book_plot(int channel, std::string tag, std::string histo_name,
                 std::string histo_title, std::string obs, std::string xTitle,
                 std::string fitFunc, int binNum, double x_min, double x_max) {
    PlotList my_plot;
    my_plot.histo_name = histo_name;
    my_plot.histo_title = histo_title;
    my_plot.observable_name = obs;
    my_plot.xTitle = xTitle;
    my_plot.fitFunc = fitFunc;
    my_plot.binNum = binNum;
    my_plot.x_min = x_min;
    my_plot.x_max = x_max;
    my_plot.channel = channel;
    my_plot.tag = tag;
    this->my_plot_list.push_back(my_plot);
  };

  std::vector<PlotList> get_booked_plot() { return this->my_plot_list; };
};

std::vector<PlotList> create_job();

#endif
