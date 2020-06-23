#ifndef WAVEFORMANA_H
#define WAVEFORMANA_H

#include "WaveformAna/include/Waveform.hpp"
#include "WaveformAna/include/WaveformAnaBase.hpp"

template <class data_type, class input_type>
class WaveformAna : public Waveform<data_type, input_type>, public WaveformAnaBase
{
private:
    int channel_ = -1;
    int max_index_ = -1;
    double pmax_ = -11e11;
    double tmax_ = -11e11;
    double fit_tmax_ = -11e11;
    double fit_tmax_chi_ = -11e11;
    double zero_cross_tmax_ = -11e11;
    double zero_cross_tmax_chi_ = -11e11;
    double neg_pmax_ = -11e11;
    double neg_tmax_ = -11e11;
    int neg_max_index_ = -1;
    double rise_time_ = -11e11;
    double fall_time_ = -11e11;
    double pulse_area_ = -11e11;
    double pulse_area_undershoot_ = -11e11;
    double undershoot_pmax_ = -11e11;
    double undershoot_tmax_ = -11e11;
    std::vector<double> dvdt_ = {};
    std::vector<double> cfd_ = {};
    std::vector<double> cfd_fall_ = {};
    std::vector<double> fine_cfd_ = {};
    std::vector<double> fine_cfd_fall_ = {};
    std::vector<double> threshold_time_ = {};
    std::vector<double> tot_ = {};
    double rms_ = -11e11;

    double front_baseline_int_ = -11e11;
    double back_baseline_int_ = -11e11;

public:
    WaveformAna() {};
    ~WaveformAna() {};

    WaveformAna(std::vector<input_type> *v1, std::vector<input_type> *v2)
      : Waveform<data_type, input_type>(v1, v2) {};

    WaveformAna( TTreeReaderArray<input_type> *v1, TTreeReaderArray<input_type> *v2 ) : Waveform<data_type, input_type>(v1, v2){};

    WaveformAna(TTreeReaderArray<input_type> *v1, TTreeReaderArray<input_type> *v2,
      const bool &invert, const float &ampFactor, const float &tFactor,
      const bool &tResample, const float &xori, const float &dt
    ) : Waveform<data_type, input_type>(v1, v2, invert, ampFactor, tFactor, tResample, xori, dt) {
      dvdt_.reserve(2000);
      cfd_.reserve(2000);
      cfd_fall_.reserve(2000);
      fine_cfd_.reserve(2000);
      fine_cfd_fall_.reserve(2000);
      threshold_time_.reserve(2000);
      tot_.reserve(2000);
    };

    virtual std::ostream &print(std::ostream &os) const { return Waveform<data_type,input_type>::print(os); }

    void channel(const int &value){ this->channel_ = value; }
    void pmax(const double &value){ this->pmax_ = value; }
    void tmax(const double &value){ this->tmax_ = value; }
    void fit_tmax(const double &value){ this->fit_tmax_ = value; }
    void fit_tmax_chi(const double &value){ this->fit_tmax_chi_ = value; }
    void zero_cross_tmax(const double &value){ this->zero_cross_tmax_ = value; }
    void zero_cross_tmax_chi(const double &value){ this->zero_cross_tmax_chi_ = value; }
    void max_index(const int &value){ this->max_index_ = value; }
    void neg_pmax(const double &value){ this->neg_pmax_ = value; }
    void neg_tmax(const double &value){ this->neg_tmax_ = value; }
    void neg_max_index(const int &value){ this->neg_max_index_ = value; }
    void rise_time(const double &value){ this->rise_time_ = value; }
    void fall_time(const double &value){ this->fall_time_ = value; }
    void pulse_area(const double &value){ this->pulse_area_ = value; }
    void pulse_area_undershoot(const double &value){ this->pulse_area_undershoot_ = value; }
    void front_baseline_int(const double &value){ this->front_baseline_int_ = value; }
    void back_baseline_int(const double &value){ this->back_baseline_int_ = value; }
    void rms(const double &value){ this->rms_ = value; }
    void undershoot_pmax(const double &value){ this->undershoot_pmax_ = value;}
    void undershoot_tmax(const double &value){ this->undershoot_tmax_ = value;}
    void dvdt(std::vector<double> value){ this->dvdt_ = std::move(value); }
    void cfd(std::vector<double> value){ this->cfd_ = std::move(value); }
    void cfd_fall(std::vector<double> value){ this->cfd_fall_ = std::move(value); }
    void fine_cfd(std::vector<double> value){ this->fine_cfd_ = std::move(value); }
    void fine_cfd_fall(std::vector<double> value){ this->fine_cfd_fall_ = std::move(value); }
    void tot(std::vector<double> value){ this->tot_ = std::move(value);}
    void threashold_time(std::vector<double> value){ this->threshold_time_ = std::move(value); }

    int channel() const { return this->channel_; }
    int max_index() const { return this->max_index_; }
    double pmax() const { return this->pmax_; }
    double tmax() const { return this->tmax_; }
    double fit_tmax() const { return this->fit_tmax_; }
    double fit_tmax_chi() const { return this->fit_tmax_chi_; }
    double zero_cross_tmax() const { return this->zero_cross_tmax_; }
    double zero_cross_tmax_chi() const { return this->zero_cross_tmax_chi_; }
    int meg_max_index() const { return this->neg_max_index_; }
    double neg_pmax() const { return this->neg_pmax_; }
    double neg_tmax() const { return this->neg_tmax_; }
    double rise_time() const { return this->rise_time_; }
    double fall_time() const { return this->fall_time_; }
    double pulse_area() const { return this->pulse_area_; }
    double pulse_area_undershoot() const { return this->pulse_area_undershoot_; }
    std::vector<double> dvdt() const { return this->dvdt_; }
    std::vector<double> cfd() const { return this->cfd_; }
    std::vector<double> cfd_fall() const { return this->cfd_fall_; }
    std::vector<double> fine_cfd() const { return this->fine_cfd_; }
    std::vector<double> fine_cfd_fall() const { return this->fine_cfd_fall_; }
    double front_baseline_int() const { return this->front_baseline_int_; }
    double back_baseline_int() const { return this->back_baseline_int_; }
    std::vector<double> threshold_time() const { return this->threshold_time_; }
    double rms() const { return this->rms_; }
    double undershoot_pmax() const {return this->undershoot_pmax_; }
    double undershoot_tmax() const {return this->undershoot_tmax_; }
    std::vector<double> tot() const {return this->tot_; }

    ClassDef(WaveformAna, 1)
};


#endif // WAVEFORMANA_H
