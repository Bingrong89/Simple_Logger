#include <pthread.h>
#include "logger.hpp"

// Test for data race to ensure thread safety.
/* Construct class to hold two data members, because pthread_create
can only accept one argument for the function to be executed.
Because the aim here is to test a member function, class object's this
and the target member function's argument are to be wrapped here.
*/
struct pline_dummy
{
  pline_dummy(Logger& log, std::string& msg)
  {
    loggo = &log;
    message = &msg;
  }
  Logger* loggo;
  std::string* message;
};

struct pvec_dummy
{
  pvec_dummy(Logger& log, std::vector<std::string>& msg)
  {
    loggo = &log;
    message = &msg;
  }
  Logger* loggo;
  std::vector<std::string>* message;
};

// This follows the signature for pthread_create's constructor.
static void* execute_pline(void* dummy)
{
    pline_dummy* hptr = (pline_dummy*)dummy;
    Logger* cptr =  hptr->loggo;
    cptr->pline(*hptr->message);
    return NULL;
}

static void* execute_pvec(void* dummy)
{
    pvec_dummy* hptr = (pvec_dummy*)dummy;
    Logger* cptr =  hptr->loggo;
    cptr->pvec(*hptr->message);
    return NULL;
}

int main(){
  pthread_t t1;
  pthread_t t2;
  pthread_t t3;
  pthread_t t4;
  Logger logger;
  logger.on_console();
  logger.on_logfile("threadtestcpp.txt");
  std::string msg1 = "t1 printing string";
  std::string msg2 = "t2 printing string";
  std::vector<std::string> msgvec1 = {"t3","t3","t3","t3","t3"};
  std::vector<std::string> msgvec2 = {"t4","t4","t4","t4","t4"};
  pline_dummy dummy1(logger, msg1);
  pline_dummy dummy2(logger,msg2);
  pvec_dummy dummy3(logger,msgvec1);
  pvec_dummy dummy4(logger,msgvec2);

  pthread_create(&t1,NULL,execute_pline,&dummy1);
  pthread_create(&t2,NULL,execute_pline,&dummy2);
  pthread_create(&t3,NULL,execute_pvec,&dummy3);
  pthread_create(&t4,NULL,execute_pvec,&dummy4);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  pthread_join(t3,NULL);
  pthread_join(t4,NULL);
  remove("threadtestcpp.txt");
}
