module uta.cse3310 {
    requires javafx.controls;
    requires javafx.fxml;

    opens uta.cse3310 to javafx.fxml;
    exports uta.cse3310;
}
