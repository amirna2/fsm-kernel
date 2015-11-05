Copyright (c) 2000-2014 - Amir Nathoo 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

#ifndef _FSM_MSG_H_
#define _FSM_MSG_H

#include <string.h>
#include "fsm_kernel.h"

class FsmMsg
{
  public:
    FsmMsg();
    FsmMsg(FsmEvent msg_id, void* data, uint32_t size);
    ~FsmMsg():
    
    void setMessageId(FsmEvent msg_id);
    FsmEvent readMessageId();
    void* readMessageData();
    void writeMessageData(void* data, uint32_t size);
    uint32_t getMessageDataSize();
    
  private:
    FsmEvent mMessageId;
    unsigned char* mData;
    uint32_t mSize;
}

inline FsmEvent FsmMsg::readMessageId()
{
  return mMessageId;
}

inline void FsmMsg::setMessageId(FsmEvent msg_id)
{
  mMessageId = msg_id;
}

inline void* Fsm::readMessageData()
{
  return mData;
}

inline void FsmMsg::writeMessageData(void* data, uint32_t size)
{
  if( mData )
  {
    delete mData;
  }
  if( data )
  {
    mData = new unsigned char[size];
    memcpy( mData, data, size);
    mSize = size;
  }
}

inline uint32_t FsmMsg::getDataSize()
{
  return mSize;
}

#endif //_FSM_MSG_H_
