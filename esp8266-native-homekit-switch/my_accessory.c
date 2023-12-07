#include <homekit/homekit.h>
#include <homekit/characteristics.h>


void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
} 



/* NORMAL SWITCH SETTINGS */
homekit_characteristic_t switch_state                 = HOMEKIT_CHARACTERISTIC_(ON,           false);
homekit_characteristic_t switch_name                  = HOMEKIT_CHARACTERISTIC_(NAME,         "makesmart Switch");



homekit_accessory_t *accessories[] = {
  
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
    
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "makesmart Switch"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "makesmart Community"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),

        HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
          &switch_state,
          &switch_name,
          NULL
        }),

        NULL
    }),

       
  NULL
};


homekit_server_config_t accessory_config = {
    .accessories = accessories,
    .password = "123-45-678"
};