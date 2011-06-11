#include <scheduler.h>

int
main (int argc, char* argv[])
{
  miniscsim::Scheduler Sch;

  Sch.run ();
  return 0;
}
