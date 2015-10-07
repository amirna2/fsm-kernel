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
