/*
  HashGenerator.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Hash_Generator_h
#define Hash_Generator_h

#if defined(ESP8266) || defined(ESP32)

// Library includes.
#if defined(ESP8266)
#include <Seeed_mbedtls.h>
#else
#include <mbedtls/md.h>
#endif
#include <string>

/// @brief Allows generating a hash of the given type
/// with partial data and then retreiving the completed hash once it has been completed
class HashGenerator {
  public:
    /// @brief Constructor
    /// @param type Supported type of hash that should be generated from this class
    HashGenerator(const mbedtls_md_type_t& type);

    /// @brief Destructor
    ~HashGenerator(void);

    /// @brief Update the current hash value with new data
    /// @param data Data that should be added to generate the hash
    /// @param len Length of data entered
    /// @return Wheter updating the hash for the given bytes was successfull or not
    const bool update(const uint8_t* data, const size_t& len);

    /// @brief Returns the final hash value as a string
    /// @return String containing the final hash value for the passed bytes
    const std::string get_hash_string();

private:
    /// @brief Calculates the final hash value
    /// @param hash Output byte array that the hash value will be copied into
    void finish(unsigned char *hash);

    mbedtls_md_context_t m_ctx;
};

#endif // defined(ESP8266) || defined(ESP32)

#endif // Hash_Generator_h
