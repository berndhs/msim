#ifndef MINISCSIM_SCHEDULER_H
#define MINISCSIM_SCHEDULER_H

using namespace std;

#include "simtime.h"

namespace miniscsim 
{
class Scheduler {

public:

  Scheduler ();
  void Run ();

private:

  SimTime   currentTime;

};

} // namespace


#endif
