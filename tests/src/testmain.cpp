
#include <iostream>
#include <scheduler.h>
#include <event.h>

using namespace std;

class MyEvent : public msim::Event {
public:

  MyEvent (msim::SimTime when);

  void happen () ;
};

MyEvent::MyEvent (msim::SimTime when)
  :Event (when)
{
}

void
MyEvent::happen ()
{
  cout << __PRETTY_FUNCTION__ << " is was time" << endl;
}

int
main (int argc, char* argv[])
{
  msim::Scheduler Sch;
  msim::SimTime  endTime (21);
  MyEvent  myEvent (msim::SimTime (10));
  Sch.addEvent (&myEvent);
  Sch.runUntil (endTime);
  return 0;
}
