
#ifndef MSIM_SIMTIME_H
#define MSIM_SIMTIME_H



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
#include <string>

using namespace std;


// we need some concepts here for UNDETERMINED times
// There should be unassigned time values that are:
//   - earlier than anything else
//        (something that has 'always' been here,
//         like a constant value maybe)
//   - later than end of the universe
//        (something that has never been requested,
//         something that will never expire)

namespace msim
{

template <typename TimeType>
class SimTimeT
{
public:

  //  typedef long long int TimeType ;

  enum Eternity { beforeUniverse = 1,
                  inUniverse = 2,
                  afterUniverse = 3
                };


  /// \brief constructors

  SimTimeT<TimeType> ()
    :whichEternity (beforeUniverse),
     t (TimeType()) 
  {}

  SimTimeT<TimeType> (const SimTimeT<TimeType>& other)
    :whichEternity (other.whichEternity),
     t (other.t) 
  {}

  SimTimeT<TimeType> (Eternity when)
    :whichEternity (when),
     t (TimeType())
  {}

  SimTimeT<TimeType> (Eternity eternity, TimeType tick)
    :whichEternity (eternity),
     t(tick)
  {}

  SimTimeT<TimeType> (TimeType theTime)
    :whichEternity (inUniverse),
     t (theTime)
  {}

  void set (TimeType absolute) { // makes it a valid time
    whichEternity = inUniverse;
    t = absolute;
  }

  SimTimeT<TimeType> operator+ (TimeType incr) const {
    SimTimeT<TimeType> result (*this);
    result.t += incr;
    return result;
  }

  SimTimeT<TimeType> operator+= (TimeType incr) {
    t += incr;
    return *this;
  }

  static SimTimeT<TimeType> tooEarly () {
    return (SimTimeT<TimeType>(beforeUniverse));
  }

  static SimTimeT<TimeType> tooLate () {
    return (SimTimeT<TimeType>(afterUniverse));
  }

  TimeType ticks() {
    return t;
  }
  Eternity epoch() {
    return whichEternity;
  }


  string epochString () const {
    if (whichEternity == inUniverse) {
      return string ("CE");     // Current Epoch
    } else if (whichEternity == beforeUniverse) {
      return string ("BCE");   // Becure Current Epoch
    } else if (whichEternity == afterUniverse) {
      return string ("PCE");    // Post (After)Current Epoch
    } else {
      return string ("invalid-time");
    }
  }

  void dump( ostream& out) {
    if (whichEternity == inUniverse) {
      out << t ;
    } else {
      out << "NaT: ";
      if (whichEternity == beforeUniverse) {
        out << "too-early";
      } else if (whichEternity == afterUniverse) {
        out << "too-late";
      } else {
        cout << "invalid-time";
      }
    }
  }


private:

  Eternity whichEternity;
  TimeType t;


  template <typename TT> friend int operator<
  (const SimTimeT<TT>& t1,
   const SimTimeT<TT>& t2) {
    if (t1.whichEternity == SimTimeT<TimeType>::inUniverse &&
        t2.whichEternity == SimTimeT<TimeType>::inUniverse) {
      return t1.t < t2.t;
    }
    return t1.whichEternity < t2.whichEternity;
  }

  template <typename TT> friend int operator==
  (const SimTimeT<TT>& t1,
   const SimTimeT<TT>& t2) {
    if (t1.whichEternity == SimTimeT<TimeType>::inUniverse &&
        t2.whichEternity == SimTimeT<TimeType>::inUniverse) {
      return t1.t == t2.t;
    }
    return t1.whichEternity == t2.whichEternity;
  }

  template <typename TT> friend ostream& operator<<
  (ostream& out, const SimTimeT<TT>& t1) {
    out << "<" << t1.epochString() << "," << t1.t << ">";
    return out;
  }



};

template <typename TimeType>
ostream&
operator<< (ostream& out, typename SimTimeT<TimeType>::Eternity e)
{
  if (e == SimTimeT<TimeType>::inUniverse) {
    out << " present-Eon " ;
  } else if (e == SimTimeT<TimeType>::beforeUniverse) {
    out << " earliest-Eon " ;
  } else if (e == SimTimeT<TimeType>::afterUniverse) {
    out << " last-Eon " ;
  } else {
    out << " NotATime " ;
  }

  return out;
}

template <typename TimeType>
inline int
operator != (const SimTimeT<TimeType>& t1, const SimTimeT<TimeType>& t2)
{
  return !(t1 == t2);
}


template <typename TimeType>
inline int
operator > (const SimTimeT<TimeType>& t1, const SimTimeT<TimeType>& t2)
{
  return t2 < t1;
}


template <typename TimeType>
inline int
operator <= (const SimTimeT<TimeType>& t1, const SimTimeT<TimeType>& t2)
{
  return !(t1 >t2);
}

template <typename TimeType>
inline int
operator >= (const SimTimeT<TimeType>& t1,const SimTimeT<TimeType>& t2)
{
  return !(t1 < t2);
}

typeded long long int SimTickType;

typedef SimTimeT<SimTickType> SimTime;


} // namespace msim

#endif
