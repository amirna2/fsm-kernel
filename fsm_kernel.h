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

/*
* fsm_kernel.h
*
*  File created on:  Jan 25th, 2014 
*
*  Description: Public header file for the Fsm Engine
*/

#ifndef _FSMKERNEL_H_
#define _FSMKERNEL_H_

#include "fsm_private.h"
#include "fsm_msg.h"

typedef bool (*FsmActionFuncPtr) (FsmAction action)

class FsmKernel
{
  public:
    FsmKernel( char* fsm_name, FsmState* table,
               uint32_t num_state,
               uint32_t num_event,
               uint32_t num_action,
               FsmEvent event_start,
               FsmEvent event_end,
               FsmState initial_state,
               FsmActionFuncPtr action_handler
             );
    ~FsmKernel(){};
    
    /* Fsm entry points methods */
    void processEventWithNoParams(FsmEvent event);
    void processEventParamStruct(FsmEvent event, void* param_struct, uint32_t size);
    void processEventWithOneParam(FsmEvent event, int64_t size);
    
    /* Event parameter query */
    void* getEventParameters();
    FsmEvent getCurrentEvent();
    FsmState getConditionalState();
    
    /* Trace */
    void initTrace(const char* eventTable[], const char* actionTable[], const char* stateTable[]);
  
  private:
    /* Trace related */
    char   mFsmName[256];                  /* current fsm name */
    char   mTrace[256];                    /* trace to be displayed */
    const char** mEventNameTbl;            /* event names */
    const char** mActionNameTbl;           /* action names */
    const char** mStateNameTbl;            /* state names */
    
    /* Fsm engine variables */
    FsmState mState;                       /* current state */
    FsmState mStateCount;                  /* Number of states */
    FsmState mCondState;                   /* Next state (result from a condition) */
    
    FsmEvent mEventCount;                  /* number of events */
    FsmEvent mEventStart;                  /* event range */
    FsmEvent mEventEnd;
    
    FsmAction mActionCount;                /* number of actions */
    FsmState* mStateTable;
    uint32_t  mStateSize;
    uint32_t  mEventSize;
    
    /* This queue is to ensure only one event gets processed at a time */
    FsmTableEvent mEventQ[EVENTQ_SIZE];
    FsmMsg* mEvent;
    
    uint32_t mPutIndex;
    uint32_t mGetIndex;
    uint32_t mQueueEntries;                 /* current number of entries in the queue */
    
    /* Pointer to action handler */
    bool (*actionHandler)(FsmAction action);
    
    /* Get the index in the FSM according to the current state and event */
    uint32_t getFsmTableIndex();
};

#endif /*_FSMKERNEL_H_    
