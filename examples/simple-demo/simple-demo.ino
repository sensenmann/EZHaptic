/** Include Header-File: */
#include <Arduino.h>
#include <EZLog.h>
#include "EZHaptic.h"

#define VIBRATION_MOTOR_PIN     GPIO_NUM_14

EZHaptic haptic = EZHaptic(VIBRATION_MOTOR_PIN);

void setup() {
    /** Setting Serial Console */
    Serial.begin(115200);
    delay(1000);

    /** Create a simple loggingConfig: */
    LoggingConfig loggingConfig = {};
    loggingConfig.loglevel = Loglevel::VERBOSE;

    /** Setup EZLog: */
    Log::init(loggingConfig);

    haptic.begin();
}


void loop() {
    EZ_LOG(__FILE__);

    Log::infoln("Slow LongRing");
    haptic.play(WeeOoo);
    delay(5000);



    //
    // Log::infoln("Fast Pulse");
    // haptic.play(Pulse, 50);
    // delay(5000);
}
