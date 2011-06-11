#include <scheduler.h>

int
main (int argc, char* argv[])
{
  miniscsim::Scheduler Sch;
  miniscsim::SimTime  endTime (21);
  Sch.runUntil (endTime);
  return 0;
}
