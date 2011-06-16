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

  Connector (Scheduler * sched, int inputs=1, int outputs=1);
  ~Connector ();

  int inputs ();
  int outputs ();

  Property <Scheduler*> scheduler;

  virtual bool write (int input, SimpleTaggedData * data);

  virtual SimTime  readAvailable  (int output);
  virtual SimpleTaggedData * read (int output);

  virtual void setDelay (int input, int output, SimTickType delay);
  virtual SimTickType delay (int input, int output);

  typedef SimTickType (*DelayFunctionType) (Connector*, int, int);

  void setDelayFunction (DelayFunctionType * function);

  virtual SimTickType delay (Connector* context, int input, int output);

private:

  int                 numInputs;
  int                 numOutputs;
  DelayFunctionType  *delayFunc;

  void setOutputDelay (int input, int output, SimTickType delay);
  
  SimTickType        *delayMap[];

};

} // namespace

#endif
