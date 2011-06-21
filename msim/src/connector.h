#ifndef MSIM_CONNECTOR_H
#define MSIM_CONNECTOR_H


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


#include "property.h"
#include "simtime.h"
#include "tagged-data.h"
#include "data-destination.h"
#include "event.h"
#include "debug-log.h"

#include <map>
#include <set>

namespace msim
{

class Scheduler;

class Connector 
{
public:

  class DataPacket {
  public:
    DataPacket ()
      :releaseTime (SimTime::tooLate()),
       data (0),
       input (-1),
       output (-1)
    {}
    DataPacket (SimTime rt, SimpleTaggedDataPtr pD, int in, int out)
      :releaseTime (rt),
       data (pD),
       input (in),
       output (out)
    {}
    DataPacket (const DataPacket &other) 
      :releaseTime (other.releaseTime),
       data (other.data),
       input (other.input),
       output (other.output)
    {}
    SimTime              releaseTime;
    SimpleTaggedDataPtr  data;
    int                  input;
    int                  output;
    bool operator< (const DataPacket & other)
    { 
      MS_TRACE << releaseTime << " ?< " << other.releaseTime << std::endl;
      return releaseTime < other.releaseTime; 
    }
  };



  Connector (Scheduler * sched, int inputs=1, int outputs=1);
  ~Connector ();

  int inputs ();
  int outputs ();
  void registerClient   (int output, DataDestination * client);
  void unregisterClient  (int output);

  Property <Scheduler*> scheduler;

  /* data packet read/write functions */

  /** write: output = -1 broadcasts */
  virtual SimTime write (int input, int output, SimpleTaggedData * data);

  /** read returns null if not available at current sim time */
  virtual SimpleTaggedDataPtr  read   (int output);
  virtual SimTime  whenReadAvailable  (int output);
  virtual bool     isReadAvailable    (int output);

  /* delay calculation facilities */

  typedef SimTickType (*DelayFunctionType) (Connector*, int, int);

  virtual SimTickType delay (int input, int output);

  virtual void        setDelay (int input, int output, 
                                SimTickType delay);
  void setDelayFunction (DelayFunctionType * function);

private:

  int                 numInputs;
  int                 numOutputs;

  /* Delay calculation stuff */

  DelayFunctionType  *delayFunc;

  void setOutputDelay (int input, int output, SimTickType delay);

  /* Data Packet queueing stuff */
  

protected:

  typedef std::multiset <DataPacket>   PacketQueue;
  typedef std::map <int, PacketQueue>  PacketQueueMap;
  typedef std::multiset <SimTime>      ArrivalTimeQueue;

  class OutputEvent : public Event {
  public:
    OutputEvent ()
      :Event (0), connector (0)
    {}
    OutputEvent (Scheduler * sch, Connector * conn)
      :Event (sch),
       connector (conn)
    {}
    OutputEvent *copy () const
    {
      return new OutputEvent (scheduler(), connector);
    }
    void happen ();
  private:
    Connector * connector;
  };

  void reschedule ();
  void deliverOutputs ();
  void deliverOutputs (int output);

  SimTickType        **delayMap;
  DataDestination    **destination;

  PacketQueueMap    packetMap;
  ArrivalTimeQueue  arrivalTimes;
  SimTime           nextArrival;

  OutputEvent       outputEvent;

};


bool operator< (const Connector::DataPacket & left, 
                const Connector::DataPacket & right);

} // namespace

#endif
