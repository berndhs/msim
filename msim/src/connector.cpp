#include "connector.h"


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


#include "scheduler.h"
#include "debug-log.h"

namespace msim
{

bool
operator< (const Connector::DataPacket & left, 
                      const Connector::DataPacket & right)
{
  return left.releaseTime < right.releaseTime;
}

Connector::Connector (Scheduler * sched, 
                      int inputs, 
                      int outputs)
  :scheduler(sched),
   numInputs (inputs > 0 ? inputs : 0),
   numOutputs (outputs > 0 ? outputs : 0),
   delayFunc (0),
   nextArrival (SimTime::tooLate()),
   outputEvent (sched, this)
{
  delayMap = new SimTickType* [numInputs];
  destination = new DataDestination* [numInputs];
  for (int i=0; i<numInputs; i++) {
    delayMap[i] = new SimTickType[numOutputs];
    for (int j=0; j<numOutputs; j++) {
      delayMap[i][j] = 1;
    }
  }
  for (int d=0; d<numOutputs; d++) {
    destination[d] = 0;
  }
}

Connector::~Connector ()
{
  for (int i=0; i<numInputs; i++) {
    delete[] delayMap[i];
  }
}

void
Connector::registerClient (int output, DataDestination * client)
{
  MS_TRACE << " out " << output << " client " << client << std::endl;
  MS_TRACE << "    numOut " << numOutputs << std::endl;
  if (0 <= output && output < numOutputs) {
    destination[output] = client;
  }
}

void
Connector::unregisterClient (int output) 
{
  if (0 <= output && output <= numOutputs) {
    destination[output] = 0;
  }
}

SimTickType
Connector::delay (int input, int output)
{
  if (delayFunc) {
    return (*delayFunc)(this, input, output);
  } else {
    return 1;
  }
}

void
Connector::setDelay (int input, int output, SimTickType delay)
{
  if (input >= numInputs || output >= numOutputs) {
    return;
  }
  if (input < 0) {
    for (int i=0; i<numInputs; i++) {
      setOutputDelay (i, output, delay);
    }
  } else {
    setOutputDelay (input, output, delay);
  }
}

void
Connector::setOutputDelay (int input, int output, SimTickType delay)
{
  if (input < 0 || input >= numInputs || output >= numOutputs) {
    return;
  }
  if (output < 0) {
    for (int i=0; i<numOutputs; i++) {
      delayMap [input][i] = delay;
    }
  } else {
    delayMap [input][output] = delay;
  }
}

void
Connector::setDelayFunction (DelayFunctionType * function)
{
  delayFunc = function;
}

SimTime
Connector::write (int input, int output, SimpleTaggedDataPtr data)
{
  MS_TRACE << " in " << input << " out " << output << std::endl;
  if (scheduler() == 0 || data == 0 
      || input < 0 || input >= numInputs
      || output >= numOutputs) {
    return SimTime::tooLate();
  }
  SimTime arrival = scheduler()->simTime();
  SimTickType maxDelay (-1);
  if (output < 0) {
    for (int i=0; i<numOutputs; i++) {
      SimTickType di = delayMap[input][i];
      if (di > maxDelay) {
        maxDelay = di;
      }
    }
  } else {
    maxDelay = delayMap[input][output];
  }
  if (maxDelay < 0) {
    return SimTime::tooLate();
  }
  arrival += maxDelay;
  if (output >= 0) {
    packetMap[output].insert (DataPacket (arrival, data, input, output));
  } else {
    // NOTE this will replicate the data pointer !
    for (int out=0; out<numOutputs; out++) {
      packetMap[out].insert (DataPacket (arrival, data, input, out));
    }
  }
  if (arrivalTimes.find (arrival) == arrivalTimes.end()) {
    arrivalTimes.insert (arrival);
    reschedule ();
  }
  return arrival;
}

SimpleTaggedDataPtr
Connector::read (int output)
{
  if (output < 0 || output >= numOutputs) {
    return 0;
  }
  if (packetMap[output].empty()) {
    return 0;
  }
  if (scheduler() == 0) {
    return 0;
  }
  SimTime now = scheduler()->simTime();
  PacketQueue::iterator front = packetMap[output].begin ();
  if (front->releaseTime > now) {
    return 0;
  }
  DataPacket packet (*front);
  MS_TRACE << " count before erase " << packetMap[output].size () << std::endl;
  packetMap[output].erase (front);
  MS_TRACE << " count after erase " << packetMap[output].size () << std::endl;
  return packet.data;
}

SimTime
Connector::whenReadAvailable (int output)
{
  if (output < 0 || output >= numOutputs
      || packetMap[output].empty()) {
    return SimTime::tooLate();
  }
  PacketQueue::iterator front = packetMap[output].begin ();
  return front->releaseTime;
}

bool
Connector::isReadAvailable (int output)
{
  if (output < 0 || output >= numOutputs) {
    return false;
  }
  if (scheduler() == 0) {
    return false;
  }
  if (packetMap[output].empty()) {
    return false;
  }
  SimTime now = scheduler()->simTime();
  PacketQueue::iterator front = packetMap[output].begin ();
  return now >= front->releaseTime;
}

void
Connector::reschedule ()
{
  MS_TRACE << std::endl;
  if (!arrivalTimes.empty()) {
    auto it = arrivalTimes.begin ();
    SimTime first = *it;
    if (first < nextArrival) {
      MS_TRACE << " schedule output for " << first << std::endl;
      scheduler()->schedule (outputEvent, first);
      arrivalTimes.erase (it);
    }
  }
}

void
Connector::deliverOutputs ()
{
  for (int d=0; d<numOutputs; d++) {
    MS_TRACE << d << std::endl;
    deliverOutputs (d);
  }
}

void
Connector::deliverOutputs (int output)
{
  DataDestination * dest = destination[output];
  if (!dest) {
    return;
  }
  Scheduler * sched = scheduler();
  if (!sched) {
    return;
  }
  SimTime now = sched->simTime();
  PacketQueue  &q (packetMap[output]);
  auto dit = q.begin ();
  while (dit != q.end()) {
    DataPacket packet (*dit);
    if (packet.releaseTime <= now) {
      if (dest->dataArrived (packet.data)) {
        q.erase (dit);
      }
    }
    dit ++;
  }
}

void
Connector::OutputEvent::happen ()
{
  MS_TRACE << " at time " << scheduler()->simTime() << std::endl;
  MS_TRACE << " sched " << scheduler() << " conn " << connector << std::endl;
  if (scheduler() && connector) {
    connector->deliverOutputs();
  }
}

} // namespace
