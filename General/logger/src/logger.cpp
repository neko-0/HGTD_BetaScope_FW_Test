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

namespace BETA_LOG{
    int LOG_LEVEL = 0;
    int NEW_COUNTER = 0;
    int DELETE_COUNTER = 0;
    int OTHER_NEW_COUNTER = 0;
    int OTHER_DELETE_COUNTER = 0;
    bool NEW_ = false;
    bool DELETE_ = false;
}

BETA_LOG::Logger &BETA_LOG::make_logger(const std::string &name, const int &level)
{
  static BETA_LOG::Logger logger(name, level);
  return logger;
}

void BETA_LOG::Logger::error(
  const std::string &content,
  const std::string &file_name,
  const int &line_num,
  const std::string &func_name
)
{
  std::string current_time = Logger::timestamp();
  std::string colored_cotent = Logger::color_wrap(BETA_LOG::RED, content);
  std::string prefix = Logger::prefix_format(current_time, file_name, line_num, func_name);

  std::cout << prefix << " " << colored_cotent << std::endl;
}

void BETA_LOG::Logger::info(
  const std::string &content,
  const std::string &file_name,
  const int &line_num,
  const std::string &func_name
)
{
  std::string current_time = Logger::timestamp();
  std::string colored_cotent = Logger::color_wrap(BETA_LOG::BOLDCYAN, content);
  std::string prefix = Logger::prefix_format(current_time, file_name, line_num, func_name);

  std::cout << prefix << " " << colored_cotent << std::endl;
}

void BETA_LOG::Logger::warning(
  const std::string &content,
  const std::string &file_name,
  const int &line_num,
  const std::string &func_name
)
{
  std::string current_time = Logger::timestamp();
  std::string colored_cotent = Logger::color_wrap(BETA_LOG::YELLOW, content);
  std::string prefix = Logger::prefix_format(current_time, file_name, line_num, func_name);

  std::cout << prefix << " " << colored_cotent << std::endl;
}

/*
void* operator new(std::size_t sz)
{
  if(BETA_LOG::NEW_){ BETA_LOG::NEW_COUNTER++; BETA_LOG::NEW_=false; }
  else{ BETA_LOG::OTHER_NEW_COUNTER++; }
  void *m = std::malloc(sz);
  return m;
}

void operator delete(void *m)
{
  if(BETA_LOG::DELETE_){ BETA_LOG::DELETE_COUNTER++; BETA_LOG::DELETE_=false; }
  else{ BETA_LOG::OTHER_DELETE_COUNTER++; }
  std::free(m);
}
*/
