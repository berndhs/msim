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
Scheduler::Run ()
{
  cout << __PRETTY_FUNCTION__ << " running " << endl;
  cout << "                " << currentTime << endl;
}

} // namespace
