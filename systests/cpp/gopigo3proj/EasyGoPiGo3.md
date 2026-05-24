# EasyGoPiGo3 C++ API
```
    C++ port of EasyGoPiGo3 Python library
    Sensor functions for distance sensor, IMU, etcetera are intentionally excluded.
  
    using RGBColor = tuple(uint8_t R, uint8_t G, uint8_t B)
  
    Class EasyGoPiGo3(string config_file_path): public GoPiGo3
      config_file_path defaults to ~/.gpg3_config.json
      Inherits all GoPiGo3 instance vars and methods
  
      Instance Vars:
        int speed: default 150 DPS
        tuple RGBColor left_eye_color:   (uint8 R, uint8 G, uint8 B)
        tuple RGBColor right_eye_color:  (uint8 R, uint8 G, uint8 B)
        left_encoder_target
        right_encoder_target
```
  
 ### IMPLEMENTED METHODS:
```
(Methods maked with P were tested by test_EasyGoPiGo3.cpp and deemed to "PASS")

 P      void  set_speed(int speed_in_DPS=150)
 P      void  reset_speed():  reset speed to DEFAULT_SPEED (300 DPS)
 *      int   get_speed()
 *      void  forward(): drive forward - use set_speed() or default: 150 DPS
 *      void  backward(): drive backward
 *      void  stop():   stop both motors, power off to let wheels float
 P      void  drive_cm(float dist, blocking=true): dist in cm
 P      void  drive_inches(float dist, blocking=true): dist in inches
 *      void  right() : pivot cw around right wheel
 *      left  left():  pivot ccw around left wheel
 *      void  spin_right(): spin in place clockwise
 *      void  spin_left(): spin in place counter-clockwise
 P      void  steer(int left_percent, int right_percent):  -100 to +100
 P      target_reached(left_tgt_degrees, right_tgt_degrees):  use to detect when to stop 
                                                              forward(), backward(), right(), left(), spin_right(), spin_left()
                                                              and for non-blocking drive_cm() or drive_inches()
 P      void reset_encoders(bool blocking = true): resets both encoders to 0, 
                                                   if blocking=true waits till both motors have stopped
 P      pair(int32_t, int32_t) read_encoders(string& units = "cm") returns (left,right) encoder in degrees or converted to a distance  
                                                     units: {"cm","in","inch","inches","raw") 
                                                     "raw" causes encoder degree average for both left and right
 P      float read_encoders_average(units = "cm") returns average of left and right encoders in degrees or optionall convert to distance
                                                     units: {"cm","in","inch","inches","raw") 
                                                     "raw" causes encoder degree average for both left and right
 P      void drive_degrees( float degrees, bool blocking=true): drive by turning each wheel degrees
 P      void turn_degrees(in:deg, blocking=true):  left: negative degrees
 P      void orbit(float degrees, float radius_cm = 0.0f, bool blocking = true)  Orbit with radius to center of GoPiGo3
 P      float volt();  return battery voltage as float
 P      void blinker_on(int id)   0=Left,1=Right
 *      void blinker_off(int id)  0=Left 1=Right
 *      void blinker_on(string&  id)   "left" or "right"
 P      void blinker_off(string& id)   "left" or "right"
 P      void led_on(int id)        alias for blinker_on
 *      void led_on(string& id)    alias for blinker_on
 P      void led_off(int id)       alias for blinker_off
 *      void led_off(string& id)   alias for blinker_off
 *
 P      void set_left_eye_color(RGBColor& color)
 P      void set_right_eye_color(RGBColor& color)
 P      void set_eye_color(RGBColor& color)    set both left and right to same color
 P      void open_left_eye()   turn on left eye  (to the stored left eye  color)
 P      void open_right_eye()  turn on right eye (to the stored right eye color)
 P      void open_eyes()       turn on both right and left eyes (with respective stored color)
 P      void close_left_eye()  turn off left eye programmable LED
 P      void close_right_eye() turn off right eye programmable LED
 P      void close_eyes()      turn off both right and left programmable "eye" LEDs
```  

