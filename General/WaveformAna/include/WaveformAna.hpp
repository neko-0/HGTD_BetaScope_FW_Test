#include "WaveformAna/include/Waveform.hpp"

class WaveformAna:public Waveform
{
  private:
    int channel_;
    int max_index_;
    double pmax_;
    double tmax_;
    double rise_time_;
    double fall_time_;
    double pulse_area_;
    double pulse_area_undershoot_;
    std::vector<double> dvdt_;
    std::vector<double> cfd_;
    std::vector<double> cfd_fall_;
    std::vector<double> fine_cfd_;
    std::vector<double> fine_cfd_fal_;

    double front_baseline_int;
    double back_baseline_int;

  public:
    void set_channel(int value){this->channel_  = value;}
    void set_pmax(double value){this->pmax_ = value;}
    void set_tmax(double value){this->tmax_ = value;}
    void set_max_index(int value){this->max_index_ = value;}
    void set_rise_time(double value){this->rise_time_ = value;}
    void set_fall_time(double value){this->fall_time_ = value;}
    void set_pulse_area(double value){this->pulse_area_ = value;}
    void set_pulse_area_undershoot(double value){this->pulse_area_undershoot_ = value;}
    void set_dvdt(std::vector<double> value){this->dvdt_ = value};
    void set_cfd(std::vector<double> value){this->cfd_ = value};
    void set_cfd_fall(std::vector<double> value){this->cfd_fall_ = value};
    void set_fine_cfd(std::vector<double> value){this->fine_cfd_ = value};
    void set_fine_cfd_fall(std::vector<double> value){this->fine_cfd_fall_ = value};

    int get_channel(){return this->channel_;}
    int get_max_index(){return this->max_index_;}
    double get_pmax(){return this->pmax_; }
    double get_tmax(){return this->tmax_; }
    double get_rise_time(){return this->rise_time_; }
    double get_fall_time(){return this->fall_time_; }
    double get_pulse_area(){return this->pulse_area_; }
    double get_pulse_area_undershoot(){return this->pulse_area_undershoot_; }
    double get_dvdt(){return this->dvdt_; }
    double get_cfd(){return this->cfd_; }
    double get_cfd_fall(){return this->cfd_fall_; }
    double get_fine_cfd(){return this->fine_cfd_; }
    double get_fine_cfd_fall(){return this->fine_cfd_fall_; }

}
