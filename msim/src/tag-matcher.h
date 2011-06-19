#ifndef MSIM_TAG_MATCHER_H
#define MSIM_TAG_MATCHER_H


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


#include "tagged-data.h"
#include <map>

namespace msim
{

class TagMatcherClient {
public:
  TagMatcherClient () {}
  virtual bool dataArrived (SimpleTaggedDataPtr  pData) = 0;
};

class TagMatcher 
{
public:


  TagMatcher ();
  TagMatcher (const TagMatcher & other);

  void registerClient   (TagMatcherClient * client, 
                   DataTagType        tag, 
                   TagDuration        duration);
  int unregisterClient (TagMatcherClient * client);
  int unregisterClient (TagMatcherClient * client, DataTagType tag);

  bool dataArrived (SimpleTaggedDataPtr  pData);

private:

  class ClientRecord {
  public:
    ClientRecord (DataTagType t, TagDuration d, TagMatcherClient *pD)
      :tag (t),
       duration (d),
       client (pD)
    {}
    ClientRecord (const ClientRecord &other)
      :tag (other.tag),
       duration (other.duration),
       client (other.client)
    {}
    DataTagType        tag;
    TagDuration        duration;
    TagMatcherClient  *client;
  };

  typedef std::pair <DataTagType, ClientRecord>                       
      ClientMapPair;
  typedef std::multimap <DataTagType, ClientRecord>  
      ClientMap;

  ClientMap  clientMap;

};

} // namespace


#endif
