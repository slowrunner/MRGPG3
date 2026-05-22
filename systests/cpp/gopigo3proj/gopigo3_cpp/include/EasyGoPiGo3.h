/*
 *  https://www.dexterindustries.com/GoPiGo3/
 *  https://github.com/DexterInd/GoPiGo3
 *
 *  Copyright (c) 2026 Modular Robotics
 *  Released under the MIT license (http://choosealicense.com/licenses/mit/).
 *
 *  C++ port of EasyGoPiGo3 Python library
 *  Sensor functions for distance sensor, IMU, etcetera are intentionally excluded.
 *
 *  using RGBColor = tuple(uint8_t R, uint8_t G, uint8_t B)
 *
 *  Class EasyGoPiGo3(string config_file_path): public GoPiGo3
 *    config_file_path defaults to ~/.gpg3_config.json
 *    Inherits all GoPiGo3 instance vars and methods
 *
 *    Instance Vars:
 *      int speed: default 150 DPS
 *      tuple RGBColor left_eye_color:   (uint8 R, uint8 G, uint8 B)
 *      tuple RGBColor right_eye_color:  (uint8 R, uint8 G, uint8 B)
 *      left_encoder_target
 *      right_encoder_target
 *
 *    Implemented Methods:
 *      void  set_speed(int speed_in_DPS=150)
 *      void  reset_speed():  reset speed to DEFAULT_SPEED (300 DPS)
 *      int   get_speed()
 *      void  forward(): drive forward - use set_speed() or default: 150 DPS
 *      void  backward(): drive backward
 *      void  stop():   stop both motors, power off to let wheels float
 *      void  drive_cm(float dist, blocking=true): dist in cm
 *      void  drive_inches(float dist, blocking=true): dist in inches
 *      void  right() : pivot cw around right wheel
 *      left  left():  pivot ccw around left wheel
 *      void  spin_right(): spin in place clockwise
 *      void  spin_left(): spin in place counter-clockwise
 *      void  steer(int left_percent, int right_percent):  -100 to +100
 *      target_reached(left_tgt_degrees, right_tgt_degrees):  use to detect when to stop 
 *                                                            forward(), backward(), right(), left(), spin_right(), spin_left()
 *                                                            and for non-blocking drive_cm() or drive_inches()
 *      void reset_encoders(bool blocking = true): resets both encoders to 0, 
 *                                                 if blocking=true waits till both motors have stopped
 *      pair(int32_t, int32_t) read_encoders(string& units = "cm") returns (left,right) encoder in degrees or converted to a distance  
 *                                                   units: {"cm","in","inch","inches","raw") 
 *                                                   "raw" causes encoder degree average for both left and right
 *      float read_encoders_average(units = "cm") returns average of left and right encoders in degrees or optionall convert to distance
 *                                                   units: {"cm","in","inch","inches","raw") 
 *                                                   "raw" causes encoder degree average for both left and right
 *      void drive_degrees( float degrees, bool blocking=true): drive by turning each wheel degrees
 *      void turn_degrees(in:deg, blocking=true):  left: negative degrees
 *      void orbit(float degrees, float radius_cm = 0.0f, bool blocking = true)  Orbit with radius to inside wheel
 *      float volt();  return battery voltage as float
 *      void blinker_on(int id)   0=Left,1=Right
 *      void blinker_off(int id)  0=Left 1=Right
 *      void blinker_on(string&  id)   "left" or "right"
 *      void blinker_off(string& id)   "left" or "right"
 *      void led_on(int id)        alias for blinker_on
 *      void led_on(string& id)    alias for blinker_on
 *      void led_off(int id)       alias for blinker_off
 *      void led_off(string& id)   alias for blinker_off
 *
 *      void set_left_eye_color(RGBColor& color)
 *      void set_right_eye_color(RGBColor& color)
 *      void set_eye_color(RGBColor& color)    set both left and right to same color
 *      void open_left_eye()   turn on left eye  (to the stored left eye  color)
 *      void open_right_eye()  turn on right eye (to the stored right eye color)
 *      void open_eyes()       turn on both right and left eyes (with respective stored color)
 *      void close_left_eye()  turn off left eye programmable LED
 *      void close_right_eye() turn off right eye programmable LED
 *      void close_eyes()      turn off both right and left programmable "eye" LEDs
 *
 * 
 */

