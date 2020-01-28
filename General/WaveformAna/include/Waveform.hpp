/*
  Class for waveform
*/
#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <TObject.h>
#include <string>
#include <vector>

template <class data_type, class input_type> class Waveform : public TObject {
protected:
  std::vector<data_type> v1_ = {};
  std::vector<data_type> v2_ = {};

public:
  Waveform(){};
  virtual ~Waveform(){};

  Waveform(std::vector<input_type> *amp, std::vector<input_type> *t);

  Waveform(std::vector<input_type> *amp);

  Waveform(std::vector<input_type> amp, std::vector<input_type> t);

  Waveform(std::vector<input_type> amp);

  Waveform(input_type amp[], input_type t[]);

  Waveform(input_type amp[]);

  std::vector<data_type> get_v1() { return this->v1_; }
  std::vector<data_type> get_v2() { return this->v2_; }

  data_type get_v1_value(int index) { return this->v1_.at(index); }
  data_type get_v2_value(int index) { return this->v2_.at(index); }

  void set_v1_value(int index, data_type value) { this->v1_.at(index) = value; }
  void set_v2_value(int index, data_type value) { this->v2_.at(index) = value; }

  void set_v1(std::vector<data_type> value) { this->v1_ = value; }
  void set_v2(std::vector<data_type> value) { this->v2_ = value; }

  ClassDef(Waveform, 1)
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(std::vector<input_type> *amp,
                                          std::vector<input_type> *t) {
  if (amp->size() != t->size()) {
    throw false;
  }

  for (std::size_t i = 0, max = amp->size(); i < max; i++) {
    this->v1_.push_back(t->at(i));
    this->v2_.push_back(amp->at(i));
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(std::vector<input_type> amp,
                                          std::vector<input_type> t) {
  if (amp.size() != t.size()) {
    throw false;
  }

  for (std::size_t i = 0, max = amp.size(); i < max; i++) {
    this->v1_.push_back(t.at(i));
    this->v2_.push_back(amp.at(i));
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(std::vector<input_type> *amp) {
  for (std::size_t i = 0, max = amp->size(); i < max; i++) {
    this->v1_.push_back(i);
    this->v2_.push_back(amp->at(i));
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(std::vector<input_type> amp) {
  for (std::size_t i = 0, max = amp.size(); i < max; i++) {
    this->v1_.push_back(i);
    this->v2_.push_back(amp.at(i));
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(input_type amp[]) {
  for (std::size_t i = 0, max = sizeof(amp) / sizeof(amp[0]); i < max; i++) {
    this->v1_.push_back(i);
    this->v2_.push_back(amp[i]);
  }
};

#endif // WAVEFORM_H
