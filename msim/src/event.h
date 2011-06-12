#ifndef MSIM_EVENT_H
#define MSIM_EVENT_H

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

#include "simtime.h"
#include "deliberate.h"
#include <map>

using namespace std;
using namespace deliberate;

namespace msim {
// an Event consists of a time (when it happens)
// and a set of entities that want to know that
// this has happened.

class Scheduler;

class Event {

 public:

  Event (Scheduler * scheduler);
  Event (const Event & other);
  virtual ~Event();

  Property <Scheduler*>  scheduler;

  int id () const { return ident; }

  virtual void    happen() = 0;
  virtual Event  *copy () const = 0;

  SimTime when () const;

  struct timeless {
    bool operator() (const SimTime t1, const SimTime t2) 
      const {
      return t1 < t2 ;
    }
  };

  typedef std::pair<SimTime, Event*> ListPair;

private:

  int         ident;

  static int  eventCounter;

};

ostream & 
operator<< (ostream & ost, const Event & evt);

typedef std::multimap <SimTime, Event *, Event::timeless>  TimedEventMap;

class EventList : public TimedEventMap
{
public:

  void erase (const SimTime & when, const Event * event);    
  void erase (EventList::iterator first, EventList::iterator last) {
    TimedEventMap::erase (first, last);
  }
};

} // namespace msim
#endif
