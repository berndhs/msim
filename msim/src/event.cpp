

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


#include "event.h"
#include "scheduler.h"
#include <iostream>

using namespace std;
using namespace msim;

int Event::eventCounter (1);

Event::Event (Scheduler * scheduler)
  :scheduler (scheduler),
   ident (eventCounter++)
{ 
}

Event::Event (const Event &other)
  :scheduler (other.scheduler()),
   ident (other.ident)
{
}

Event::~Event()
{
  cout << "Deallocating event " << hex << this << dec << endl;
}

SimTime
Event::when () const
{
  if (scheduler()) {
    return scheduler()->dueTime (ident);
  } else {
    return SimTime::tooLate();
  }
}


ostream &
operator<< (ostream & ost, const Event & evt)
{
  ost << "event " << evt.id() << " at " << evt.when()
      << " by " << evt.scheduler();
  return ost;
}

void
EventList::erase (const SimTime & when, const Event * event)
{
  iterator it = lower_bound (when);        
  while (it != end() && it->first == when) {
    if (it->second == event) {
      TimedEventMap::erase (it);
      break;
    } else {
      it++;
    }
  }
}


