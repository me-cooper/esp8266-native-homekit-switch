# esp8266-native-homekit-switch
This sketch turns your ESP8266 into a native HomeKit switch. During pairing, you have the option to choose from a lamp, fan, or switch.

The project allows for the ESP8266 to be natively integrated into HomeKit. When switching, a function is called, which can be equipped with its own logic. For example, this could be controlling a relay.



To implement the logic, it simply needs to be done in the following function.

The variable `currentSwitchState` contains the current state as either `true` or `false`, indicating whether the switch is activated or not.

```arduino
void setPinState(bool currentSwitchState){
  digitalWrite(switchPin, currentSwitchState ? HIGH : LOW);
}
```



The sketch includes the following functionalities:

- Switching directly through the Home app or by voice via Siri, and integration into the smart home.
- (Optional) An auto toggle can be set, so that the switch reverts to its previous state after x milliseconds.
- (Optional) The switch can also be operated by a hardware button on the ESP8266. This not only activates the supposed relay but also transmits the status to HomeKit.



The only section you still need to edit is the following:



```arduino
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
```



- `byte switchPin` - GPIO Pin for the Relais e.g. you dont need this if you delete `digitalWrite(switchPin, currentSwitchState ? HIGH : LOW);` and implement own logic without digitalWrite



Optional:

- `autoToggle` - to en / disable auto toggle functionality
- `autoToggleDelay` - auto toggle delay in milliseconds



- `enableHardwareButton`  - Activate the hardware button function. This can be, for example, a push-button or a switch that is connected to the `hardwareButton` pin. If you push the button. HomeKit is notified and `setPinState()` is executed