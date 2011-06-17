#include "scheduler.h"
#include <iostream>

#include "debug-log.h"


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
  MS_TRACE  << " allocated " << endl;
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
  MS_TRACE ;
  runUntil (SimTime::tooLate());
}

void
Scheduler::runUntil (const SimTime & endTime)
{
  MS_TRACE 
       << " until " << endTime 
       << " starting at " << currentTime << endl;
  do {
    EventList::iterator it;
    it = eventList.lower_bound (currentTime);
    while (it != eventList.end () && it->first <= endTime) {
      Event* event = it->second;
      MS_TRACE << " time " << it->first << " event " << hex << event << dec << endl;
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
  MS_TRACE  << " done at time " << currentTime << endl;
}


void
Scheduler::removePastEvents (const SimTime & upperLimit)
{
  MS_TRACE  << " until " << upperLimit << endl;
  SimTime firstBadTime (upperLimit + 1);
  EventList::iterator lastToRemove = eventList.upper_bound (firstBadTime);
  if (lastToRemove != eventList.end()) {
    MS_TRACE << "   found some to remove " << endl;
    for (EventList::iterator chase = eventList.begin();
         chase != lastToRemove;
         chase++) {
      MS_TRACE << "    try to remove " << chase->second << endl;
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
  MS_TRACE << endl << "  event " << evt.id() << endl;
  MS_TRACE << endl << "        at " << when << endl;
  Event * pEvent = evt.copy ();
  pEvent->scheduler = this;
  EventTimeMap::iterator eit = eventTimes.find (pEvent->id());
  if (eit != eventTimes.end()) {
    MS_TRACE << "   event already scheduled, removeing old one "
         << pEvent->id() << " at " << eit->second
         << endl;
    eventList.erase (eit->second, pEvent->id());
  }
  eventList.insert (Event::ListPair (when, pEvent));
  eventTimes[pEvent->id()] = when;
  MS_TRACE << " done " << endl << "     event " << pEvent->id() 
       << " now scheduled at " << pEvent->when() << endl;
}

} // namespace
