// ctrl shift I formats code in vscode.....

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "debug.h"
#include "elevator.h"
#include "elevatorController.h"

// Simulaton of the actual, physical elevator is done in this
// module.

// these variables can be written / read by another thread,
// thus the volatile qualifier
static volatile unsigned int elapsedTime; // seconds
static volatile unsigned int power;       // 0 is off 1 is on
static volatile unsigned int cabPosition; // 20 to 40.  20 is the second floor,
                                          // 30 is the third floor, etc.
static volatile int cabDirection;         // -1 is down, +1 is up.  and 0 is neither
static volatile int doorPosition;         //  0 is open, 5 is fully closed
static volatile int doorDirection;        // +1 is closing, -1 is opening, 0 is not moving

static volatile int lastDoorPosition;
static volatile int lastCabPosition;

static volatile int obstructed;

static volatile unsigned int elevatorIndicators;

void door_obstructed(int x)
{
        DEBUG_PRINT("setting obstructed door to %#x\n", x);
        obstructed = x;
}

int elevator_indicators(unsigned int i)
{
        INFO_PRINT("indicators set to %#x\n", i);
        elevatorIndicators = i;
        return 1;
}

int elevator_control_cmd(unsigned int c)
{
        DEBUG_PRINT("cmd = %#x\n", c);
        // controls to the elevator are recieved here

        // Check for safety issues
        // Note these can be turned off with a compile option
        assert(!((c & GO_UP) & (c & GO_DOWN)));
        assert(!((c & GO_UP) & (c & STOP)));
        assert(!((c & GO_DOWN) & (c & STOP)));

        //    cabDirection
        // (any excuse to use popcount intrinsic)
        if (__builtin_popcount(c & (GO_UP | GO_DOWN | STOP)) > 1)
        {
                ERROR_PRINT("invalid command %#x\n", c);
                return -1;
        }

        // a safe default
        cabDirection = ELEV_CAB_DIRECTION_NEUTRAL;
        if (c & GO_UP)
        {
                cabDirection = ELEV_CAB_DIRECTION_UP;
        }
        if (c & GO_DOWN)
        {
                cabDirection = ELEV_CAB_DIRECTION_DOWN;
        }
        if (c & STOP)
        {
                cabDirection = ELEV_CAB_DIRECTION_NEUTRAL;
        }

        //    doorDirection
        doorDirection = ELEV_DOOR_DIRECTION_NOT_MOVING;
        if (__builtin_popcount(c & (OPEN_DOOR | CLOSE_DOOR)) > 1)
        {
                ERROR_PRINT("invalid command %#x\n", c);
                return -1;
        }
        if (c & OPEN_DOOR)
        {
                doorDirection = ELEV_DOOR_DIRECTION_OPENING;
        }
        if (c & CLOSE_DOOR)
        {
                doorDirection = ELEV_DOOR_DIRECTION_CLOSING;
        }
        return 0;
}

void init_elevator()
{
        DEBUG_PRINT("\n");
        // this function is called to initialize the simulated elevator
        elapsedTime = 0;
        power = ELEV_POWER_OFF;
        cabPosition = ELEV_CAB_POSITION_2;         // ground floor
        cabDirection = ELEV_CAB_DIRECTION_NEUTRAL; // not moving
        doorPosition = ELEV_DOOR_POSITION_CLOSED;  // closed
        doorDirection = ELEV_DOOR_DIRECTION_NOT_MOVING;
}

void elevator_tick()
{
        // DEBUG_PRINT("elevator tick called\n");
        if (power)
        {
                // DEBUG_PRINT("cab direction %d cabPosition %d\n", cabDirection, cabPosition);
                // DEBUG_PRINT("door direction %d door position %d\n", doorDirection, doorPosition);
                //////////////////////////////////////////////
                // move the time forward
                elapsedTime++;

                //////////////////////////////////////////////
                // move the cab, if so desired
                if (cabDirection == 0)
                {
                        // hang where we are
                }
                else if (cabDirection == ELEV_CAB_DIRECTION_UP)
                {
                        // going up
                        if (cabPosition < 40)
                        {
                                cabPosition++;
                        }
                }
                else if (cabDirection == -1)
                {
                        // going down
                        if (cabPosition > 20)
                        {
                                cabPosition--;
                        }
                }

                //////////////////////////////////////////////
                // deal with the door
                if (doorDirection == 0)
                {
                        ; // it is where it should be, and is not moving
                }
                else if (doorDirection == 1)
                {
                        if (doorPosition < 5)
                        {
                                doorPosition++;
                        }
                }
                else if (doorDirection == -1)
                {
                        if ((doorPosition == 4) && (cabDirection == -1) && obstructed)
                        {
                                // stop moving the door
                        }
                        else if (doorPosition)
                        {
                                doorPosition--;
                        }
                }

                ////////////////////////////////////////////////////////
                // generate any events for the elevator controller
                // these events are only generated once, each time they
                // occur
                /////////////////////////////////////////////////////////
                if (doorDirection)
                {
                        if ((doorPosition == 1) && (doorPosition != lastDoorPosition))
                        {
                                event_to_controller(DOOR_IS_OPEN);
                        }
                        else if ((doorPosition == 5) && (doorPosition != lastDoorPosition))
                        {
                                event_to_controller(DOOR_IS_CLOSED);
                        }
                }
                // DOOR_IS_OBSTRUCTED();
                // An obstructed door is found when the door is obstructed when it is closing,
                // at position 4
                if ((doorPosition == ELEV_DOOR_POSITION_CLOSED - 1) && (doorDirection == -1) && obstructed)
                {
                        event_to_controller(DOOR_IS_OBSTRUCTED);
                }
                else if (cabDirection)
                {
                        if (lastCabPosition != cabPosition)
                        {
                                if (cabPosition == ELEV_CAB_POSITION_2)
                                {
                                        event_to_controller(CAB_POSITION_FLOOR_2);
                                }
                                else if (cabPosition == ELEV_CAB_POSITION_2_5)
                                {
                                        event_to_controller(CAB_POSITION_FLOOR_2_5);
                                }
                                else if (cabPosition == ELEV_CAB_POSITION_3)
                                {
                                        event_to_controller(CAB_POSITION_FLOOR_3);
                                }
                                else if (cabPosition == ELEV_CAB_POSITION_3_5)
                                {
                                        event_to_controller(CAB_POSITION_FLOOR_3_5);
                                }
                                else if (cabPosition == ELEV_CAB_POSITION_4)
                                {
                                        event_to_controller(CAB_POSITION_FLOOR_4);
                                }
                        }
                } // cab is moving
                lastCabPosition = cabPosition;
                lastDoorPosition = doorPosition;

        } // power is on
}

unsigned int timeInSeconds()
{
        return elapsedTime;
}

void power_on()
{
        DEBUG_PRINT("\n");

        if (!power)
        {
                init_elevator();
                controller_init();

                power = 1;
                elevatorIndicators = ALL_OFF;
                event_to_controller(POWER_ON);
        }
}

void power_off()
{
        DEBUG_PRINT("\n");
        power = ELEV_POWER_OFF;
}

int power_status()
{
        return power;
}

int cab_position()
{
        return cabPosition;
}

unsigned int indicators()
{
        return elevatorIndicators;
}

int door_position()
{
        return doorPosition;
}
