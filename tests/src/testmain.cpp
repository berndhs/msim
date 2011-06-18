

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
#include <scheduler.h>
#include <event.h>
#include <version.h>
#include <tagged-data.h>
#include <expect-buffer.h>
#include <debug-log.h>
#include <connector.h>

using namespace std;

typedef msim::TaggedData <double>  DData;

double dnumber (111.1);
msim::DataTagType  etag (msim::SimpleTaggedData::genTag());

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
  bool accept = buffer.writeToBuffer (ddata);
  MS_TRACE << " accepted " << accept << " data " << ddata->tag() << " "
           << ddata->payload () 
           << endl;
}


class MyBuf : public msim::ExpectBuffer {
public:

  MyBuf ()
    :ExpectBuffer (2)
  { 
    MS_TRACE << endl;
  }

  bool writeToBuffer (msim::SimpleTaggedDataPtr  pData)
  {
    MS_TRACE << endl;
    bool ok = ExpectBuffer::writeToBuffer (pData);
    MS_TRACE << " adding data " << ok << endl;
    return ok;
  }

};

class ReadEvent : public msim::Event
{
public:

  ReadEvent (msim::Scheduler * sched, msim::Connector * conn)
    :Event (sched),
     connector (conn)
  {}

  void happen ();
  msim::Event  * copy () const;
  
private:

  msim::Connector  *connector;
};

msim::Event * 
ReadEvent::copy () const
{
  ReadEvent * cp = new ReadEvent (scheduler(), connector);
  return cp;
}

void
ReadEvent::happen ()
{
  MS_TRACE << " at " << scheduler()->simTime() << endl;
  if (connector == 0) {
    return;
  }
  MS_TRACE << " available " << connector->isReadAvailable (1) << endl;
  MS_TRACE << "    available time (0) " 
           << connector->whenReadAvailable (0) << endl;
  MS_TRACE << "    available time (1) " 
           << connector->whenReadAvailable (1) << endl;
  if (connector->isReadAvailable (1)) {
    msim::SimpleTaggedDataPtr pD = connector->read (1);
    MS_TRACE << "  got data " << hex << pD << endl;
    MS_TRACE << "  now available " << connector->isReadAvailable (1) << endl;
    MS_TRACE << "    now available time (1) " 
           << connector->whenReadAvailable (1) << endl;
  }
}


msim::Scheduler Sch;
msim::Connector  testCon (&Sch, 3, 2);  // 3 inputs 2 outputs

void
testConnector ()
{
  MS_TRACE << endl;
  testCon.setDelay (-1,-1,200);  // 200 ticks for anything
  DData * testData = new DData (1001.1);
  msim::SimTime whenOk = testCon.write (0,1,testData);
  MS_TRACE << " wrote " << whenOk << endl;
  ReadEvent  re100 (&Sch, &testCon);
  ReadEvent  re200 (&Sch, &testCon);
  msim::SimTime now (Sch.simTime());
  MS_TRACE << " now is " << now << endl;
  msim::SimTime  readTime1 (now + 100);
  msim::SimTime  readTime2 (now + 300);
  MS_TRACE << " check 1 " << now + 100 << " check 2 " << now + 200 << endl;
  Sch.schedule (re100, readTime1);
  Sch.schedule (re200, readTime2);
  MS_TRACE << __LINE__ << endl;
}

int
main (int argc, char* argv[])
{
 {

  cout << " using msim version " << msim::Version::version << endl;


  MyBuf  myBuf;
  MS_TRACE << " myBuf expect count(" << etag << ") " << myBuf.expectCount(etag)
           << endl;
  myBuf.expectData (msim::DataTagType (etag));
  MS_TRACE << " myBuf expect count(" << etag << ") " << myBuf.expectCount(etag)
           << endl;

  msim::SimTime  endTime (21);
  MyEvent  ev1 (&Sch, myBuf);
  Sch.schedule (ev1, msim::SimTime (10));

  MyEvent ev2 (&Sch, myBuf);
  Sch.schedule (ev2, msim::SimTime (15));

  MyEvent ev3 (&Sch, myBuf);
  Sch.schedule (ev3, msim::SimTime (19));

  Sch.schedule (ev1, msim::SimTime (17)); // should replace the earlier one

  testConnector ();

  //Sch.runUntil (endTime);
  Sch.run ();
  MS_TRACE << " time of last event " << Sch.lastEventTime() << endl;
 }
  MS_LOG << " exiting " << endl;
 return 0;
}
