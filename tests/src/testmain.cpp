
#include <iostream>
#include <scheduler.h>
#include <event.h>

using namespace std;

class MyEvent : public msim::Event {
public:

  MyEvent (msim::Scheduler * sched);

  void          happen () ;
  msim::Event * copy () const;
};

MyEvent::MyEvent (msim::Scheduler * sched)
  :Event (sched)
{
}

msim::Event *
MyEvent::copy () const
{
  MyEvent * cp = new MyEvent (scheduler());
  return cp;
}

void
MyEvent::happen ()
{
  cerr << __PRETTY_FUNCTION__ << " is was time at " 
       << scheduler()->simTime()
       << " next is " << when() 
       << endl;
}

int
main (int argc, char* argv[])
{
  msim::Scheduler Sch;
  msim::SimTime  endTime (21);
  MyEvent  myEvent (&Sch);
  Sch.schedule (myEvent, msim::SimTime (10));
  Sch.runUntil (endTime);
  cerr << " time of last event " << Sch.lastEventTime() << endl;
  return 0;
}