#ifndef EasyGoPiGo3_h_
#define EasyGoPiGo3_h_

#include "GoPiGo3.h"
#include <tuple>
#include <string>
#include <stdexcept>

// RGB color represented as a tuple of three uint8_t values (R, G, B)
using RGBColor = std::tuple<uint8_t, uint8_t, uint8_t>;

class EasyGoPiGo3 : public GoPiGo3 {
public:
    // -----------------------------------------------------------------------
    // Construction / destruction
    // -----------------------------------------------------------------------

    /**
     * Constructor.
     *
     * Initializes the robot with default speed (300 DPS) and turquoise eyes.
     *
     * @param config_file_path  Path to the JSON robot-constants config file.
     *                          Empty string uses the default ~/.gpg3_config.json.
     * @throws std::runtime_error  When the GoPiGo3 hardware is not detected.
     * @throws std::runtime_error  When the firmware version does not match.
     */
    EasyGoPiGo3(const std::string& config_file_path = "");

    // -----------------------------------------------------------------------
    // Speed control
    // -----------------------------------------------------------------------

    /** Set motor speed in DPS (0-1000 recommended). */
    void set_speed(int in_speed);


    /** Return the current speed in DPS. */
    int      speed;             ///< Current motor speed in DPS

    int  get_speed() const;

    /** Reset speed to DEFAULT_SPEED (300 DPS). */
    void reset_speed();

    // -----------------------------------------------------------------------
    // Basic motion
    // -----------------------------------------------------------------------

    /** Stop both motors (float). */
    void stop();

    /** Drive forward continuously at current speed. */
    void forward();

    /** Drive backward continuously at current speed. */
    void backward();

    /**
     * Pivot right: left motor runs, right motor stops.
     * Results in a tight arc turn to the right.
     */
    void right();

    /**
     * Spin right in place: left motor forward, right motor backward.
     * Both wheels spin at current speed in opposite directions.
     */
    void spin_right();

    /**
     * Pivot left: right motor runs, left motor stops.
     * Results in a tight arc turn to the left.
     */
    void left();

    /**
     * Spin left in place: left motor backward, right motor forward.
     * Both wheels spin at current speed in opposite directions.
     */
    void spin_left();

    /**
     * Independently control each motor as a percentage of current speed.
     *
     * @param left_percent   -100 to 100: percentage of speed for the left motor.
     * @param right_percent  -100 to 100: percentage of speed for the right motor.
     */
    void steer(int left_percent, int right_percent);

    // -----------------------------------------------------------------------
    // Distance / angle motion
    // -----------------------------------------------------------------------

    /**
     * Drive forward (positive) or backward (negative) for a given distance.
     *
     * @param dist      Distance in centimetres.
     * @param blocking  If true, wait until the target is reached.
     */
    void drive_cm(float dist, bool blocking = true);

    /**
     * Drive forward (positive) or backward (negative) for a given distance.
     *
     * @param dist      Distance in inches.
     * @param blocking  If true, wait until the target is reached.
     */
    void drive_inches(float dist, bool blocking = true);

    /**
     * Drive forward (positive) or backward (negative) by rotating each
     * wheel the specified number of degrees.
     *
     * @param degrees   Wheel rotation in degrees (360 = one full wheel rotation).
     * @param blocking  If true, wait until the target is reached.
     */
    void drive_degrees(float degrees, bool blocking = true);

    /**
     * Rotate the robot in place by the given heading angle.
     *
     * @param degrees   Positive = right, negative = left.
     * @param blocking  If true, wait until the target is reached.
     */
    void turn_degrees(float degrees, bool blocking = true);

