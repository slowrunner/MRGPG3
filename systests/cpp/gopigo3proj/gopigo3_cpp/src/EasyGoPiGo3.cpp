/*
 *  https://www.dexterindustries.com/GoPiGo3/
 *  https://github.com/DexterInd/GoPiGo3
 *
 *  Concept of this API is EasyGoPiGo3.py: Copyright (c) 2017 Dexter Industries
 *  Released under the MIT license (http://choosealicense.com/licenses/mit/).
 *
 *  C++ port of EasyGoPiGo3 Python library
 *  Sensor imports (distance sensor, IMU) are intentionally excluded.
 *
 *  NOTE:  This file was originally created by Claude Code via my prompting  
 *           to trans-code EasyGoPiGo3.py to C++ 
 *           ignoring all sensor imports/methods  
 *         Under active test, debug, and documentation May 2026  
 */

#include "EasyGoPiGo3.h"

#include <cmath>    // M_PI, std::abs
#include <unistd.h> // usleep

// ---------------------------------------------------------------------------
// Internal helper: sleep for the given number of milliseconds
// ---------------------------------------------------------------------------
static void sleep_ms(unsigned int ms) {
    usleep(ms * 1000u);
}

// ===========================================================================
// Construction
// ===========================================================================

EasyGoPiGo3::EasyGoPiGo3(const std::string& config_file_path)
    : GoPiGo3()
    , speed(DEFAULT_SPEED)
    , left_eye_color(0, 255, 255)   // turquoise
    , right_eye_color(0, 255, 255)  // turquoise
{
    // Load robot constants from config file if provided; fall back to default.
    if (!config_file_path.empty()) {
        load_robot_constants(config_file_path);
    } else {
        load_robot_constants(); // uses ~/.gpg3_config.json by default
    }

    // Apply the default speed limit to both motors
    set_speed(DEFAULT_SPEED);
}

// ===========================================================================
// Speed control
// ===========================================================================

void EasyGoPiGo3::set_speed(int in_speed) {
    speed = in_speed;
    set_motor_limits(MOTOR_LEFT + MOTOR_RIGHT, 0, static_cast<uint16_t>(speed));
}

int EasyGoPiGo3::get_speed() const {
    return speed;
}

void EasyGoPiGo3::reset_speed() {
    set_speed(DEFAULT_SPEED);
}

// ===========================================================================
// Basic motion
// ===========================================================================

void EasyGoPiGo3::stop() {
    set_motor_dps(MOTOR_LEFT + MOTOR_RIGHT, 0);
    sleep_ms(100);
    set_motor_power(MOTOR_LEFT + MOTOR_RIGHT, MOTOR_FLOAT);
    sleep_ms(100);
}

void EasyGoPiGo3::forward() {
    set_motor_dps(MOTOR_LEFT + MOTOR_RIGHT, NO_LIMIT_SPEED);
}

void EasyGoPiGo3::backward() {
    set_motor_dps(MOTOR_LEFT + MOTOR_RIGHT, -NO_LIMIT_SPEED);
}

void EasyGoPiGo3::right() {
    set_motor_dps(MOTOR_LEFT,  NO_LIMIT_SPEED);
    set_motor_dps(MOTOR_RIGHT, 0);
}

void EasyGoPiGo3::spin_right() {
    set_motor_dps(MOTOR_LEFT,   NO_LIMIT_SPEED);
    set_motor_dps(MOTOR_RIGHT, -NO_LIMIT_SPEED);
}

void EasyGoPiGo3::left() {
    set_motor_dps(MOTOR_LEFT,  0);
    set_motor_dps(MOTOR_RIGHT, NO_LIMIT_SPEED);
}

void EasyGoPiGo3::spin_left() {
    set_motor_dps(MOTOR_LEFT,  -NO_LIMIT_SPEED);
    set_motor_dps(MOTOR_RIGHT,  NO_LIMIT_SPEED);
}

void EasyGoPiGo3::steer(int left_percent, int right_percent) {
    set_motor_dps(MOTOR_LEFT,
                  static_cast<int16_t>(get_speed() * left_percent  / 100));
    set_motor_dps(MOTOR_RIGHT,
                  static_cast<int16_t>(get_speed() * right_percent / 100));
}

// ===========================================================================
// Distance / angle motion
// ===========================================================================

void EasyGoPiGo3::drive_cm(float dist, bool blocking) {
    float dist_mm          = dist * 10.0f;
    float WheelTurnDegrees = (dist_mm / WHEEL_CIRCUMFERENCE) * 360.0f;

    int32_t StartPositionLeft  = get_motor_encoder(MOTOR_LEFT);
    int32_t StartPositionRight = get_motor_encoder(MOTOR_RIGHT);

    set_motor_position(MOTOR_LEFT,  StartPositionLeft  + static_cast<int32_t>(WheelTurnDegrees));
    set_motor_position(MOTOR_RIGHT, StartPositionRight + static_cast<int32_t>(WheelTurnDegrees));

    if (blocking) {
        while (!target_reached(
                   StartPositionLeft  + WheelTurnDegrees,
                   StartPositionRight + WheelTurnDegrees)) {
            sleep_ms(100);
        }
    }
}

