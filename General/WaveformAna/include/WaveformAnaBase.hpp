#ifndef WAVEFORMANABASE_H
#define WAVEFORMANABASE_H

#include "WaveformAna/include/Waveform.hpp"

class WaveformAnaBase
{
public:
  WaveformAnaBase(){};
  virtual ~WaveformAnaBase(){};

  virtual void channel(const int &value) = 0;
  virtual void pmax(const double &value) = 0;
  virtual void tmax(const double &value) = 0;
  virtual void fit_tmax(const double &value) = 0;
  virtual void fit_tmax_chi(const double &value) = 0;
  virtual void zero_cross_tmax(const double &value) = 0;
  virtual void zero_cross_tmax_chi(const double &value) = 0;
  virtual void max_index(const int &value) = 0;
  virtual void neg_pmax(const double &value) = 0;
  virtual void neg_tmax(const double &value) = 0;
  virtual void neg_max_index(const int &value) = 0;
  virtual void rise_time(const double &value) = 0;
  virtual void fall_time(const double &value) = 0;
  virtual void pulse_area(const double &value) = 0;
  virtual void pulse_area_undershoot(const double &value) = 0;
  virtual void front_baseline_int(const double &value) = 0;
  virtual void back_baseline_int(const double &value) = 0;
  virtual void rms(const double &value) = 0;
  virtual void undershoot_pmax(const double &value) = 0;
  virtual void undershoot_tmax(const double &value) = 0;
  virtual void dvdt(std::vector<double> value) = 0;
  virtual void cfd(std::vector<double> value) = 0;
  virtual void cfd_fall(std::vector<double> value) = 0;
  virtual void fine_cfd(std::vector<double> value) = 0;
  virtual void fine_cfd_fall(std::vector<double> value) = 0;
  virtual void tot(std::vector<double> value) = 0;
  virtual void threashold_time(std::vector<double> value) = 0;

  virtual int channel() const = 0;
  virtual int max_index() const = 0;
  virtual double pmax() const = 0;
  virtual double tmax() const = 0;
  virtual double fit_tmax() const = 0;
  virtual double fit_tmax_chi() const = 0;
  virtual double zero_cross_tmax() const = 0;
  virtual double zero_cross_tmax_chi() const = 0;
  virtual int meg_max_index() const = 0;
  virtual double neg_pmax() const = 0;
  virtual double neg_tmax() const = 0;
  virtual double rise_time() const = 0;
  virtual double fall_time() const = 0;
  virtual double pulse_area() const = 0;
  virtual double pulse_area_undershoot() const = 0;
  virtual std::vector<double> dvdt() const = 0;
  virtual std::vector<double> cfd() const = 0;
  virtual std::vector<double> cfd_fall() const = 0;
  virtual std::vector<double> fine_cfd() const = 0;
  virtual std::vector<double> fine_cfd_fall() const = 0;
  virtual double front_baseline_int() const = 0;
  virtual double back_baseline_int() const = 0;
  virtual std::vector<double> threshold_time() const = 0;
  virtual double rms() const = 0;
  virtual double undershoot_pmax() const = 0;
  virtual double undershoot_tmax() const = 0;
  virtual std::vector<double> tot() const = 0;

};

#endif // WAVEFORMANABASE_H
