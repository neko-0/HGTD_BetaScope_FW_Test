/*
  Class for waveform
*/
#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <TObject.h>
#include <TTreeReaderArray.h>

#include <iostream>
#include <string>
#include <vector>

class WaveformBase : public TObject
{
public:
  WaveformBase(){};
  virtual ~WaveformBase(){};
};

template <class data_type, class input_type> class Waveform : public WaveformBase
{
protected:
    std::vector<data_type> v1_ = {};
    std::vector<data_type> v2_ = {};
    std::size_t size_ = 0;

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

    virtual std::ostream &print(std::ostream &os) const;

    template <class T1, class T2>
    friend std::ostream &operator<<(std::ostream &os, Waveform<T1,T2> const &m);

    int size(){ return this->v1_.size(); }

    std::vector<data_type> v1() const { return this->v1_; }
    std::vector<data_type> v2() const { return this->v2_; }

    //const std::vector<data_type> &v1() const { return this->v1_; }
    //const std::vector<data_type> &v2() const { return this->v2_; }

    const std::vector<data_type> &get_v1() const { return this->v1_; }
    const std::vector<data_type> &get_v2() const { return this->v2_; }

    std::vector<data_type> &get_v1() { return this->v1_; }
    std::vector<data_type> &get_v2() { return this->v2_; }

    data_type get_v1_value(int index){ return this->v1_.at(index); }
    data_type get_v2_value(int index){ return this->v2_.at(index); }

    void set_v1_value(const int &index, const data_type &value){ this->v1_.at(index) = value; }
    void set_v2_value(const int &index, const data_type &value){ this->v2_.at(index) = value; }

    void set_v1(std::vector<data_type> value){ this->v1_ = std::move(value); }
    void set_v2(std::vector<data_type> value){ this->v2_ = std::move(value); }

    std::size_t size() const {return this->v1_.size();}

    Waveform<data_type,data_type> sub_waveform(const int &start, const int &end);

    Waveform<data_type,data_type> derivative();

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

  this->size_ = amp->GetSize();
  if(this->size_ == 0)
  {
    for( int i =0; i < 100; i++ )
    {
      this->v1_.emplace_back(-10e10);
      this->v2_.emplace_back(-10e10);
    }
  }
};

template <class data_type, class input_type>
Waveform<data_type,data_type> Waveform<data_type,input_type>::sub_waveform(const int &start, const int &end)
{
  Waveform<data_type,data_type> buffer = Waveform<data_type,data_type>();
  buffer.get_v1() = std::vector<data_type>(this->v1_.begin()+start, this->v1_.begin()+end);
  buffer.get_v2() = std::vector<data_type>(this->v2_.begin()+start, this->v2_.begin()+end);
  return buffer;
}

template <class data_type, class input_type>
Waveform<data_type,data_type> Waveform<data_type,input_type>::derivative()
{
  Waveform<data_type,data_type> buffer = Waveform<data_type,data_type>();
  for(std::size_t i = 0, max = this->v1_.size()-1; i < max; i++)
  {
    buffer.get_v1().push_back( this->v1_.at(i) );
    buffer.get_v2().push_back( (this->v2_.at(i+1)-this->v2_.at(i))/(this->v1_.at(i+1)-this->v1_.at(i)) );
  }
  return buffer;
}

template <class data_type, class input_type>
std::ostream &Waveform<data_type,input_type>::print(std::ostream &os) const
{
  os << "v1: [";
  for(auto &value : this->get_v1() ){ os << value << ","; }
  os << "]\n";
  os << "v2: [";
  for(auto &value : this->get_v2() ){ os << value << ","; }
  os << "]";
  return os;
}

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, const Waveform<T1,T2> &m)
{
  return m.print(os);
}



#endif // WAVEFORM_H
