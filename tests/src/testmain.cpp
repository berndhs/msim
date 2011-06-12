
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
  MyEvent * cp = new MyEvent (*this);
  return cp;
}

void
MyEvent::happen ()
{
  cerr << __PRETTY_FUNCTION__ << " time "
       << " for " << id() 
       << " at " << scheduler()->simTime()
       << " next is " << when() 
       << endl;
}

int
main (int argc, char* argv[])
{
  msim::Scheduler Sch;
  msim::SimTime  endTime (21);
  MyEvent  ev1 (&Sch);
  Sch.schedule (ev1, msim::SimTime (10));

  MyEvent ev2 (&Sch);
  Sch.schedule (ev2, msim::SimTime (15));

  MyEvent ev3 (&Sch);
  Sch.schedule (ev3, msim::SimTime (19));

  Sch.schedule (ev1, msim::SimTime (17)); // should replace the earlier one

  Sch.runUntil (endTime);
  cerr << " time of last event " << Sch.lastEventTime() << endl;
  return 0;
}
