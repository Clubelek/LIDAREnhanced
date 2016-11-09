#include <Arduino.h>
#include <Wire.h>
#include "I2CFunctions.h"
// We got a Lidar object per laser. 

#ifndef LIDAR_OBJECT_H
#define LIDAR_OBJECT_H

enum LIDAR_STATE {
  SHUTING_DOWN = 240,       // Shutdown the laser to reset it
  NEED_RESET = 48,          // Too much outliers, need to reset
  RESET_PENDING = 80,       // Wait 15ms after you reset the Lidar, we are waiting in this state
  NEED_CONFIGURE = 144,     // 15ms passed, we now configure the Lidar
  ACQUISITION_READY = 32,   // I started an acquisition, need someone to read it
  ACQUISITION_PENDING = 64, // The acquisition in on progress
  ACQUISITION_DONE = 128    // I read the data, need to start an acq again
};

enum LIDAR_MODE {
  NONE = 0,
  DISTANCE = 1,
  VELOCITY = 2,
  DISTANCE_AND_VELOCITY = 3
};

class LidarObject {
  public:
/*******************************************************************************
  Constructor
*******************************************************************************/
    LidarObject() {};
/*******************************************************************************
  begin : Begin the I2C master device

  If fasti2c is true, use 400kHz I2C
*******************************************************************************/
    void begin(uint8_t _EnablePin = 2, uint8_t _ModePin = 1, uint8_t _Lidar = 0x62, uint8_t _configuration = 2,  LIDAR_MODE _mode = DISTANCE, char _name = 'A'){
      pinMode(_EnablePin, OUTPUT);
      mode = DISTANCE;
      configuration = _configuration;
      address = _Lidar;
      lidar_state = NEED_RESET;
      EnablePin = _EnablePin;
      ModePin = _ModePin;
      name = _name;
    };


/*******************************************************************************
  setName : set the One char long name

  name is a one char long name 
*******************************************************************************/
    void setName(char _name){
      name = _name;
    };

/*******************************************************************************
  getName : get the One char long name

  return name which is a one char long name 
*******************************************************************************/
    char getName(char _name){
      return name;
    };

/*******************************************************************************
  on : Power On the device
*******************************************************************************/
    void on(){
      digitalWrite(EnablePin, HIGH);
    };

/*******************************************************************************
  off : Power Off the device
*******************************************************************************/
    void off(){
      digitalWrite(EnablePin, LOW);
    };

/*******************************************************************************
  on : Power On the device
*******************************************************************************/
    void enable(){
      digitalWrite(ModePin, HIGH);
    };

/*******************************************************************************
  off : Power Off the device
*******************************************************************************/
    void disable(){
      digitalWrite(ModePin, LOW);
    };

/*******************************************************************************
  timer_update : Update the timer to the current time to start the timer.
*******************************************************************************/
    void timer_update(){
      timeReset = micros();
    };


/*******************************************************************************
  reset_chack : Check the reset timer to see if the laser is correctly resetted

  The laser takes 20ms to reset
*******************************************************************************/
    bool check_reset(){
      if(lidar_state != NEED_RESET)
        return true;

      return (micros() - timeReset > 20000);
    };


/*******************************************************************************
  check_timer : Check the reset timer to see if the laser is correctly resetted

  The laser takes 20ms to reset
*******************************************************************************/
    bool check_timer(){
      if(lidar_state != RESET_PENDING)
        return true;

      return (micros() - timeReset > 20000);
    };

/*******************************************************************************
  resetNacksCount : The nack counter makes the Arduino able to know if a laser 
  needs to be resetted
*******************************************************************************/
    bool resetNacksCount(){
      nacksCount = 0;
    };

    LIDAR_MODE mode = DISTANCE;
    uint8_t nacksCount = 0;
    unsigned long timeReset = 0;
    uint8_t configuration = 2;
    uint8_t address = 0x62;
    LIDAR_STATE lidar_state = NEED_RESET;
    uint8_t EnablePin = 2;
    uint8_t ModePin = 1;
    char name = '\0';
};

#endif
