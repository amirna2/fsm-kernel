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
* fsm_private.h
*
*  Created on:  Jan,25th 2014 
*      Author:  Amir Nathoo
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
