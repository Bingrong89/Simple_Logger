#include "gtest/gtest.h"
#include "logger.hpp"

std::string message = "string testing";
std::vector<std::string> vec_message = {"testing","with","vector","string"};
/*
Test on and offing first.
Then proceed to test the 4 four uses of logger:
1. Open console, print string, close console.
2. Open console, print vector of strings, close console.
3. Open logfile, print string, close logfile.
4. Open logfile, print vector of strings, close logfile.
*/
bool on_off_console(Logger* logobj)
{
  try
  {
    logobj->on_console();
    logobj->off_console();
    return true;
  }
  catch(...){return false;}
}

bool on_off_logfile(Logger* logobj)
{
  try
  {
    logobj->on_logfile("on_off_logfile.txt");
    logobj->off_logfile();
    remove("on_off_logfile.txt");
    return true;
  }
  catch(...){return false;}
}

bool test_pline_console(Logger* logobj,std::string msg)
{
  try
  {
    logobj->on_console();
    logobj->pline(msg);
    logobj->off_console();
    return true;
  }
  catch(...){return false;}
}

bool test_pline_logfile(Logger* logobj,std::string msg)
{
  try
  {
    logobj->on_logfile("test_pline_logfile.txt");
    logobj->pline(msg);
    logobj->off_logfile();
    remove("test_pline_logfile.txt");
    return true;
  }
  catch(...){return false;}
}

bool test_pvec_console(Logger* logobj,std::vector<std::string> vec_msg)
{
  try
  {
    logobj->on_console();
    logobj->pvec(vec_msg);
    logobj->off_console();
    return true;
  }
  catch(...){return false;}
}

bool test_pvec_logfile(Logger* logobj,std::vector<std::string> vec_msg)
{
  try
  {
    logobj->on_logfile("test_pvec_logfile.txt");
    logobj->pvec(vec_msg);
    logobj->off_logfile();
    remove("test_pvec_logfile.txt");
    return true;
  }
  catch(...){return false;}
}


TEST(LOGGER_on_off,console)
{
  Logger *logger = new Logger();
  ASSERT_TRUE(on_off_console(logger));
}

TEST(LOGGER_on_off,logfile)
{
  Logger *logger = new Logger();
  ASSERT_TRUE(on_off_logfile(logger));
}

TEST(LOGGER_pline,console)
{
  Logger *logger = new Logger();
  ASSERT_TRUE(test_pline_console(logger,message));
}

TEST(LOGGER_pline,logfile)
{
  Logger *logger = new Logger();
  ASSERT_TRUE(test_pline_logfile(logger,message));
}

TEST(LOGGER_pvec,console)
{
  Logger *logger = new Logger();
  ASSERT_TRUE(test_pvec_console(logger,vec_message));
}

TEST(LOGGER_pvec,logfile)
{
  Logger *logger = new Logger();
  ASSERT_TRUE(test_pvec_logfile(logger,vec_message));
}

int main(int argc, char** argv) {
  std::cout.rdbuf(nullptr); //suppress cout to keep screen clean
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
