/*
  Configuration.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Configuration_h
#define Configuration_h

// Enable the usage of the STL library, depending on if needed STL base functionality is supported
#  ifdef __has_include
#    if __has_include(<string>) && __has_include(<functional>) && __has_include(<vector>) && __has_include(<iterator>) && __has_include(<cassert>)
#      define THINGSBOARD_ENABLE_STL 1
#    else
#      define THINGSBOARD_ENABLE_STL 0
#    endif
#  else
#    ifdef ARDUINO
#      define THINGSBOARD_ENABLE_STL 0
#    else
#      define THINGSBOARD_ENABLE_STL 1
#    endif
#  endif

#endif // Configuration_h
