// This is the controller for the elevator.
// It is implemented strictly in the C programming language,
// but is callable by a C++ program, like for example an FLTK
// window, or a unit test framework
//
/////////////////////////////////////////////////////////////
#pragma once
#include <stdbool.h>

#include "controls_to_elevator.h"
#include "events_from_elevator.h"

#ifdef __cplusplus
extern "C" 
{
#endif
typedef enum {OFF,FLOOR2,FLOOR3,FLOOR4,GOINGUPTO3,GOINGDNTO3,GOINGUPTO4,GOINGDNTO2} elevatorStateEnum;

void controller_tick();
void controller_init();

// debugging / test routines
char *controller_current_state();
char* elevatorStateEnumNames(elevatorStateEnum e);

// visibility to support testing 
elevatorStateEnum transition(elevatorStateEnum state, elevatorEventEnum event);
#ifdef __cplusplus
}
#endif

