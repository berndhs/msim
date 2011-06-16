#ifndef MSIM_CONNECTOR_H
#define MSIM_CONNECTOR_H

#include "property.h"
#include "simtime.h"
#include "tagged-data.h"

#include <map>
#include <set>

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
  };


  Connector (Scheduler * sched, int inputs=1, int outputs=1);
  ~Connector ();

  int inputs ();
  int outputs ();

  Property <Scheduler*> scheduler;

  /* data packet read/write functions */

  /** write: output = -1 broadcasts */
  virtual bool write (int input, int output, SimpleTaggedData * data);

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

  typedef std::multiset <DataPacket>  PacketQueue;
  typedef std::map <int, PacketQueue>  PacketQueueMap;

  SimTickType        **delayMap;

  PacketQueueMap    packetMap;

};


bool operator< (const Connector::DataPacket & left, 
                const Connector::DataPacket & right);

} // namespace

#endif
