/*
  ThingsBoardDefaultLogger.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Thingsboard_Default_Logger_h
#define Thingsboard_Default_Logger_h

/// @brief Default logger class used by the ThingsBoard class to log messages into the console
class ThingsBoardDefaultLogger {
  public:
    /// @brief Logs the given message to the Serial console
    /// Ensure to initalize the Serial before calling this method (Serial.begin)
    /// @param msg Message we want to print into the console
    static void log(const char* msg);
};

#endif // Thingsboard_Default_Logger_h
