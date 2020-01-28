#ifndef WAVEFORMANA_H
#define WAVEFORMANA_H

#include "WaveformAna/include/Waveform.hpp"

template <class data_type, class input_type>
class WaveformAna:public Waveform<data_type,input_type>
{
  private:
    int channel_;
    int max_index_;
    double pmax_;
    double tmax_;
    double neg_pmax_;
    double neg_tmax_;
    int neg_max_index_;
    double rise_time_;
    double fall_time_;
    double pulse_area_;
    double pulse_area_undershoot_;
    std::vector<double> dvdt_;
    std::vector<double> cfd_;
    std::vector<double> cfd_fall_;
    std::vector<double> fine_cfd_;
    std::vector<double> fine_cfd_fall_;
    std::vector<double> threshold_time_;
    double rms_;

    double front_baseline_int_;
    double back_baseline_int_;

  public:

    WaveformAna(){};
    ~WaveformAna(){};

    WaveformAna(std::vector<input_type> *v1, std::vector<input_type> *v2);


    void set_channel(int value){this->channel_  = value;}
    void set_pmax(double value){this->pmax_ = value;}
    void set_tmax(double value){this->tmax_ = value;}
    void set_max_index(int value){this->max_index_ = value;}
    void set_neg_pmax(double value){this->neg_pmax_ = value;}
    void set_neg_tmax(double value){this->neg_tmax_ = value;}
    void set_neg_max_index(int value){this->neg_max_index_ = value;}
    void set_rise_time(double value){this->rise_time_ = value;}
    void set_fall_time(double value){this->fall_time_ = value;}
    void set_pulse_area(double value){this->pulse_area_ = value;}
    void set_pulse_area_undershoot(double value){this->pulse_area_undershoot_ = value;}
    void set_dvdt(std::vector<double> value){this->dvdt_ = value; }
    void set_cfd(std::vector<double> value){this->cfd_ = value; }
    void set_cfd_fall(std::vector<double> value){this->cfd_fall_ = value;}
    void set_fine_cfd(std::vector<double> value){this->fine_cfd_ = value;}
    void set_fine_cfd_fall(std::vector<double> value){this->fine_cfd_fall_ = value; }
    void set_front_baseline_int(double value){this->front_baseline_int_ = value;}
    void set_back_baseline_int(double value){this->back_baseline_int_ = value;}
    void set_threashold_time(std::vector<double> value){this->threshold_time_ = value;}
    void set_rms(double value){this->rms_ = value;}

    int get_channel(){return this->channel_;}
    int get_max_index(){return this->max_index_;}
    double get_pmax(){return this->pmax_; }
    double get_tmax(){return this->tmax_; }
    int get_meg_max_index(){return this->neg_max_index_;}
    double get_neg_pmax(){return this->neg_pmax_; }
    double get_neg_tmax(){return this->neg_tmax_; }
    double get_rise_time(){return this->rise_time_; }
    double get_fall_time(){return this->fall_time_; }
    double get_pulse_area(){return this->pulse_area_; }
    double get_pulse_area_undershoot(){return this->pulse_area_undershoot_; }
    std::vector<double> get_dvdt(){return this->dvdt_; }
    std::vector<double> get_cfd(){return this->cfd_; }
    std::vector<double> get_cfd_fall(){return this->cfd_fall_; }
    std::vector<double> get_fine_cfd(){return this->fine_cfd_; }
    std::vector<double> get_fine_cfd_fall(){return this->fine_cfd_fall_; }
    double get_front_baseline_int(){return this->front_baseline_int_;}
    double get_back_baseline_int(){return this->back_baseline_int_;}
    std::vector<double> get_threshold_time(){return this->threshold_time_;}
    double get_rms(){return this->rms_;}

ClassDef(WaveformAna, 1)
};

template <class data_type, class input_type>
WaveformAna<data_type,input_type>::WaveformAna(std::vector<input_type> *v1, std::vector<input_type> *v2):Waveform<data_type,input_type>(v1,v2){};

#endif // WAVEFORMANA_H