void EasyGoPiGo3::drive_inches(float dist, bool blocking) {
    drive_cm(dist * 2.54f, blocking);
}

void EasyGoPiGo3::drive_degrees(float degrees, bool blocking) {
    int32_t StartPositionLeft  = get_motor_encoder(MOTOR_LEFT);
    int32_t StartPositionRight = get_motor_encoder(MOTOR_RIGHT);

    set_motor_position(MOTOR_LEFT,  StartPositionLeft  + static_cast<int32_t>(degrees));
    set_motor_position(MOTOR_RIGHT, StartPositionRight + static_cast<int32_t>(degrees));

    if (blocking) {
        while (!target_reached(
                   StartPositionLeft  + degrees,
                   StartPositionRight + degrees)) {
            sleep_ms(100);
        }
    }
}

void EasyGoPiGo3::turn_degrees(float degrees, bool blocking) {
    // Distance each wheel must travel (mm)
    float WheelTravelDistance = (WHEEL_BASE_CIRCUMFERENCE * degrees) / 360.0f;
    // Convert that distance to degrees of wheel rotation
    float WheelTurnDegrees    = (WheelTravelDistance / WHEEL_CIRCUMFERENCE) * 360.0f;

    int32_t StartPositionLeft  = get_motor_encoder(MOTOR_LEFT);
    int32_t StartPositionRight = get_motor_encoder(MOTOR_RIGHT);

    // Left wheel forward, right wheel backward (or vice-versa for negative degrees)
    set_motor_position(MOTOR_LEFT,  StartPositionLeft  + static_cast<int32_t>( WheelTurnDegrees));
    set_motor_position(MOTOR_RIGHT, StartPositionRight + static_cast<int32_t>(-WheelTurnDegrees));

    if (blocking) {
        while (!target_reached(
                   StartPositionLeft  + WheelTurnDegrees,
                   StartPositionRight - WheelTurnDegrees)) {
            sleep_ms(100);
        }
    }
}

void EasyGoPiGo3::orbit(float degrees, float radius_cm, bool blocking) {
    int   spd    = get_speed();
    float radius = radius_cm * 10.0f; // convert to mm

    // Total arc distance (mm)
    float drive_distance   = static_cast<float>(M_PI) * std::abs(radius) * std::abs(degrees) / 180.0f;

    // Lateral offset between the two wheel paths (mm)
    float drive_difference = (WHEEL_BASE_CIRCUMFERENCE * degrees) / 360.0f;

    // Convert distances to wheel-rotation degrees
    float distance_degrees   = (drive_distance   / WHEEL_CIRCUMFERENCE) * 360.0f;
    float difference_degrees = (drive_difference / WHEEL_CIRCUMFERENCE) * 360.0f;

    float left_target  = distance_degrees + difference_degrees;
    float right_target = distance_degrees - difference_degrees;

    // Identify fast / slow motor
    uint8_t MOTOR_FAST, MOTOR_SLOW;
    float   fast_target, slow_target;

    if (degrees < 0) {
        MOTOR_FAST  = MOTOR_RIGHT;
        MOTOR_SLOW  = MOTOR_LEFT;
        fast_target = right_target;
        slow_target = left_target;
    } else {
        MOTOR_FAST  = MOTOR_LEFT;
        MOTOR_SLOW  = MOTOR_RIGHT;
        fast_target = left_target;
        slow_target = right_target;
    }

    // Handle direction sign
    float direction            = (spd < 0) ? -1.0f : 1.0f;
    float speed_with_direction = std::abs(static_cast<float>(spd));

    float fast_speed = speed_with_direction;
    float slow_speed = std::abs((speed_with_direction * slow_target) / fast_target);

    // Apply per-motor speed limits
    set_motor_limits(MOTOR_FAST, 0, static_cast<uint16_t>(fast_speed));
    set_motor_limits(MOTOR_SLOW, 0, static_cast<uint16_t>(slow_speed));

    int32_t StartPositionLeft  = get_motor_encoder(MOTOR_LEFT);
    int32_t StartPositionRight = get_motor_encoder(MOTOR_RIGHT);

    set_motor_position(MOTOR_LEFT,
                       StartPositionLeft  + static_cast<int32_t>(left_target  * direction));
    set_motor_position(MOTOR_RIGHT,
                       StartPositionRight + static_cast<int32_t>(right_target * direction));

    if (blocking) {
        while (!target_reached(
                   StartPositionLeft  + left_target  * direction,
                   StartPositionRight + right_target * direction)) {
            sleep_ms(100);
        }
        // Restore the original uniform speed once done
        set_speed(spd);
    }
}

// ===========================================================================
// Encoder utilities
// ===========================================================================

