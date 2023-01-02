/*
  ThingsBoardDefaultLogger.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Thingsboard_Default_Logger_h
#define Thingsboard_Default_Logger_h

class ThingsBoardDefaultLogger {
public:
    // ThingsBoardDefaultLogger log method.
    static void log(const char* msg);
};

#endif // Thingsboard_Default_Logger_h
