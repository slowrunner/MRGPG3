/*
 *  test_EasyGoPiGo3.cpp
 *
 *  Example / smoke-test program for the EasyGoPiGo3 C++ library.
 *
 *  Build (on the GoPiGo3 Raspberry Pi):
 *
 *    g++ -std=c++17 -o test_EasyGoPiGo3 \
 *        test_EasyGoPiGo3.cpp EasyGoPiGo3.cpp GoPiGo3.cpp \
 *        -lrt -lm
 *
 *  Run as root (SPI requires elevated access):
 *
 *    sudo ./test_EasyGoPiGo3
 */

#include "EasyGoPiGo3.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <unistd.h>  // sleep / usleep

// ---------------------------------------------------------------------------
// Print a section header and wait for the user to press Return.
// ---------------------------------------------------------------------------
static void wait_for_enter(const char* test_name) {
    std::cout << "\n=== " << test_name << " ===\n";
    std::cout << "Press Return to perform test: " << test_name << " ... ";
    std::cout.flush();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main() {

    // ------------------------------------------------------------------
    // 1. Instantiate the robot
    // ------------------------------------------------------------------
    std::cout << "Initialising GoPiGo3...\n";
    EasyGoPiGo3 gpg;

    // ------------------------------------------------------------------
    // 2. Hardware / firmware info  (no movement - no prompt needed)
    // ------------------------------------------------------------------
    std::cout << "\n=== Hardware info ===\n";

    char mfr[32] = {}, board[32] = {}, hw_ver[32] = {}, fw_ver[32] = {}, id[64] = {};
    gpg.get_manufacturer(mfr);
    gpg.get_board(board);
    gpg.get_version_hardware(hw_ver);
    gpg.get_version_firmware(fw_ver);
    gpg.get_id(id);

    std::cout << "Manufacturer  : " << mfr    << '\n';
    std::cout << "Board         : " << board  << '\n';
    std::cout << "HW version    : " << hw_ver << '\n';
    std::cout << "FW version    : " << fw_ver << '\n';

    std::cout << "\n=== Class Constants ===\n";
    std::cout << "gpg.WHEEL_BASE_WIDTH - - - - - : " << gpg.WHEEL_BASE_WIDTH     << '\n';
    std::cout << "gpg.WHEEL_DIAMETER             : " << gpg.WHEEL_DIAMETER     << '\n';
    std::cout << "gpg.WHEEL_BASE_CIRCUMFERENCE - : " << gpg.WHEEL_BASE_CIRCUMFERENCE     << '\n';
    std::cout << "gpg.WHEEL_CIRCUMFERENCE        : " << gpg.WHEEL_CIRCUMFERENCE     << '\n';
    std::cout << "gpg.MOTOR_GEAR_RATIO  - - - -  : " << gpg.MOTOR_GEAR_RATIO     << '\n';
    std::cout << "gpg.ENCODER_TICKS_PER_ROTATION : " << gpg.ENCODER_TICKS_PER_ROTATION     << '\n';
    std::cout << "gpg.MOTOR_TICKS_PER_DEGREE  -  : " << gpg.MOTOR_TICKS_PER_DEGREE     << '\n';


    // ------------------------------------------------------------------
    // 3. Voltage  (no movement - no prompt needed)
    // ------------------------------------------------------------------
    std::cout << "\n=== Voltage ===\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Battery voltage : " << gpg.volt() << " V\n";

    // ------------------------------------------------------------------
    // 4. Speed control  (no movement - no prompt needed)
    // ------------------------------------------------------------------
    std::cout << "\n=== Speed control ===\n";
    std::cout << "Default speed   : " << gpg.get_speed() << " DPS\n";
    gpg.set_speed(200);
    std::cout << "After set(200)  : " << gpg.get_speed() << " DPS\n";
    gpg.reset_speed();
    std::cout << "After reset     : " << gpg.get_speed() << " DPS\n";

    // ------------------------------------------------------------------
    // 5. Eye LEDs
    // ------------------------------------------------------------------
    wait_for_enter("Eye LEDs");

    std::cout << "Opening both eyes (turquoise default)..., close left, close right\n";
    gpg.open_eyes();
    sleep(1);
    gpg.close_left_eye();
    sleep(1);
    gpg.close_right_eye();

    std::cout << "Changing to red... open left, open right\n";
    gpg.set_eye_color({255, 0, 0});
    gpg.open_left_eye();
    sleep(1);
    gpg.open_right_eye();
    sleep(1);

    std::cout << "Left eye green, right eye blue...open both eyes\n";
    gpg.set_left_eye_color({0, 255, 0});
    gpg.set_right_eye_color({0, 0, 255});
    gpg.open_eyes();
    sleep(1);

    std::cout << "Closing both eyes...\n";
    gpg.close_eyes();
    sleep(1);

    // ------------------------------------------------------------------
    // 6. Blinker LEDs
    // ------------------------------------------------------------------
    wait_for_enter("Blinker LEDs");

    std::cout << "Left blinker ON  (int id = 1)...\n";
    gpg.blinker_on(1);
    sleep(1);
    gpg.blinker_off(1);

    std::cout << "Right blinker ON (string id = \"right\")...\n";
    gpg.blinker_on("right");
    sleep(1);
    gpg.blinker_off("right");

    std::cout << "Both blinkers via led_on/led_off...\n";
    gpg.led_on(0);
    gpg.led_on(1);
    sleep(1);
    gpg.led_off(0);
    gpg.led_off(1);

    // ------------------------------------------------------------------
    // 7. Drive forward 30.5 cm (blocking)
    // ------------------------------------------------------------------
    wait_for_enter("Drive 30.5 cm forward at 125 DPS");

    gpg.set_speed(125);
    gpg.reset_encoders();

    std::cout << "Reset Encoders and Drive 30.5 cm forward...\n";
    gpg.drive_cm(30.5f, true);
    std::cout << "Done.\n";

    auto [l1, r1] = gpg.read_encoders();
    std::cout << "Encoders  - left: " << l1 << " deg  right: " << r1 << " deg\n";
    std::cout << "Average   - " << gpg.read_encoders_average("cm") << " cm\n";
    std::cout << "Average   - " << gpg.read_encoders_average("inch") << " inches\n";
    sleep(1);

    // ------------------------------------------------------------------
    // 8. Encoder reset and read
    // ------------------------------------------------------------------
    wait_for_enter("Encoder reset and read");

    auto [l0, r0] = gpg.read_encoders();
    std::cout << "Before reset  - left: " << l0 << " deg  right: " << r0 << " deg\n";

    gpg.reset_encoders();

    auto [le1, re1] = gpg.read_encoders();
    std::cout << "After reset  - left: " << le1 << " deg  right: " << re1 << " deg\n";

    // ------------------------------------------------------------------
    // 9. Drive backward 30.5 cm (blocking)
    // ------------------------------------------------------------------
    wait_for_enter("Drive 30.5 cm backward");

    std::cout << "Driving 30.5 cm backward...\n";
    gpg.drive_cm(-30.5f, true);
    std::cout << "Encoder Average   - " << gpg.read_encoders_average("inch") << " inches\n";
    std::cout << "Done.\n";
    sleep(1);

    // ------------------------------------------------------------------
    // 10. Drive 12 inches forward (and return)
    // ------------------------------------------------------------------
    wait_for_enter("Drive 12 inches forward then return");
    gpg.reset_encoders();

    std::cout << "Driving 12 inches forward...\n";
    gpg.drive_inches(12.0f, true);

    std::cout << "Done. Returning...\n";
    std::cout << "Average   - " << gpg.read_encoders_average("inch") << " inches\n";

    sleep(1);
    gpg.drive_inches(-12.0f, true);
    std::cout << "Back at start.\n";
    std::cout << "Average   - " << gpg.read_encoders_average("inch") << " inches\n";
    sleep(1);

    // ------------------------------------------------------------------
    // 11. Turn in place 90 deg right then 90 deg left
    // ------------------------------------------------------------------
    wait_for_enter("Turn 90 deg right then 90 deg left");

    std::cout << "Turning 90 deg right...\n";
    gpg.turn_degrees(90.0, true);
    sleep(1);

    std::cout << "Turning 90 deg left (returning to heading)...\n";
    gpg.turn_degrees(-90.0, true);
    sleep(1);

    // ------------------------------------------------------------------
    // 12. Steer arc
    // ------------------------------------------------------------------
    wait_for_enter("Steer: gentle right arc for 1 s");

    std::cout << "Gentle right arc (left=100%, right=50%) for 1 s...\n";
    gpg.steer(100, 50);
    sleep(1);
    gpg.stop();
    sleep(1);

    // ------------------------------------------------------------------
    // 13. Orbit 360 deg with radius 20 cm
    // ------------------------------------------------------------------
    wait_for_enter("Orbit 360 deg, radius 20 cm");

    std::cout << "Orbiting 360 deg around a 20 cm radius...\n";
    gpg.orbit(360.0f, 20.0f, true);
    std::cout << "Done.\n";
    sleep(1);

    // ------------------------------------------------------------------
    // 14. drive_degrees (one full wheel rotation forward and return)
    // ------------------------------------------------------------------
    wait_for_enter("Drive degrees: one full wheel rotation forward and return");

    std::cout << "Driving one full wheel rotation (360 deg) forward...\n";
    gpg.drive_degrees(360.0f, true);
    std::cout << "Done. Returning...\n";
    sleep(1);
    gpg.drive_degrees(-360.0f, true);
    std::cout << "Back at start.\n";
    sleep(1);

    // ------------------------------------------------------------------
    // 15. Non-blocking drive_cm with manual target_reached() poll
    // ------------------------------------------------------------------
    wait_for_enter("Non-blocking drive_cm 15 cm with manual poll");

    gpg.reset_encoders();
    float target           = 15.0f;
    float dist_mm          = target * 10.0f;
    float wheel_target_deg = (dist_mm / gpg.WHEEL_CIRCUMFERENCE) * 360.0f;

    std::cout << "Starting non-blocking 15 cm drive...\n";
    gpg.drive_cm(target, false);

    int polls = 0;
    while (!gpg.target_reached(wheel_target_deg, wheel_target_deg)) {
        ++polls;
        usleep(50000); // 50 ms
    }
    std::cout << "Target reached after " << polls << " poll(s).\n";
    gpg.stop();
    sleep(1);

    // ------------------------------------------------------------------
    // 16. Cleanup
    // ------------------------------------------------------------------
    std::cout << "\n=== Cleanup ===\n";
    gpg.stop();
    gpg.close_eyes();
    gpg.blinker_off(0);
    gpg.blinker_off(1);
    gpg.reset_all();

    std::cout << "\nAll tests complete.\n";
    return 0;
}

