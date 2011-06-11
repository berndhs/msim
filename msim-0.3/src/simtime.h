
#ifndef MINISCSIM_SIMTIME_H
#define MINISCSIM_SIMTIME_H


/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2004, 2011, Bernd Stramm
 *
 */

/*
//
// miniscsim library, Copyright (C) 2004 - Bernd H Stramm
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it may be useful,
// but WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
*/

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

namespace miniscsim
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

  SimTimeT<TimeType> () {
    whichEternity = beforeUniverse;
  }

  SimTimeT<TimeType> (const SimTimeT<TimeType>& sOld) {
    whichEternity = sOld.whichEternity;
    t = sOld.t;
  }

  SimTimeT<TimeType> (Eternity when) {
    whichEternity = when;
    t = 0;
  }

  SimTimeT<TimeType> (TimeType theTime) {
    whichEternity = inUniverse;
    t = theTime;
  }

  void Set (TimeType absolute) { // makes it a valid time
    whichEternity = inUniverse;
    t = absolute;
  }

  void Advance (TimeType increment = 1) { // makes it valid
    if (whichEternity == beforeUniverse) {
      whichEternity = inUniverse;
      t = increment;
    } else if (whichEternity == inUniverse) {
      t += increment;
    } // otherwise its after the end of the universe anyway,
    // can't get any later
  }

  SimTimeT<TimeType> operator+ (TimeType incr) {
    SimTimeT<TimeType> result (*this);
    result.Advance (incr);
    return result;
  }

  static SimTimeT<TimeType> TooEarly () {
    return (SimTimeT<TimeType>(beforeUniverse));
  }

  static SimTimeT<TimeType> TooLate () {
    return (SimTimeT<TimeType>(afterUniverse));
  }

  TimeType Ticks() {
    return t;
  }
  Eternity Epoch() {
    return whichEternity;
  }


  string EpochChar() const {
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

  void Dump( ostream& out) {
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

  TimeType t;
  Eternity whichEternity;


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
    out << "<" << t1.EpochChar() << "," << t1.t << ">";
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


typedef SimTimeT<long long int> SimTime;

typedef SimTimeT<double> DSimTime;

} // namespace miniscsim

#endif
