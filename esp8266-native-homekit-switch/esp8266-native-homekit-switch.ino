
#define EVERY_N_MILLISECONDS(interval) for (static unsigned long _lastTime = millis(); millis() - _lastTime >= (interval); _lastTime = millis())

/* ############################### ~ cooper.bin @ makesmart.net ~ ############################### */
/* ---------------------------------------------------------------------------------------------- */
/*                                                                                                */
/*                                   General Arduino Libraries                                    */
/*                                                                                                */
/* ---------------------------------------------------------------------------------------------- */
/* ############################################################################################## */

#include <functional>
#include <Arduino.h>
#include <arduino_homekit_server.h>



/* ############################### ~ cooper.bin @ makesmart.net ~ ############################### */
/* ---------------------------------------------------------------------------------------------- */
/*                                                                                                */
/*                                      Local Project Files                                       */
/*                                                                                                */
/* ---------------------------------------------------------------------------------------------- */
/* ############################################################################################## */


#include "wifi_info.h"



/* ############################################################################################## */
/* ---------------------------------------------------------------------------------------------- */
/*                                                                                                */
/*                                   Global Switch Variables                                      */
/*                                                                                                */
/* ---------------------------------------------------------------------------------------------- */
/* ############################################################################################## */


byte switchPin              = 2;

bool autoToggle             = false;
int autoToggleDelay         = 5000;

bool enableHardwareButton   = false;
byte hardwareButton         = 15;



/* ############################################################################################## */
/*              Some other variables to control delayed toggle | you can ignore this              */
/* ############################################################################################## */

bool currentSwitchState           = false;

unsigned long nextActionTime      = 0;
bool actionEnabled                = false;

const unsigned long debounceDelay = 50;
unsigned long lastDebounceTime    = 0;
bool buttonState                  = false;
bool lastButtonState              = false; 

void activateAutoToggle(unsigned long delay) {
    nextActionTime = millis() + delay;
    actionEnabled = true;
}

void readHardwareButton(){
  int reading = digitalRead(hardwareButton);
    if (reading != lastButtonState) { lastDebounceTime = millis(); }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
        notifyState(buttonState);
      }
    }
    lastButtonState = reading;
}



/* ############################################################################################## */
/* ---------------------------------------------------------------------------------------------- */
/*                                                                                                */
/*                                        setup() Function                                        */
/*                                                                                                */
/* ---------------------------------------------------------------------------------------------- */
/* ############################################################################################## */


void setup() {  
  Serial.begin(115200);
  pinMode(switchPin, OUTPUT);
  wifi_connect();
  my_homekit_setup();
}


     
void loop() {
  
  EVERY_N_MILLISECONDS( 10 ) { homekit_loop(); }  

  if(enableHardwareButton){ readHardwareButton(); }

  if (actionEnabled && millis() >= nextActionTime) {
        notifyState(!currentSwitchState); 
        actionEnabled = false;
  }


  

  
}






/* ############################################################################################## */
/* ---------------------------------------------------------------------------------------------- */
/*                                                                                                */
/*                       Homekit characteristics defined in my_accessory.c                        */
/*                                                                                                */
/* ---------------------------------------------------------------------------------------------- */
/* ############################################################################################## */


extern "C" homekit_server_config_t    accessory_config;
extern "C" homekit_characteristic_t   switch_state;




void my_homekit_setup() {
  
  /* ############################################################################################## */
  /*                                      SWITCH FUNCTIONS                                          */
  /* ############################################################################################## */
  
  switch_state.setter       = function_switch_state;


  
  arduino_homekit_setup(&accessory_config);

}

void homekit_loop() {  
  arduino_homekit_loop();
}



/* ############################################################################################## */
/* ---------------------------------------------------------------------------------------------- */
/*                                                                                                */
/*                                      Switch Functions                                          */
/*                                                                                                */
/* ---------------------------------------------------------------------------------------------- */
/* ############################################################################################## */


void function_switch_state(const homekit_value_t v) {
    bool isOn                       = v.bool_value;
    switch_state.value.bool_value   = isOn;

    currentSwitchState = isOn;
    setPinState(currentSwitchState);

    if(autoToggle){
      activateAutoToggle(autoToggleDelay);
    }

}


void setPinState(bool currentSwitchState){
  digitalWrite(switchPin, currentSwitchState ? HIGH : LOW);
}


void notifyState(bool state){

  setPinState(state);
  switch_state.value.bool_value = state;
  homekit_characteristic_notify(&switch_state, switch_state.value);

}