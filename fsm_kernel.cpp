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
* fsm_kernel.cpp
*
*  File created on:  Jan, 25th 2014 
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
