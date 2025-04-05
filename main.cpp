#include "hx711_gpiod.hpp"
#include <iostream>
#include <unistd.h>

int main() {
    HX711 scale(27, 17); // CLK = GPIO line 27, DT = GPIO line 17

    if (!scale.load_calibration()) {
        std::cout << "No calibration found.\nTaring... Remove all weight." << std::endl;
        scale.tare();
        std::cout << "Tare done.\nPlace known weight (e.g., 500g) on scale." << std::endl;
        sleep(5);

        float known_weight = 500.0;
        float scale_factor = scale.calibrate_known_weight(known_weight);
        std::cout << "Calibration complete. Scale factor = " << scale_factor << std::endl;

        scale.save_calibration();
    } else {
        std::cout << "Loaded saved calibration. Reading values..." << std::endl;
    }

    while (true) {
        float weight = scale.get_units();
        std::cout << "Weight: " << weight << " g" << std::endl;
        usleep(500000);
    }

    return 0;
}
