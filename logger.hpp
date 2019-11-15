#include <iostream>
#include <fstream>
#include <mutex>
#include <vector>
#include <time.h>
#include <stdexcept>

#ifndef __LOGGER_HPP
#define __LOGGER_HPP
/*
C++ header only logging library that can print to terminal and/or a file
depending on config.
Config here is controlled by bool data members console and logfile.
One Logger class object can print to both terminal and logfile.
*/

class Logger{
public:
  // Turn off both by default
  Logger():console(false),logfile(false){}
  ~Logger(){off_logfile();}
  // Option for writing to terminal
  void on_console(){console = true;}
  void off_console(){console = false;}

  // Option for writing to logfile, e.g filename = "log.txt"
  void on_logfile(std::string filename)
  {
    if (stream.is_open())
    {
      std::invalid_argument("Logfile already open, please close"
                            " current one before opening a new one");
    }
    else
    {
      stream.open(filename);
      logfile = true;
    }
  }

  void off_logfile()
  {
    // Stop if mutex is still locked, just a slight safety here i guess.
    std::lock_guard<std::mutex> lock1(mu_stream);
    if (logfile)
    {
      try
      {
    stream.flush();
    stream.close();
    logfile = false;
      }
      catch(std::exception& error)
      {
        std::cerr<<"Woops, error:"<<error.what()<<std::endl;
      }
    }
  }

  /* Despite both pline and pvec printing strings, I have chosen not to overload
  as that would require the caller to resolve which overloaded function
  to pass to std::thread's constructor which itself cannot resolve which one to
  pick. I am currently unable to produce a better solution for this.
  */

  // Print one string
  void pline(const std::string &msg)
  {
    /* Time is obtained first to ensure the same line of code gets printed
    the same timestamp on both console and logfile. Although this decreases
    the actual accuracy of the timestamp.
    */
    std::string start_time = get_time();
    if (console)
    {
      std::lock_guard<std::mutex> lock1(mu_cout);
      std::cout<<start_time<<"| : "<<msg<<std::endl;
    }
    if (logfile)
    {
      std::lock_guard<std::mutex> lock2(mu_stream);
      stream<<start_time<<"| : "<<msg<<std::endl;
    }
  }
  // Print vector of strings
  void pvec(const std::vector<std::string> &msg)
  {
    std::string start_time = get_time();
    if (console)
    {
      std::lock_guard<std::mutex> lock1(mu_cout);
      for (int i = 0; i<msg.size();++i)
      {
        std::cout<<start_time<<"| : "<<msg[i]<<std::endl;
      }
    }
    if (logfile)
    {
      std::lock_guard<std::mutex> lock2(mu_stream);
      for (int i = 0;i<msg.size();++i)
      {
        stream<<start_time<<"| : "<<msg[i]<<std::endl;
      }
    }
  }

protected:
  std::mutex mu_stream;
  std::mutex mu_cout;
  std::ofstream stream;
  bool console;
  bool logfile ;

  std::string get_time()
  {
    /* localtime_r is threadsafe, or atleast using it removed the datarace
    warning from testing with thread sanitizer */
    time_t now = time(0);
    struct tm result;
    struct tm* the_time = localtime_r(&now,&result);
    char buff[20];
    strftime(buff,20,"%x %X",the_time);
    return buff;
  };
};
#endif
