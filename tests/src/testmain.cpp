

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/


#include <iostream>
#if TEST_SOURCE
#include "../../msim/src/scheduler.h"
#include "../../msim/src/event.h"
#include "../../msim/src/version.h"
#include "../../msim/src/tagged-data.h"
#include "../../msim/src/expect-buffer.h"
#include "../../msim/src/debug-log.h"
#include "../../msim/src/connector.h"
#include "../../msim/src/tag-matcher.h"
#include "../../msim/src/data-consumer.h"
#endif
#if TEST_INSTALLED
#include <msim/scheduler.h>
#include <msim/event.h>
#include <msim/version.h>
#include <msim/tagged-data.h>
#include <msim/expect-buffer.h>
#include <msim/debug-log.h>
#include <msim/connector.h>
#include <msim/tag-matcher.h>
#include <msim/data-consumer.h>
#endif

using namespace std;
using namespace msim;

typedef TaggedData <double>  DData;

double dnumber (111.1);
DataTagType  etag (SimpleTaggedData::genTag());

class MyEvent : public Event {
public:

  MyEvent (Scheduler * sched, ExpectBuffer & buf);

  void          happen () ;
  Event * copy () const;

private:

  ExpectBuffer & buffer;
};

MyEvent::MyEvent (Scheduler * sched, ExpectBuffer & buf)
  :Event (sched),
   buffer (buf)
{
}

Event *
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
  bool accept = buffer.writeToBuffer (ddata);
  MS_TRACE << " accepted " << accept << " data " << ddata->tag() << " "
           << ddata->payload () 
           << endl;
}


class MyBuf : public ExpectBuffer {
public:

  MyBuf ()
    :ExpectBuffer (2)
  { 
    MS_TRACE << endl;
  }

  bool writeToBuffer (SimpleTaggedDataPtr  pData)
  {
    MS_TRACE << endl;
    bool ok = ExpectBuffer::writeToBuffer (pData);
    MS_TRACE << " adding data " << ok << endl;
    return ok;
  }

};

class ReadEvent : public Event
{
public:

  ReadEvent (Scheduler * sched, Connector * conn)
    :Event (sched),
     connector (conn)
  {}

  void happen ();
  Event  * copy () const;
  
private:

  Connector  *connector;
};

Event * 
ReadEvent::copy () const
{
  ReadEvent * cp = new ReadEvent (scheduler(), connector);
  return cp;
}

void
ReadEvent::happen ()
{
  MS_TRACE << " event id " << id() << endl;
  MS_TRACE << " at " << scheduler()->simTime() << endl;
  MS_TRACE << " scheduler " << hex << scheduler() << dec << endl;
  if (connector == 0) {
    return;
  }
  MS_TRACE << " available " << connector->isReadAvailable (1) << endl;
  MS_TRACE << "    available time (0) " 
           << connector->whenReadAvailable (0) << endl;
  MS_TRACE << "    available time (1) " 
           << connector->whenReadAvailable (1) << endl;
  if (connector->isReadAvailable (1)) {
    SimpleTaggedDataPtr pD = connector->read (1);
    MS_TRACE << "  got data " << hex << pD << dec << endl;
    MS_TRACE << "  now available " << connector->isReadAvailable (1) << endl;
    MS_TRACE << "    now available time (1) " 
           << connector->whenReadAvailable (1) << endl;
  }
}


/*******************************/


Scheduler Sch;
Connector  testCon (&Sch, 3, 2);  // 3 inputs 2 outputs



void
testConnector ()
{
  MS_TRACE << endl;
  testCon.setDelay (-1,-1,200);  // 200 ticks for anything
  DData * testData = new DData (1001.1);
  SimTime whenOk = testCon.write (0,1,testData);
  MS_TRACE << " wrote sould be 1 is " << whenOk.current() << endl;
  whenOk = testCon.write (6,7,testData);
  MS_TRACE << " wrote sould be 0 is " << whenOk.current() << endl;
  ReadEvent  re100 (&Sch, &testCon);
  ReadEvent  re200 (&Sch, &testCon);
  SimTime now (Sch.simTime());
  MS_TRACE << " now is " << now << endl;
  SimTime  readTime1 (now + 100);
  SimTime  readTime2 (now + 300);
  MS_TRACE << " check 1 " << now + 100 << " check 2 " << now + 200 << endl;
  Sch.schedule (re100, readTime1);
  Sch.schedule (re200, readTime2);
  MS_TRACE << __LINE__ << endl;
}

DataTagType  testMatchTag (55);

class MatchEvent: public Event 
{
public:

  MatchEvent (Scheduler *sch, Connector * conn)
    :Event (sch),
     connector (conn)
  {}

  Event * copy () const
  {
    return new MatchEvent (scheduler(), connector);
  }

  void happen () 
  {
    MS_TRACE << " match-event id " << id() << endl;
    MS_TRACE << " at " << scheduler()->simTime() << endl;
    MS_TRACE << " scheduler " << hex << scheduler() << dec << endl;
    TaggedData<double>  * pD 
      = new TaggedData<double> (testMatchTag, 5.5);
    connector->write (0,0, pD);  
  }

private:
  Connector * connector;
};

/*********************************/

DataConsumer  consumer (1);
TagMatcher    matcher;
Connector     matchConn (&Sch,1,1);

void
testConsumer ()
{
  MS_TRACE << endl;
  matcher.registerClient (consumer.port(0), testMatchTag, 
         TagDuration::Once);
  matchConn.registerClient (0, &matcher);
  consumer.expectTag (0, testMatchTag);
  matchConn.setDelay (0,0,50);
  MS_TRACE << " for match events, sched  at " << hex << &Sch 
          << " matcher at " << &matcher << dec << endl;
  MatchEvent  mEv1 (&Sch, &matchConn);
  MatchEvent  mEv2 (&Sch, &matchConn);
  SimTime now = Sch.simTime ();
  MS_TRACE << " schedule event " << mEv1.id () << endl;
  Sch.schedule (mEv1, now + 1000);
  MS_TRACE << " schedule event " << mEv2.id () << endl;
  Sch.schedule (mEv2, now + 2000);
}


int
main (int argc, char* argv[])
{
 {

  cout << " using msim version " << Version::version << endl;
  cout << " quiet ? " << DebugLog::isQuiet () << endl;
  MS_DEBUG_ON ;


  MyBuf  myBuf;
  MS_TRACE << " myBuf expect count(" << etag << ") " << myBuf.expectCount(etag)
           << endl;
  myBuf.expectData (DataTagType (etag));
  MS_TRACE << " myBuf expect count(" << etag << ") " << myBuf.expectCount(etag)
           << endl;

  MS_TRACE << " turning off messags " << endl;
  cout << " quiet ? " << DebugLog::isQuiet () << endl;
  
  SimTime  endTime (21);
  MyEvent  ev1 (&Sch, myBuf);
  Sch.schedule (ev1, SimTime (10));

  MyEvent ev2 (&Sch, myBuf);
  Sch.schedule (ev2, SimTime (15));

  MyEvent ev3 (&Sch, myBuf);
  Sch.schedule (ev3, SimTime (19));

  Sch.schedule (ev1, SimTime (17)); // should replace the earlier one

  testConnector ();

  testConsumer ();

  //Sch.runUntil (endTime);
  Sch.run ();
  MS_TRACE << " time of last event " << Sch.lastEventTime() << endl;
 }
  MS_LOG << " exiting " << endl;
  cout << " using msim version " << Version::version << endl;
  cout << " quiet ? " << DebugLog::isQuiet () << endl;
 return 0;
}
