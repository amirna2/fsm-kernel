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
* fsm_private.h
*
*  File created on:  Jan 25th, 2014 
*
*  Description: Private header file for the Fsm Engine
*/
#ifndef _FSM_PRIVATE_H_
#define _FSM_PRIVATE_H_

#define MAX_PARAM_STRUCT_SIZE 1
#define EVENTQ_SIZE           1

#define UNEXPECTED_EVENT_AC   0x00FE
#define NO_EVENT              0x00FF
#define NO_ACTION             0x0000

#define CONDITIONAL_STATE     0xFFFF

typedef uint32_t  FsmState;
typedef uint32_t  FsmEvent;
typedef uint32_t  FsmAction;

typedef struct
{
  uint8_t plist[MAX_PARAM_STRUCT_SIZE];
  uint32_t size;
  FsmEvent event;
}FsmTblEvent;

/* FSM table generation macros
 * The MAKEFSMTable macro creates a Finite State Machine table. It takes the following inputs:
 * name of the state machine, number of states, number of event and the number of actions performed to
 * get into a state.
 * Example:
 * MAKEFSMTable(myFSM, 2,3,1) = {
 *   {{1,b},
 *    {2,c},
 *    {3,d}
 *   },
 *   {
 *    {4,e},
 *    {5,f},
 *    {0,a}
 *   }
 * };
 * 
 * where 0-5 are the different states
 * where a-e are the different actions
 * Note this assumes that the state and action counts are the same
 * The +1 is for the state entry in the current_event_entry point current_event_entry[0]
 */

#define MAKEFSMTable(tableName, numStates, numEvents, numActions) \
static const uint32_t tableName[numStates][numEvents][numActions + 1]

/* use this macro instead of directly calling "tableName in the above macro */
/* e.g MAKEFSMTable(FSMTable(myFSM), 3,2,1); */
#define FSMTable(tableName) (FsmState*)&tableName;



  
#endif //_FSM_PRIVATE_H_
