#include <scheduler.h>

int
main (int argc, char* argv[])
{
  miniscsim::Scheduler Sch;

  Sch.Run ();
  return 0;
}
