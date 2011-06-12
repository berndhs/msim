#include <scheduler.h>

int
main (int argc, char* argv[])
{
  msim::Scheduler Sch;
  msim::SimTime  endTime (21);
  Sch.runUntil (endTime);
  return 0;
}
