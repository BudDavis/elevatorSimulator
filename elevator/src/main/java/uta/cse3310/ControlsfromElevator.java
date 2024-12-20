package uta.cse3310;

public interface ControlsfromElevator {

    void DOOR_IS_OPEN();

    // Event occurs when the door is completely open.

    void DOOR_IS_CLOSED();

    // Event occurs when the door is completely closed

    void DOOR_IS_OBSTRUCTED();

    // Event occurs when the safety interlock detects and obstruction of the door,
    // while the door is in motion.

    void CAB_POSITION_FLOOR_2();

    // Elevator Cab has passed the sensor for floor 2
    // If a STOP command is sent upon receipt of this event, the elevator cab will
    // be stopped exactly on floor 2

    void CAB_POSITION_FLOOR_2_5();

    // Elevator Cab has passed the sensor between floor 2 and 3

    void CAB_POSITION_FLOOR_3();

    // Elevator Cab has passed the sensor for floor 3
    // If a STOP command is sent upon receipt of this event, the elevator cab will
    // be stopped exactly on floor 3

    void CAB_POSITION_FLOOR_3_5();

    // Elevator Cab has passed the sensor between floor 3 and 4

    void CAB_POSITION_FLOOR_4();

    // Elevator Cab has passed the sensor for floor 4
    // If a STOP command is sent upon receipt of this event, the elevator cab will
    // be stopped exactly on floor 4

}

 