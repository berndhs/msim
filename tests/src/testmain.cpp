
#include <iostream>
#include <scheduler.h>
#include <event.h>
#include <version.h>
#include <tagged-data.h>
#include <debug-log.h>

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
  MS_LOG << __PRETTY_FUNCTION__ << " time "
       << " for " << id() 
       << " at " << scheduler()->simTime()
       << " next is " << when() 
       << endl;
}

int
main (int argc, char* argv[])
{
 {

  cout << " using msim version " << msim::Version::version << endl;

  msim::TaggedData <std::string> myData;

  cout << " my data default tag " << myData.tag() 
       << " data " << myData.payload() << endl;

  myData.tag = msim::SimpleTaggedData::genTag ();
  myData.payload = std::string ("hello world 1");
  cout << " my data default loaded " << myData.tag() 
       << " data " << myData.payload() << endl;
  myData.payload.set (std::string ("hello world 2"));
  cout << " my data default loaded " << myData.tag() 
       << " data " << myData.payload() << endl;

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
  MS_TRACE << " time of last event " << Sch.lastEventTime() << endl;
 }
  MS_LOG << " exiting " << endl;
 return 0;
}
