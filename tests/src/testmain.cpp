
#include <iostream>
#include <scheduler.h>
#include <event.h>
#include <version.h>
#include <tagged-data.h>
#include <expect-buffer.h>
#include <debug-log.h>

using namespace std;

typedef msim::TaggedData <double>  DData;

double dnumber (111.1);
msim::DataTagType  etag (33);

class MyEvent : public msim::Event {
public:

  MyEvent (msim::Scheduler * sched, msim::ExpectBuffer & buf);

  void          happen () ;
  msim::Event * copy () const;

private:

  msim::ExpectBuffer & buffer;
};

MyEvent::MyEvent (msim::Scheduler * sched, msim::ExpectBuffer & buf)
  :Event (sched),
   buffer (buf)
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
  MS_TRACE << " time "
       << " for " << id() 
       << " at " << scheduler()->simTime()
       << " next is " << when() 
       << endl;
  dnumber += 1.1;
  DData * ddata = new DData (etag, dnumber);
  MS_TRACE << " buffer available " << buffer.capacity() - buffer.fullCount() 
           << " expecting " << buffer.expectCount (etag)
           << endl;
  MS_TRACE << " data tag " << ddata->tag() 
           << " valid " << ddata->valid () << endl;
  bool accept = buffer.dataToBuffer (ddata);
  MS_TRACE << " accepted " << accept << " data " << ddata->tag() << " "
           << ddata->payload () 
           << endl;
}

int
main (int argc, char* argv[])
{
 {

  cout << " using msim version " << msim::Version::version << endl;

  msim::TaggedData <string> myData;

  msim::ExpectBuffer  myBuf (2);
  MS_TRACE << " myBuf expect count(" << etag << ") " << myBuf.expectCount(etag)
           << endl;
  myBuf.expectData (msim::DataTagType (etag));
  MS_TRACE << " myBuf expect count(" << etag << ") " << myBuf.expectCount(etag)
           << endl;

  cout << " my data default tag " << myData.tag() 
       << " data " << myData.payload() << endl;

  myData.tag = msim::SimpleTaggedData::genTag ();
  myData.payload = string ("hello world 1");
  cout << " my data default loaded " << myData.tag() 
       << " data " << myData.payload() << endl;
  myData.payload.set (string ("hello world 2"));
  cout << " my data default loaded " << myData.tag() 
       << " data " << myData.payload() << endl;

  msim::Scheduler Sch;
  msim::SimTime  endTime (21);
  MyEvent  ev1 (&Sch, myBuf);
  Sch.schedule (ev1, msim::SimTime (10));

  MyEvent ev2 (&Sch, myBuf);
  Sch.schedule (ev2, msim::SimTime (15));

  MyEvent ev3 (&Sch, myBuf);
  Sch.schedule (ev3, msim::SimTime (19));

  Sch.schedule (ev1, msim::SimTime (17)); // should replace the earlier one

  Sch.runUntil (endTime);
  MS_TRACE << " time of last event " << Sch.lastEventTime() << endl;
 }
  MS_LOG << " exiting " << endl;
 return 0;
}
