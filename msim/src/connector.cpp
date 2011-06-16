#include "connector.h"

namespace msim
{
Connector::Connector (Scheduler * sched, 
                      int inputs, 
                      int outputs)
  :numInputs (inputs > 0 ? inputs : 0),
   numOutputs (outputs > 0 ? outputs : 0),
   delayFunc (0)
{
  for (int i=0; i<numInputs; i++) {
    delayMap[i] = new SimTickType[numOutputs];
    for (int j=0; j<numOutputs; j++) {
      delayMap[i][j] = 1;
    }
  }
}

Connector::~Connector ()
{
  for (int i=0; i<numInputs; i++) {
    delete[] delayMap[i];
  }
}

SimTickType
Connector::delay (Connector * context, int input, int output)
{
  if (delayFunc) {
    return (*delayFunc)(context,input,output);
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


} // namespace
