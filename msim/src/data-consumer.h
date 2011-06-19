#ifndef MSIM_DATA_CONSUMER_H
#define MSIM_DATA_CONSUMER_H


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


#include "tag-matcher.h"
#include "expect-buffer.h"

namespace msim
{
class DataConsumer
{
public:

  DataConsumer ();

  int  portCount ();
  TagMatcherClient * port (int portNum);

  void expectTag (int port, DataTagType tag);
  SimpleTaggedDataPtr  readPort (int port);

private:

  class Register : public ExpectBuffer, public TagMatcher
  {
  public:
    Register ();
    bool dataArrived (SimpleTaggedDataPtr pData);
  private:
    DataConsumer *parent;
  };

  const int numPorts;
  Register     *registerFile;
};

} // namespace

#endif
