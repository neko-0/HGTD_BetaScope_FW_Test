#include "logger/include/logger.h"

/*
#ifndef LOG_INFO
#define LOG_INFO(message) BETA_LOG::make_logger(__FILE__, 0)(std::string(message), __FILE__, __LINE__, __func__);
#endif

#ifndef LOG_WARNING
#define LOG_WARNING(message) BETA_LOG::make_logger(__FILE__, 1)(std::string(message), __FILE__, __LINE__, __func__);
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(message) BETA_LOG::make_logger(__FILE__, 2)(std::string(message), __FILE__, __LINE__, __func__);
#endif
*/

//#define LOG_INFO(message) BETA_LOG::make_logger(std::string(__FILE__), 0)(std::string(message), __FILE__, __LINE__, __func__);
//#define LOG_WARNING(message) BETA_LOG::make_logger(std::string(__FILE__), 1)(std::string(message), __FILE__, __LINE__, __func__);
//#define LOG_ERROR(message) BETA_LOG::make_logger(std::string(__FILE__), 2)(std::string(message), __FILE__, __LINE__, __func__);


BETA_LOG::Logger &BETA_LOG::make_logger(std::string name, int level)
{
  static BETA_LOG::Logger logger(name, level);
  return logger;
}

void BETA_LOG::Logger::error(
  std::string content,
  std::string file_name,
  int line_num,
  std::string func_name
)
{
  std::string current_time = Logger::timestamp();
  std::string colored_cotent = Logger::color_wrap(BETA_LOG::RED, content);
  std::string prefix = Logger::prefix_format(current_time, file_name, line_num, func_name);

  std::cout << prefix << " " << colored_cotent << std::endl;
}

void BETA_LOG::Logger::info(
  std::string content,
  std::string file_name,
  int line_num,
  std::string func_name
)
{
  std::string current_time = Logger::timestamp();
  std::string colored_cotent = Logger::color_wrap(BETA_LOG::BOLDCYAN, content);
  std::string prefix = Logger::prefix_format(current_time, file_name, line_num, func_name);

  std::cout << prefix << " " << colored_cotent << std::endl;
}

void BETA_LOG::Logger::warning(
  std::string content,
  std::string file_name,
  int line_num,
  std::string func_name
)
{
  std::string current_time = Logger::timestamp();
  std::string colored_cotent = Logger::color_wrap(BETA_LOG::YELLOW, content);
  std::string prefix = Logger::prefix_format(current_time, file_name, line_num, func_name);

  std::cout << prefix << " " << colored_cotent << std::endl;
}
