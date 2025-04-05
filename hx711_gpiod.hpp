#pragma once

#include <gpiod.h>
#include <unistd.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

class HX711 {
public:
    HX711(int clkLine, int dataLine, const std::string& chipName = "gpiochip0")
        : clkLineNum(clkLine), dataLineNum(dataLine), chipName(chipName) {
        chip = gpiod_chip_open_by_name(chipName.c_str());

        clk = gpiod_chip_get_line(chip, clkLineNum);
        data = gpiod_chip_get_line(chip, dataLineNum);

        gpiod_line_request_output(clk, "hx711", 0);
        gpiod_line_request_input(data, "hx711");
    }

    ~HX711() {
        gpiod_line_release(clk);
        gpiod_line_release(data);
        gpiod_chip_close(chip);
    }

    int32_t read() {
        while (gpiod_line_get_value(data) == 1) {
            usleep(1);
        }

        int32_t count = 0;
        for (int i = 0; i < 24; ++i) {
            gpiod_line_set_value(clk, 1);
            usleep(1);
            count = count << 1 | gpiod_line_get_value(data);
            gpiod_line_set_value(clk, 0);
            usleep(1);
        }

        // 25th pulse for channel A, gain 128
        gpiod_line_set_value(clk, 1);
        usleep(1);
        gpiod_line_set_value(clk, 0);
        usleep(1);

        if (count & 0x800000) {
            count |= ~0xFFFFFF;
        }

        return count;
    }

    void tare(int times = 10) {
        int64_t sum = 0;
        for (int i = 0; i < times; ++i) {
            sum += read();
            usleep(10000);
        }
        offset = sum / times;
    }

    void set_scale(float scale_factor) {
        scale = scale_factor;
    }

    float get_units(int times = 5) {
        int64_t sum = 0;
        for (int i = 0; i < times; ++i) {
            sum += read();
            usleep(10000);
        }
        float avg = static_cast<float>(sum / times);
        return (avg - offset) / scale;
    }

    float calibrate_known_weight(float known_weight_grams) {
        int64_t sum = 0;
        for (int i = 0; i < 10; ++i) {
            sum += read();
            usleep(10000);
        }
        int64_t avg = sum / 10;
        scale = static_cast<float>(avg - offset) / known_weight_grams;
        return scale;
    }

    void save_calibration(const std::string& filename = "calibration.txt") {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << offset << "\n" << scale << std::endl;
            file.close();
        }
    }

    bool load_calibration(const std::string& filename = "calibration.txt") {
        std::ifstream file(filename);
        if (file.is_open()) {
            file >> offset >> scale;
            file.close();
            return true;
        }
        return false;
    }

private:
    int clkLineNum, dataLineNum;
    std::string chipName;
    gpiod_chip* chip;
    gpiod_line* clk;
    gpiod_line* data;

    int32_t offset = 0;
    float scale = 1.0f;
};
