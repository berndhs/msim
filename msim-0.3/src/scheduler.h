#ifndef MINISCSIM_SCHEDULER_H
#define MINISCSIM_SCHEDULER_H

using namespace std;

#include "simtime.h"

namespace miniscsim 
{
class Scheduler {

public:

  Scheduler ();
  void run ();
  void runUntil (const SimTime & endTime);

private:

  void step ();

  SimTime   currentTime;

};

} // namespace


#endif
