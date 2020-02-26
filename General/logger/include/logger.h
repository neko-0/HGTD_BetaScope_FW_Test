#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <boost/format.hpp>

/*
From stack-overflow:
https://stackoverflow.com/questions/9158150/colored-output-in-c
*/

// the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

class Logger
{
  private:
    int verbosity_ = 0;
    std::string name_ = "Logger";

  public:

    Logger( std::string name, int verbosity = 0 )
      : verbosity_(verbosity)
    {
      name_ = name;
    };
    ~Logger(){};

  void print(
    std::string prefix,
    std::string content,
    std::string contentColor
  )
  {
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::string ct(std::ctime(&current_time));
    std::string date = ct.substr(0, ct.length() - 1);

    std::string output_string = boost::str(boost::format("%1% [%2%] [%3%] [] %4%: %5% %6%") % GREEN % date % name_ % prefix % RESET % content % contentColor);
    std::cout << output_string << std::endl;
  }

  void error(std::string prefix, std::string content){ Logger::print(prefix, content, RED); }

  void info(std::string prefix, std::string content){ Logger::print(prefix, content, CYAN); }

  void warning(std::string prefix, std::string content){ Logger::print(prefix, content, YELLOW); }
};

#endif // LOGGER_H