bool EasyGoPiGo3::target_reached(float left_target_degrees, float right_target_degrees) {
    constexpr float tolerance = 5.0f;

    int32_t cur_left  = get_motor_encoder(MOTOR_LEFT);
    int32_t cur_right = get_motor_encoder(MOTOR_RIGHT);

    return (cur_left  > (left_target_degrees  - tolerance) &&
            cur_left  < (left_target_degrees  + tolerance) &&
            cur_right > (right_target_degrees - tolerance) &&
            cur_right < (right_target_degrees + tolerance));
}

void EasyGoPiGo3::reset_encoders(bool blocking) {
    set_motor_power(MOTOR_LEFT + MOTOR_RIGHT, 0);

    int32_t left_target  = get_motor_encoder(MOTOR_LEFT);
    int32_t right_target = get_motor_encoder(MOTOR_RIGHT);

    offset_motor_encoder(MOTOR_LEFT,  left_target);
    offset_motor_encoder(MOTOR_RIGHT, right_target);

    if (blocking) {
        // Wait until the encoder values stop changing
        int32_t prev_left  = -9999;
        int32_t prev_right = -9999;
        while (prev_left  != get_motor_encoder(MOTOR_LEFT) ||
               prev_right != get_motor_encoder(MOTOR_RIGHT)) {
            prev_left  = get_motor_encoder(MOTOR_LEFT);
            prev_right = get_motor_encoder(MOTOR_RIGHT);
            sleep_ms(25);
        }
    }
}

std::pair<int32_t, int32_t> EasyGoPiGo3::read_encoders() {
    int32_t left_encoder  = get_motor_encoder(MOTOR_LEFT);
    int32_t right_encoder = get_motor_encoder(MOTOR_RIGHT);
    return {left_encoder, right_encoder};
}

float EasyGoPiGo3::read_encoders_average(const std::string& units) {
    auto [left, right] = read_encoders();
    float average = (static_cast<float>(left) + static_cast<float>(right)) / 2.0f;

    if (units == "cm") {
        average = ((average / 360.0f) * WHEEL_CIRCUMFERENCE) / 10.0f;
    } else if (units == "in" || units == "inch" || units == "inches") {
        average = ((average / 360.0f) * WHEEL_CIRCUMFERENCE) / (10.0f * 2.54f);
    }
    // else: return raw encoder average (degrees)
    return average;
}

// ===========================================================================
// Voltage
// ===========================================================================

float EasyGoPiGo3::volt() {
    return get_voltage_battery();
}

// ===========================================================================
// Blinker LEDs
// ===========================================================================

void EasyGoPiGo3::blinker_on(int id) {
    if (id == 1) {
        set_led(LED_LEFT_BLINKER,  255);
    } else if (id == 0) {
        set_led(LED_RIGHT_BLINKER, 255);
    }
}

void EasyGoPiGo3::blinker_on(const std::string& id) {
    if (id == "left") {
        set_led(LED_LEFT_BLINKER,  255);
    } else if (id == "right") {
        set_led(LED_RIGHT_BLINKER, 255);
    }
}

void EasyGoPiGo3::blinker_off(int id) {
    if (id == 1) {
        set_led(LED_LEFT_BLINKER,  0);
    } else if (id == 0) {
        set_led(LED_RIGHT_BLINKER, 0);
    }
}

void EasyGoPiGo3::blinker_off(const std::string& id) {
    if (id == "left") {
        set_led(LED_LEFT_BLINKER,  0);
    } else if (id == "right") {
        set_led(LED_RIGHT_BLINKER, 0);
    }
}

void EasyGoPiGo3::led_on(int id)                { blinker_on(id);  }
void EasyGoPiGo3::led_on(const std::string& id) { blinker_on(id);  }
void EasyGoPiGo3::led_off(int id)               { blinker_off(id); }
void EasyGoPiGo3::led_off(const std::string& id){ blinker_off(id); }

// ===========================================================================
// Eye LEDs
// ===========================================================================

void EasyGoPiGo3::set_left_eye_color(const RGBColor& color) {
    left_eye_color = color;
}

void EasyGoPiGo3::set_right_eye_color(const RGBColor& color) {
    right_eye_color = color;
}

void EasyGoPiGo3::set_eye_color(const RGBColor& color) {
    set_left_eye_color(color);
    set_right_eye_color(color);
}

void EasyGoPiGo3::open_left_eye() {
    set_led(LED_LEFT_EYE,
            std::get<0>(left_eye_color),
            std::get<1>(left_eye_color),
            std::get<2>(left_eye_color));
}

void EasyGoPiGo3::open_right_eye() {
    set_led(LED_RIGHT_EYE,
            std::get<0>(right_eye_color),
            std::get<1>(right_eye_color),
            std::get<2>(right_eye_color));
}

void EasyGoPiGo3::open_eyes() {
    open_left_eye();
    open_right_eye();
}

void EasyGoPiGo3::close_left_eye() {
    set_led(LED_LEFT_EYE, 0, 0, 0);
}

void EasyGoPiGo3::close_right_eye() {
    set_led(LED_RIGHT_EYE, 0, 0, 0);
}

void EasyGoPiGo3::close_eyes() {
    close_left_eye();
    close_right_eye();
}