    /**
     * Drive the robot in a circular arc around an object.
     *
     * @param degrees    Arc in degrees (360 = full circle). Negative = left.
     * @param radius_cm  Orbit radius in centimetres (0 = spin in place).
     * @param blocking   If true, wait until the target is reached.
     *
     * @note In non-blocking mode, call set_speed() before the next movement
     *       because orbit temporarily changes per-motor speed limits.
     */
    void orbit(float degrees, float radius_cm = 0.0f, bool blocking = true);

    // -----------------------------------------------------------------------
    // Encoder utilities
    // -----------------------------------------------------------------------

    /**
     * Check whether both wheels have reached their target positions
     * (within a B15 degree tolerance).
     *
     * @param left_target_degrees   Target encoder position for the left wheel.
     * @param right_target_degrees  Target encoder position for the right wheel.
     * @return true when both wheels are within the tolerance window.
     */
    bool target_reached(float left_target_degrees, float right_target_degrees);

    /**
     * Reset both motor encoders to zero.
     *
     * @param blocking  If true, wait until the motors have fully stopped.
     */
    void reset_encoders(bool blocking = true);

    /**
     * Read both encoder positions.
     *
     * @return Pair of (left_encoder_degrees, right_encoder_degrees).
     */
    std::pair<int32_t, int32_t> read_encoders();

    /**
     * Read the average of both encoder positions, optionally converted to
     * a distance unit.
     *
     * @param units  "cm" (default), "in" / "inch" / "inches", or anything
     *               else for raw encoder-degree average.
     * @return Average encoder value in the requested unit.
     */
    float read_encoders_average(const std::string& units = "cm");

    // -----------------------------------------------------------------------
    // Voltage
    // -----------------------------------------------------------------------

    /** Return the battery voltage as a float. */
    float volt();

    // -----------------------------------------------------------------------
    // Blinker LEDs
    // -----------------------------------------------------------------------

    /**
     * Turn on a blinker LED.
     *
     * @param id  0 or "right" for the right blinker,
     *            1 or "left"  for the left blinker.
     */
    void blinker_on(int id);
    void blinker_on(const std::string& id);

    /**
     * Turn off a blinker LED.
     *
     * @param id  0 or "right" for the right blinker,
     *            1 or "left"  for the left blinker.
     */
    void blinker_off(int id);
    void blinker_off(const std::string& id);

    /** Alias for blinker_on(). */
    void led_on(int id);
    void led_on(const std::string& id);

    /** Alias for blinker_off(). */
    void led_off(int id);
    void led_off(const std::string& id);

    // -----------------------------------------------------------------------
    // Eye LEDs
    // -----------------------------------------------------------------------

    /**
     * Set the stored color for the left eye (call open_left_eye() to apply).
     *
     * @param color  RGB tuple (r, g, b), each component 0-255.
     * @throws std::invalid_argument if the tuple values are out of range (unused
     *         in C++ since uint8_t is already bounded, but kept for parity).
     */
    void set_left_eye_color(const RGBColor& color);

    /** Set the stored color for the right eye (call open_right_eye() to apply). */
    void set_right_eye_color(const RGBColor& color);

    /** Set the stored color for both eyes (call open_eyes() to apply). */
    void set_eye_color(const RGBColor& color);

    /** Turn on (illuminate) the left eye LED with its stored color. */
    void open_left_eye();

    /** Turn on (illuminate) the right eye LED with its stored color. */
    void open_right_eye();

    /** Turn on both eye LEDs with their stored colors. */
    void open_eyes();

    /** Turn off the left eye LED. */
    void close_left_eye();

    /** Turn off the right eye LED. */
    void close_right_eye();

    /** Turn off both eye LEDs. */
    void close_eyes();

    // -----------------------------------------------------------------------
    // Public constants (match Python defaults)
    // -----------------------------------------------------------------------
    static constexpr int DEFAULT_SPEED   = 300;
    static constexpr int NO_LIMIT_SPEED  = 1000;

private:
    RGBColor left_eye_color;    ///< Stored left eye color
    RGBColor right_eye_color;   ///< Stored right eye color
};

#endif // EasyGoPiGo3_h_
