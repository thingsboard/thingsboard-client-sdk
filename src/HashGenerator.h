#ifndef Hash_Generator_h
#define Hash_Generator_h

// Local includes.
#include "Configuration.h"

#if THINGSBOARD_ENABLE_OTA

// Library includes.
#if THINGSBOARD_USE_MBED_TLS
#include <mbedtls/md.h>
#else
#include <Seeed_mbedtls.h>
#endif // THINGSBOARD_USE_MBED_TLS
#include <string>


/// @brief Wrapper class which allows generating a hash of the given type from any arbitrary byte payload, which is hashable in chunks.
/// The class wraps around either the Arduino Seeed mbedtls library from Seed Studio (https://github.com/Seeed-Studio/Seeed_Arduino_mbedtls) or the offical ESP Mbed TLS implementation from Mbed TLS (https://github.com/Mbed-TLS/mbedtls), the latter takes precendence if it exists.
/// This is done because it removes the need to include another library, because the component already exists on the system and we can therefore simply utilize that one.
/// The ESP Mbed TLS implementationt works with both Espressif IDF v4.X and v5.X, meaning it is version idependent, this is the case
/// because depending on the used version the implementation automatically adjusts to still initalize correctly.
/// The class instance is meant to be started with start() which will then create the configuration for a hash of the given type
/// and we then expect the complete binary payload to be called in multiple calls to update() and the final result to be read with get_hash_string()
/// Documentation about the specific use and caviates of the ESP Mbedt TLS implementation can be found here https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mbedtls.html
class HashGenerator {
  public:
    /// @brief Constructor
    HashGenerator(void);

    /// @brief Destructor
    ~HashGenerator(void);

    /// @brief Starts the hashing process
    /// @param type Supported type of hash that should be generated from this class
    void start(const mbedtls_md_type_t& type);

    /// @brief Update the current hash value with new data
    /// @param data Data that should be added to generate the hash
    /// @param len Length of data entered
    /// @return Whether updating the hash for the given bytes was successful or not
    bool update(const uint8_t* data, const size_t& len);

    /// @brief Returns the final hash value as a string
    /// @return String containing the final hash value for the passed bytes
    std::string get_hash_string();

  private:
    mbedtls_md_context_t m_ctx; // Context used to access the already written bytes and update them latter

    /// @brief Calculates the final hash value
    /// @param hash Output byte array that the hash value will be copied into
    void finish(unsigned char *hash);
};

#endif // THINGSBOARD_ENABLE_OTA

#endif // Hash_Generator_h
