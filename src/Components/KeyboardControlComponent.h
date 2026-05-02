#pragma once

struct KeyboardControlComponent {
    double velocity;

    KeyboardControlComponent(double velocity = 0.0) : velocity(velocity) {
    }
};
