package uta.cse3310;

public interface ControlsToElevator {
    void GO_UP();

    // When this control is active the elevator goes up.

    void GO_DOWN();

    // When this control is active the elevator goes down.

    void STOP();

    // When this control is active, the elevator stops moving.

    void OPEN_DOOR();

    // When this control is active, the door open mechanism is powered

    void CLOSE_DOOR();

    // When this control is active, the door close mechanism is powered
    // Note if the door is obstructed, leaving this control turned on will damage
    // the cargo.

}
