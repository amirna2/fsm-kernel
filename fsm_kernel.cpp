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
* fsm_kernel.cpp
*
*  Created on:  Jan,25th 2014 
*      Author:  Amir Nathoo
*
*  Description: Implementation for the FSM engine
*/

#include <string.h>
#include <stdio.h>
#include "fsm_kernel.h"

/***************************************************************************************************************
* Name:               FsmKernel()
* Scope:              Public
* Description:        The Fsm Engine class constructor
*  
* Parameters:
*   char* fsm_name                      - Given name for the Finite State Machine (for tracing purpose)
*   FsmState* table                     - Next state/action list table
*   uint32_t  num_state                 - Number of states in the FSM
*   uint32_t  num_event                 - Number of events in the FSM
*   uint32_t  num_action                - Max Number of actions performed per event
*   FsmState  initial_state             - Initial state of the machine
*   FsmActionFuncPtr action_handler     - the FSM action handler
* Return:
*   void
* Additional Notes:
****************************************************************************************************************/
FsmKernel::FsmKernel( char* fsm_name, FsmState* table,
               uint32_t num_state,
               uint32_t num_event,
               uint32_t num_action,
               FsmEvent event_start,
               FsmEvent event_end,
               FsmState initial_state,
               FsmActionFuncPtr action_handler )
{
  
  strcpy( mFsmName, fsm_name );
  mState = initial_state;
  mCondState = cond_state;
  mStateCount = num_state;
  mEventCount = num_event;
  mEventStart = event_start;
  mEventEnd = event_end;
  mActionCount = num_action;

  mStateTable = table;
  mEventSize  = 2; /*because each event in each state has a "Next event and an "Action" */
  mStateSize = (mEventSize * mEventCount);
  mPutIndex = mGetIndex = mQueueEntries = 0;
  mActionHandler = action_handler;
  mEvent = NULL;
}

void FsmKernel::processEventWithNoParams( FsmEvent event )
{
  this->processEventWithParamStruct( event, (void*)NULL, 0 );
}

void FsmKernel::processEventWithOneParams( FsmEvent event, int64_t p1 )
{
  this->processEventWithParamStruct( event, &p1, sizeof( int64_t ) );
}

void FsmKernel::processEventWithParamStruct(FsmEvent event, void* param_struct, uint32_t size)
{
  /* making sure the event can be processed */
  if( event < mEventStart || event >mQueueEntries mEventEnd )
  {
    return;
  }
  
  // the fsm event is stored dynamically in a single object
  mEvent = new FsmMsg( event, param_struct, size );
  if(mEvent == NULL)
  {
    return;
  }
  // magic function to get the current index in the fsm table
  uint32_t current_index = getFsmTableIndex();
  FsmAction* action = ACTION_LIST( mStateTable, current_index );
  FsmState next_state;
  uint32_t i;
  bool change_state = true;
  
  //Perform all the actions in the associated action list
  for( i = 0; ( ((action[i] != NO_ACTION) && (i < mActionCount)) ); i++)
  {
    if( action[i] == UNEXPECTED_EVENT_AC )
    {
      change_state = false;
      break;
    }
    if( !(*(this->actionHandler))(action[i]) )
    {
      change_state = false; // don't change state if the action failed to execute properly
    }
  }
  
  //Decide on the transition to the next state
  if( change_state )
  {
    if( NEXT_STATE( mStateTable, current_index ) == CONDITIONAL_STATE )
    {
      // next state is determine by the action
      next_state = getConditionalState();
      if( next_state == CONDITIONAL_STATE )
      {
        next_state = mState; // this is still a conditional state - don't transition
      } 
    }
    else // we have an actual state
    {
      next_state = NEXT_STATE(mStateTable, current_index);
    }
  }
  else // no state change
  {
    next_state = mState
  }
  //TODO: Maybe log the state here
  
  //Perform the state machine transition if needed
  if( next_state != mState )
  {
    mState = next_state;
  }
  
  delete mEvent;
}

FsmEvent FsmKernel::getCurrentEvent()
{
  return eventQ[mGetIndex].event;
}

FsmEvent FsmKernel::getEventParameters()
{
  return( mEvent->readMessageData() );
}

uint32_t FsmKernel::getFsmTableIndex()
{
  
  uint32_t state_offset = mState * mStateSize;
  uint32_t event_offset = (mEvent->readMessageId() - mEventStart) * mEventSize;
  
  return( state_offset + event_offset );
}

void FsmKernel::initTrace(const char* eventTable[], const char* actionTable[], const char* stateTable[])
{
  mEventNameTbl = eventTable;
  mActionNameTbl = actionTable;
  mStateNameTbl = stateTable;
}
  
void FsmKernel::setConditionalState(FsmState st)
{
  mCondState = st;
}

FsmState FsmKernel::getConditionalState()
{
  return mCondState;
}
