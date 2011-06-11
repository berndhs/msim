#include "scheduler.h"
#include <iostream>

using namespace std;

namespace miniscsim
{

Scheduler::Scheduler ()
{
  cout << __PRETTY_FUNCTION__ << " allocated " << endl;
}

void
Scheduler::Run ()
{
  cout << __PRETTY_FUNCTION__ << " running " << endl;
}

} // namespace
