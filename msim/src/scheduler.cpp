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
  :currentTime (1)
{
  cout << __PRETTY_FUNCTION__ << " allocated " << endl;
}

SimTime
Scheduler::simTime () const
{
  return currentTime;
}

SimTime
Scheduler::dueTime (int eventId)
{
  EventTimeMap::iterator it = eventTimes.find (eventId);
  if (it == eventTimes.end()) {
    return SimTime::tooLate();
  } else {
    return it->second;
  }
}

void
Scheduler::run ()
{
  cout << __PRETTY_FUNCTION__ << " running " << endl;
  cout << "                " << currentTime << endl;
  step ();
}

void
Scheduler::runUntil (const SimTime & endTime)
{
  do {
   step ();
  } while (currentTime < endTime);
}

void
Scheduler::step ()
{
  cout << __PRETTY_FUNCTION__ << endl;
  EventList::iterator it;
  SimTime nextTick (currentTime);
  nextTick.advance ();
  for (it = eventList.lower_bound (currentTime);
       it != eventList.end () && it->first < nextTick;
       it++) {
    Event* event = it->second;
    eventList.erase (currentTime, event);
    if (event) {
      eventTimes.erase (event->id());
      event->happen ();
      delete event;
    }
  }
  currentTime.advance();
  cout << "              " << currentTime << endl;
}

void
Scheduler::schedule (const Event & evt, const SimTime & when)
{
  Event * pEvent = evt.copy ();
  pEvent->scheduler = this;
  EventTimeMap::iterator eit = eventTimes.find (pEvent->id());
  if (eit != eventTimes.end()) {
    eventList.erase (when, pEvent);
    eventTimes.erase (pEvent->id());
  }
  eventList.insert (Event::ListPair (when, pEvent));
  eventTimes[pEvent->id()] = when;
}

} // namespace
