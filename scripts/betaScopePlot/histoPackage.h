#ifndef HISTO_PACKAGE_H
#define HISTO_PACKAGE_H

class HistoPackage
{
  private:

    int binNum = 100;
    double x_min = -2000.0;
    double x_max = 2000.0;

    std::string my_histo_name;
    std::string my_title;

    std::string observable_name;

    std::string root_file_name;

    std::string cut_str  = "";

    TH1D* histo;

  public:
    HistoPackage(){};
    virtual ~HistoPackage(){};

    void set_histo(std::string histoName, std::string title, std::string xtitle){
      this->histo = new TH1D(histoName.c_str(), title.c_str(), this->binNum, this->x_min, this->x_max);
      this->histo->GetXaxis()->SetTitle(xtitle.c_str());
      this->my_histo_name = histoName;
      this->my_title = title;
    };
    TH1D *get_histo(){return this->histo;};
    void clean(){if(this->histo)delete this->histo; };

    void set_bin(int iValue){this->binNum = iValue;};
    int get_bin(){return this->binNum; };

    void set_min(double xmin ){this->x_min = xmin;};
    void set_max(double xmax ){this->x_max = xmax;};

    double get_xmin(){return this->x_min;}
    double get_xmax(){return this->x_max;}

    void set_cut_str(std::string i_cut){this->cut_str = i_cut;};
    std::string get_cut_str(){return this->cut_str;};

    void set_fname(std::string fname){this->root_file_name=fname;};
    std::string get_fname(){return this->root_file_name;};

    std::string get_observable(){return this->observable_name;};
    std::string get_title(){return this->my_title;};
    std::string get_histoName(){return this->my_histo_name; };

    double get_max_bin_value(){ return this->histo->GetBinContent(this->histo->GetMaximumBin()); }

    void fillFromTree( TTree *itree, std::string observable_name){
      itree->Project( this->my_histo_name.c_str(), observable_name.c_str(), this->cut_str.c_str() );
      this->observable_name = observable_name;
    }
};

#endif
