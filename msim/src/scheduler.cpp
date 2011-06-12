#include "scheduler.h"
#include <iostream>


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

using namespace std;

namespace msim
{

Scheduler::Scheduler ()
  :currentTime (1),
   lastHappenTime (SimTime::tooEarly())
{
  cerr << __PRETTY_FUNCTION__ << " allocated " << endl;
}

SimTime
Scheduler::simTime () const
{
  return currentTime;
}

SimTime
Scheduler::lastEventTime () const
{
  return lastHappenTime;
}

SimTime
Scheduler::dueTime (int eventId) const
{
  EventTimeMap::const_iterator it = eventTimes.find (eventId);
  if (it == eventTimes.end()) {
    return SimTime::tooLate();
  } else {
    return it->second;
  }
}

void
Scheduler::run ()
{
  cerr << __PRETTY_FUNCTION__ << " running " << endl;
  cerr << "                " << currentTime << endl;
  runUntil (SimTime::tooLate());
}

void
Scheduler::runUntil (const SimTime & endTime)
{
  cerr << __PRETTY_FUNCTION__
       << " until " << endTime 
       << " starting at " << currentTime << endl;
  do {
    EventList::iterator it;
    it = eventList.lower_bound (currentTime);
    while (it != eventList.end () && it->first <= endTime) {
      Event* event = it->second;
      if (event) {
        currentTime = eventTimes[event->id()];
        event->happen ();
        lastHappenTime = currentTime;
        eventTimes.erase (event->id());
        eventList.erase (it);
        delete event;
        it = eventList.lower_bound (currentTime);
      } else {
        it++;
      }
    }
  } while (currentTime < endTime && !eventList.empty());
  currentTime = endTime;
  removePastEvents (currentTime);
  cerr << __PRETTY_FUNCTION__ << " done at time " << currentTime << endl;
}


void
Scheduler::removePastEvents (const SimTime & upperLimit)
{
  cerr << __PRETTY_FUNCTION__ << " until " << upperLimit << endl;
  SimTime firstBadTime (upperLimit);
  firstBadTime.advance ();
  cerr << "    keep all from time " << firstBadTime << endl;
  EventList::iterator lastToRemove = eventList.upper_bound (firstBadTime);
  if (lastToRemove != eventList.end()) {
    cerr << "   found some to remove " << endl;
    for (EventList::iterator chase = eventList.begin();
         chase != lastToRemove;
         chase++) {
      cerr << "    try to remove " << chase->second << endl;
      if (chase->second) {
        delete chase->second;
      }
    }
    eventList.erase (eventList.begin(), lastToRemove);
  }
}

void
Scheduler::schedule (const Event & evt, const SimTime & when)
{
  cerr << __PRETTY_FUNCTION__ ;
  cerr << " event " << evt.id() ;
  cerr << " at " << when;
  cerr << endl;
  Event * pEvent = evt.copy ();
  pEvent->scheduler = this;
  EventTimeMap::iterator eit = eventTimes.find (pEvent->id());
  if (eit != eventTimes.end()) {
    cerr << "   event already scheduled, removeing old one "
         << pEvent->id() << " at " << eit->second
         << endl;
    eventList.erase (eit->second, pEvent->id());
  } else {
    cerr << "  event new to schedule " << pEvent->id() << endl;
  }
  eventList.insert (Event::ListPair (when, pEvent));
  eventTimes[pEvent->id()] = when;
  cerr << "     event " << pEvent->id() 
       << " now scheduled at " << pEvent->when() << endl;
  cerr << "   total events list " << eventList.size () << " times " 
       << eventTimes.size () << endl;
}

} // namespace
