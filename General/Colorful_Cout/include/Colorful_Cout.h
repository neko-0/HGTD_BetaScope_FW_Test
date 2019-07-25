#ifndef COLORFUL_COUT_H
#define COLORFUL_COUT_H

#include <string>
#include <iostream>
#include <ctime>
#include <chrono>

/*
From stack-overflow:
https://stackoverflow.com/questions/9158150/colored-output-in-c
*/

//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

class ColorCout
{
  ColorCout(){};
  ~ColorCout(){};

public:
  static void print(std::string prefix, std::string content, std::string contentColor )
  {
    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::string ct(std::ctime(&current_time));
    std::string date = ct.substr(0,ct.length()-1);

    std::cout <<"[" << date << "]" << "   " << GREEN << prefix << RESET << contentColor << content << RESET << std::endl;
  }
};

#endif //COLORFUL_COUT_H
