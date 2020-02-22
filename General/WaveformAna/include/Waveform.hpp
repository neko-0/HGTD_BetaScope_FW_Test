/*
  Class for waveform
*/
#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <TObject.h>
#include <TTreeReaderArray.h>
#include <string>
#include <vector>

template <class data_type, class input_type> class Waveform : public TObject
{
protected:
    std::vector<data_type> v1_ = {};
    std::vector<data_type> v2_ = {};

public:
    Waveform() {};
    virtual ~Waveform() {};

    Waveform(std::vector<input_type> *amp, std::vector<input_type> *t);

    Waveform(std::vector<input_type> *amp);

    Waveform(std::vector<input_type> amp, std::vector<input_type> t);

    Waveform(std::vector<input_type> amp);

    Waveform(input_type amp[], input_type t[]);

    Waveform(input_type amp[]);

    Waveform(TTreeReaderArray<input_type> *amp, TTreeReaderArray<input_type> *t);

    Waveform(TTreeReaderArray<input_type> *amp, TTreeReaderArray<input_type> *t,
      const bool &invert, const float &ampFactor, const float &tFactor,
      const bool &tResample, const float &xori, const float &dt
    );

    int size(){ return this->v1_.size(); }

    std::vector<data_type> v1(){ return this->v1_; }
    std::vector<data_type> v2(){ return this->v2_; }

    const std::vector<data_type> &get_v1() const { return this->v1_; }
    const std::vector<data_type> &get_v2() const { return this->v2_; }

    std::vector<data_type> &get_v1() { return this->v1_; }
    std::vector<data_type> &get_v2() { return this->v2_; }

    data_type get_v1_value(int index){ return this->v1_.at(index); }
    data_type get_v2_value(int index){ return this->v2_.at(index); }

    void set_v1_value(const int &index, const data_type &value){ this->v1_.at(index) = value; }
    void set_v2_value(const int &index, const data_type &value){ this->v2_.at(index) = value; }

    void set_v1(std::vector<data_type> value){ this->v1_ = value; }
    void set_v2(std::vector<data_type> value){ this->v2_ = value; }

    ClassDef(Waveform, 1)
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(std::vector<input_type> *amp, std::vector<input_type> *t)
{
    if (amp->size() != t->size())
    {
        throw false;
    }

    this->v1_.reserve(2000);
    this->v2_.reserve(2000);

    for (std::size_t i = 0, max = amp->size(); i < max; i++)
    {
        this->v1_.emplace_back(t->at(i));
        this->v2_.emplace_back(amp->at(i));
    }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(std::vector<input_type> amp, std::vector<input_type> t)
{
  if (amp.size() != t.size())
  {
      throw false;
  }

  this->v1_.reserve(2000);
  this->v2_.reserve(2000);

  for (std::size_t i = 0, max = amp.size(); i < max; i++)
  {
      this->v1_.emplace_back(t.at(i));
      this->v2_.emplace_back(amp.at(i));
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(std::vector<input_type> *amp)
{
  this->v1_.reserve(2000);
  this->v2_.reserve(2000);
  for (std::size_t i = 0, max = amp->size(); i < max; i++)
  {
      this->v1_.emplace_back(i);
      this->v2_.emplace_back(amp->at(i));
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(std::vector<input_type> amp)
{
  this->v1_.reserve(2000);
  this->v2_.reserve(2000);
  for (std::size_t i = 0, max = amp.size(); i < max; i++)
  {
      this->v1_.emplace_back(i);
      this->v2_.emplace_back(amp.at(i));
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(input_type amp[])
{
  this->v1_.reserve(2000);
  this->v2_.reserve(2000);
  for (std::size_t i = 0, max = sizeof(amp) / sizeof(amp[0]); i < max; i++)
  {
      this->v1_.emplace_back(i);
      this->v2_.emplace_back(amp[i]);
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform( TTreeReaderArray<input_type> *amp, TTreeReaderArray<input_type> *t )
{
  this->v1_.reserve(2000);
  this->v2_.reserve(2000);
  for(std::size_t i = 0, max = amp->GetSize(); i < max; i++)
  {
      this->v1_.emplace_back( t->At(i) );
      this->v2_.emplace_back( amp->At(i) );
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform(
  TTreeReaderArray<input_type> *amp, TTreeReaderArray<input_type> *t,
  const bool &invert, const float &ampFactor, const float &tFactor,
  const bool &tResample, const float &xori, const float &dt
)
{
  this->v1_.reserve(2000);
  this->v2_.reserve(2000);
  for(std::size_t i = 0, max = amp->GetSize(); i < max; i++)
  {
      tResample ? this->v1_.emplace_back((xori+i*dt)*tFactor) : this->v1_.emplace_back(t->At(i)*tFactor);
      invert ? this->v2_.emplace_back(-1.0*amp->At(i)*ampFactor) : this->v2_.emplace_back(amp->At(i)*ampFactor);
  }

};

#endif // WAVEFORM_H
