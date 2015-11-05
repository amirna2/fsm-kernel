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

/*
* fsm_kernel.h
*
*  Created on:  Jan,25th 2014 
*      Author:  Amir Nathoo
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
    char   mFsmName[256];                 /* current fsm name */
    char   mTrace[256];                    /* trace to be displayed */
    const char** mEventNameTbl;          /* event names */
    const char** mActionNameTbl;         /* action names */
    const char** mStateNameTbl;          /* state names */
    
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
