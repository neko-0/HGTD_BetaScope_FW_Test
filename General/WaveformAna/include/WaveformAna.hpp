#ifndef WAVEFORMANA_H
#define WAVEFORMANA_H

#include "WaveformAna/include/Waveform.hpp"

template <class data_type, class input_type>
class WaveformAna : public Waveform<data_type, input_type>
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
    WaveformAna() {};
    ~WaveformAna() {};

    WaveformAna(std::vector<input_type> *v1, std::vector<input_type> *v2)
      : Waveform<data_type, input_type>(v1, v2) {};

    WaveformAna( TTreeReaderArray<input_type> *v1, TTreeReaderArray<input_type> *v2 ) : Waveform<data_type, input_type>(v1, v2){};

    WaveformAna(TTreeReaderArray<input_type> *v1, TTreeReaderArray<input_type> *v2,
      const bool &invert, const float &ampFactor, const float &tFactor,
      const bool &tResample, const float &xori, const float &dt
    ) : Waveform<data_type, input_type>(v1, v2, invert, ampFactor, tFactor, tResample, xori, dt) {};

    void set_channel(const int &value){ this->channel_ = value; }
    void set_pmax(const double &value){ this->pmax_ = value; }
    void set_tmax(const double &value){ this->tmax_ = value; }
    void set_max_index(const int &value){ this->max_index_ = value; }
    void set_neg_pmax(const double &value){ this->neg_pmax_ = value; }
    void set_neg_tmax(const double &value){ this->neg_tmax_ = value; }
    void set_neg_max_index(const int &value){ this->neg_max_index_ = value; }
    void set_rise_time(const double &value){ this->rise_time_ = value; }
    void set_fall_time(const double &value){ this->fall_time_ = value; }
    void set_pulse_area(const double value){ this->pulse_area_ = value; }
    void set_pulse_area_undershoot(const double &value){ this->pulse_area_undershoot_ = value; }
    void set_dvdt(const std::vector<double> &value){ this->dvdt_ = value; }
    void set_cfd(const std::vector<double> &value){ this->cfd_ = value; }
    void set_cfd_fall(const std::vector<double> &value){ this->cfd_fall_ = value; }
    void set_fine_cfd(const std::vector<double> &value){ this->fine_cfd_ = value; }
    void set_fine_cfd_fall(const std::vector<double> &value){ this->fine_cfd_fall_ = value; }
    void set_front_baseline_int(const double &value){ this->front_baseline_int_ = value; }
    void set_back_baseline_int(const double &value){ this->back_baseline_int_ = value; }
    void set_threashold_time(const std::vector<double> &value){ this->threshold_time_ = value; }
    void set_rms(const double &value){ this->rms_ = value; }

    int get_channel() const { return this->channel_; }
    int get_max_index() const { return this->max_index_; }
    double get_pmax() const { return this->pmax_; }
    double get_tmax() const { return this->tmax_; }
    int get_meg_max_index() const { return this->neg_max_index_; }
    double get_neg_pmax() const { return this->neg_pmax_; }
    double get_neg_tmax() const { return this->neg_tmax_; }
    double get_rise_time() const { return this->rise_time_; }
    double get_fall_time() const { return this->fall_time_; }
    double get_pulse_area() const { return this->pulse_area_; }
    double get_pulse_area_undershoot() const { return this->pulse_area_undershoot_; }
    std::vector<double> get_dvdt() const { return this->dvdt_; }
    std::vector<double> get_cfd() const { return this->cfd_; }
    std::vector<double> get_cfd_fall() const { return this->cfd_fall_; }
    std::vector<double> get_fine_cfd() const { return this->fine_cfd_; }
    std::vector<double> get_fine_cfd_fall() const { return this->fine_cfd_fall_; }
    double get_front_baseline_int() const { return this->front_baseline_int_; }
    double get_back_baseline_int() const { return this->back_baseline_int_; }
    std::vector<double> get_threshold_time() const { return this->threshold_time_; }
    double get_rms() const { return this->rms_; }

    ClassDef(WaveformAna, 1)
};


#endif // WAVEFORMANA_H
