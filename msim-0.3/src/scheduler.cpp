#include "scheduler.h"
#include <iostream>

using namespace std;

namespace miniscsim
{

Scheduler::Scheduler ()
  :currentTime (1)
{
  cout << __PRETTY_FUNCTION__ << " allocated " << endl;
}

void
Scheduler::run ()
{
  cout << __PRETTY_FUNCTION__ << " running " << endl;
  cout << "                " << currentTime << endl;
  step ();
}

void
Scheduler::step ()
{
  cout << __PRETTY_FUNCTION__ << endl;
  currentTime +=1;
  cout << "              " << currentTime << endl;
}

} // namespace
