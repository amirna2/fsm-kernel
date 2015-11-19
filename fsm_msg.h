/*
Copyright (c) 2000-2014, Amir Nathoo <amir.nth@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    1.Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    2.Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    3.Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
