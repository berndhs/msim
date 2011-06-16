#ifndef MSIM_CONNECTOR_H
#define MSIM_CONNECTOR_H

#include "property.h"
#include "simtime.h"
#include "tagged-data.h"

#include <map>

namespace msim
{

class Scheduler;

class Connector {
public:

  class DataPacket {
  public:
    DataPacket ()
      :releaseTime (SimTime::tooLate()),
       data (0),
       input (-1),
       output (-1)
    {}
    DataPacket (SimTime rt, SimpleTaggedData *pD, int in, int out)
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
    SimpleTaggedData    *data;
    int                  input;
    int                  output;
  };

  Connector (Scheduler * sched, int inputs=1, int outputs=1);
  ~Connector ();

  int inputs ();
  int outputs ();

  Property <Scheduler*> scheduler;

  /* data packet read/write functions */

  virtual bool write (int input, SimpleTaggedData * data);

  virtual SimTime  readAvailable  (int output);
  virtual SimpleTaggedData * read (int output);

  /* delay calculation facilities */

  typedef SimTickType (*DelayFunctionType) (Connector*, int, int);

  virtual void        setDelay (int input, int output, 
                                SimTickType delay);
  virtual SimTickType delay (int input, int output);

  void setDelayFunction (DelayFunctionType * function);

  virtual SimTickType delay (Connector* context, int input, int output);

private:

  int                 numInputs;
  int                 numOutputs;

  /* Delay calculation stuff */

  DelayFunctionType  *delayFunc;

  void setOutputDelay (int input, int output, SimTickType delay);
  
  SimTickType        *delayMap[];


  /* Data Packet queueing stuff */
  
  typedef std::deque <DataPacket>  PacketQueue;
  typedef std::map <int, PacketQueue*>  PacketQueueMap;

  PacketQueueMap    packetMap;

};

} // namespace

#endif
