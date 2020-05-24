#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <boost/format.hpp>


namespace BETA_LOG
{
  /*
  From stack-overflow:
  https://stackoverflow.com/questions/9158150/colored-output-in-c
  */

  // the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
  static const std::string RESET =  "\033[0m";
  static const std::string BLACK =  "\033[30m";              /* Black */
  static const std::string RED =  "\033[31m";                /* Red */
  static const std::string GREEN =  "\033[32m";              /* Green */
  static const std::string YELLOW =  "\033[33m";             /* Yellow */
  static const std::string BLUE =  "\033[34m";               /* Blue */
  static const std::string MAGENTA =  "\033[35m";            /* Magenta */
  static const std::string CYAN =  "\033[36m";               /* Cyan */
  static const std::string WHITE =  "\033[37m";              /* White */
  static const std::string BOLDBLACK =  "\033[1m\033[30m";   /* Bold Black */
  static const std::string BOLDRED =  "\033[1m\033[31m";     /* Bold Red */
  static const std::string BOLDGREEN =  "\033[1m\033[32m";   /* Bold Green */
  static const std::string BOLDYELLOW = "\033[1m\033[33m";  /* Bold Yellow */
  static const std::string BOLDBLUE =  "\033[1m\033[34m";    /* Bold Blue */
  static const std::string BOLDMAGENTA =  "\033[1m\033[35m"; /* Bold Magenta */
  static const std::string BOLDCYAN =  "\033[1m\033[36m";    /* Bold Cyan */
  static const std::string BOLDWHITE =  "\033[1m\033[37m";   /* Bold White */

  extern int LOG_LEVEL;

  class Logger
  {
    private:
      int level_ = 0;
      std::string name_ = "Logger";
      int line_counter_ = 0;

      std::string color_wrap( std::string color, std::string content){ return boost::str(boost::format("%1% %2% %3%")%color%content%BETA_LOG::RESET); }
      std::string timestamp()
      {
        auto now = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);
        std::string ct(std::ctime(&current_time));
        std::string date = ct.substr(0, ct.length() - 1);
        return date;
      }

      std::string prefix_format(
        const std::string &timestamp,
        std::string file_name,
        const int &line_num,
        const std::string &func_name
      )
      {
        std::string output_file_name = file_name;
        std::string deli = "/";
        while(output_file_name.find(deli) != std::string::npos )
        {
          output_file_name.erase(0, output_file_name.find(deli)+deli.length() );
        }
        std::string prefix =  boost::str(boost::format("(%1%)%2%[%3%] [%4%] [%5%] [%6%] %7%")
                              %this->line_counter_% BETA_LOG::BOLDGREEN % timestamp % output_file_name % line_num % func_name % BETA_LOG::RESET);

        this->line_counter_++;
        return prefix;
      }


    public:

      Logger( const std::string &name, const int &level = 0 )
        : level_(level)
      {
        name_ = name;
      };
      ~Logger(){};

    void operator()(
      const std::string &content,
      const char* file_name,
      const int &line_num,
      const char* func_name
    )
    {
      if(level_ == 0 && level_ >= BETA_LOG::LOG_LEVEL)
      {
        Logger::info(content, file_name, line_num, func_name);
      }
      else if (level_ == 1 && level_ >= BETA_LOG::LOG_LEVEL )
      {
        Logger::warning(content, file_name, line_num, func_name);
      }
      else if (level_ == 2 && level_ >= BETA_LOG::LOG_LEVEL )
      {
        Logger::error(content, file_name, line_num, func_name);
      }
      else{}
    }

    void error(
      const std::string &content,
      const std::string &file_name,
      const int &line_num,
      const std::string &func_name
    );

    void info(
      const std::string &content,
      const std::string &file_name,
      const int &line_num,
      const std::string &func_name
    );

    void warning(
      const std::string &content,
      const std::string &file_name,
      const int &line_num,
      const std::string &func_name
    );

  };

  BETA_LOG::Logger &make_logger(const std::string &name, const int &level);

}

///*
#define LOG_INFO(message) BETA_LOG::make_logger(std::string(__FILE__), 0)(std::string(message), __FILE__, __LINE__, __func__);
#define LOG_WARNING(message) BETA_LOG::make_logger(std::string(__FILE__), 1)(std::string(message), __FILE__, __LINE__, __func__);
#define LOG_ERROR(message) BETA_LOG::make_logger(std::string(__FILE__), 2)(std::string(message), __FILE__, __LINE__, __func__);
//*/


#endif // LOGGER_H