### INHERITED METHODS (GoPiGo3.h)
```
 P      class GoPiGo3 GoPiGo3()

    // Instance Variables
    float WHEEL_BASE_WIDTH         = 117;     // default distance (mm) from left wheel to right wheel.
    float WHEEL_DIAMETER           = 66.5;    // defautl wheel diameter (mm)
    float WHEEL_BASE_CIRCUMFERENCE = WHEEL_BASE_WIDTH * M_PI;  // pi from cmath
    float WHEEL_CIRCUMFERENCE      = WHEEL_DIAMETER   * M_PI;  // pi from cmath
    int   MOTOR_GEAR_RATIO         = 120;
    int   ENCODER_TICKS_PER_ROTATION = 6;  // default GoPiGo3 has 6 ticks, 16 ticks if in .gpg3_list_of_serial_numbers.pkl file
    float MOTOR_TICKS_PER_DEGREE     = ((MOTOR_GEAR_RATIO * ENCODER_TICKS_PER_ROTATION) / 360.0);  // ticks per degree of wheel shaft rotation

P   int     detect(bool critical = true) // Confirm that the GoPiGo3 is connected and up-to-date

P   int     get_manufacturer(char *str)  // Get the manufacturer (should be "Dexter Industries")
P   int     get_board(char *str)         // Get the board name (should be "GoPiGo3")
P   int     get_version_hardware(char *str)  // Get the hardware version number
P   int     get_version_firmware(char *str)  // Get the firmware version number
P   int     get_id(char *str)            // Get the serial number ID that is unique to each GoPiGo3


P   bool    check_serial_number_for_16_ticks(std::string serial_file_path="~/.gpg3_list_of_serial_numbers.pkl")  // true if in file
P   int     load_robot_constants(std::string config_file_path="~/.gpg3_config.json")  // Load (or set if no file)
    int     save_robot_constants(std::string config_file_path="~/.gpg3_config.json")  // Save (to JSON file)
    int     set_robot_constants(float wheel_diameter, float wheel_base_width, int ticks, int motor_gear_ratio)  // Set new robot values and dependent constants

    int     set_led(uint8_t led, uint8_t red, uint8_t green = 0, uint8_t blue = 0)  // Control the LED

  // Get the voltages of the four power rails
    // Get the voltage and return as floating point voltage
    float   get_voltage_5v     ();
    float   get_voltage_battery();
    // Pass the pass-by-reference float variable where the voltage will be stored. Returns the error code.
    int     get_voltage_5v     (float &voltage);
    int     get_voltage_battery(float &voltage);

  // Set a servo position in microseconds
    int     set_servo(uint8_t servo, uint16_t us);

  // Set the motor PWM power
    int     set_motor_power(uint8_t port, int8_t power);
  // Set the motor target position to run to (go to the specified position)
    int     set_motor_position(uint8_t port, int32_t position);
  // Set the motor speed in degrees per second.
    int     set_motor_dps(uint8_t port, int16_t dps);
  // Set the motor PWM and speed limits. PWM limit applies to set_motor_position and set_motor_dps and speed limit applies to set_motor_position.
    int     set_motor_limits(uint8_t port, uint8_t power, uint16_t dps);
  // Get the motor status. State, PWM power, encoder position, and speed (in degrees per second)
    int     get_motor_status(uint8_t port, uint8_t &state, int8_t &power, int32_t &position, int16_t &dps);
  // Offset the encoder position. By setting the offset to the current position, it effectively resets the encoder value.
    int     offset_motor_encoder(uint8_t port, int32_t position);
  // Get the encoder position
    // Pass the port and pass-by-reference variable where the encoder value will be stored. Returns the error code.
    int     get_motor_encoder(uint8_t port, int32_t &value);
    // Pass the port. Returns the encoder value.
    int32_t get_motor_encoder(uint8_t port);
    // Set ecoder value to 0
    int     reset_motor_encoder(uint8_t port);

  // Configure grove pin(s)/port(s)
    // Set grove port type
    int      set_grove_type(uint8_t port, uint8_t type);
    // Set grove pin(s) mode
    int      set_grove_mode(uint8_t pin, uint8_t mode);
    // Set grove pin(s) output state. LOW or HIGH (0 or 1).
    int      set_grove_state(uint8_t pin, uint8_t state);
    // Set grove pin(s) PWM duty cycle. 0-100% with 0.1% precision.
    int      set_grove_pwm_duty(uint8_t pin, float duty);
    // Set grove port(s) PWM frequency. 3 - 48000 Hz
    int      set_grove_pwm_frequency(uint8_t port, uint16_t freq = 24000);
  // Perform an I2C transaction
    int      grove_i2c_transfer(uint8_t port, i2c_struct_t *i2c_struct);
  // Start an I2C transaction
    int      grove_i2c_start(uint8_t port, i2c_struct_t *i2c_struct);
  // Read grove value(s)
    int      get_grove_value(uint8_t port, void *value_ptr);
  // Read grove pin state
    // return value
    uint8_t  get_grove_state(uint8_t pin);
    // get value with pass-by-reference, and return error code
    int      get_grove_state(uint8_t pin, uint8_t &value);
  // Read grove pin voltage
    // return floating point voltage
    float    get_grove_voltage(uint8_t pin);
    // get floating point value with pass-by-reference, and return error code
    int      get_grove_voltage(uint8_t pin, float &value);
  // Read grove pin raw ADC analog value
    // return the value
    uint16_t get_grove_analog(uint8_t pin);
    // get value with pass-by-reference, and return error code
    int      get_grove_analog(uint8_t pin, uint16_t &value);

  // Reset the grove ports (unconfigure), motors (float with no limits), and LEDs.
    int     reset_all();

```
