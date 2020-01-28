/*
  Class for waveform
*/
#include <vector>
#include <string>

template <class data_type, class input_type>
class Waveform
{
  protected:

    std::vector<data_type> v1_ = {};
    std::vector<data_type> v2_ = {};

  public:

    Waveform(){};
    ~Waveform(){};

    Waveform( std::vector<input_type> *amp, std::vector<input_type> *t);

    Waveform( std::vector<input_type> *amp);

    Waveform( std::vector<input_type> amp, std::vector<input_type> t);

    Waveform( std::vector<input_type> amp);

    Waveform( input_type amp[], input_type t[] );

    Waveform( input_type amp[]);

    std::vector<data_type> get_v1(){return this->v1_;}
    std::vector<data_type> get_v2(){return this->v2_;}

    void set_v1(int index, data_type value){this->v1_.at(index) = value;}
    void set_v2(int index, data_type value){this->v2_.at(index) = value;}

};


template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform( std::vector<input_type> *amp, std::vector<input_type> *t)
{
  if(amp->size() != t->size())
  {
    throw false;
  }

  for(std::size_t i = 0, max = amp->size(); i < max; i++)
  {
    this->v1_.push_back( t->at(i) );
    this->v2_.push_back( amp->at(i) );
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform( std::vector<input_type> amp, std::vector<input_type> t)
{
  if(amp.size() != t.size())
  {
    throw false;
  }

  for(std::size_t i = 0, max = amp.size(); i < max; i++)
  {
    this->v1_.push_back( t.at(i) );
    this->v2_.push_back( amp.at(i) );
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform( std::vector<input_type> *amp)
{
  for(std::size_t i = 0, max = amp->size(); i < max; i++)
  {
    this->v1_.push_back(i);
    this->v2_.push_back( amp->at(i) );
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform( std::vector<input_type> amp)
{
  for(std::size_t i = 0, max = amp.size(); i < max; i++)
  {
    this->v1_.push_back(i);
    this->v2_.push_back( amp.at(i) );
  }
};

template <class data_type, class input_type>
Waveform<data_type, input_type>::Waveform( input_type amp[])
{
  for(std::size_t i = 0, max = amp.size(); i < max; i++)
  {
    this->v1_.push_back(i);
    this->v2_.push_back( amp.at(i) );
  }
};
