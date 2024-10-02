#ifndef Hash_Generator_h
#define Hash_Generator_h

// Local includes.
#include "Configuration.h"

// Library includes.
#if THINGSBOARD_USE_MBED_TLS
#include <mbedtls/md.h>
#else
#include <Seeed_mbedtls.h>
#endif // THINGSBOARD_USE_MBED_TLS
#include <stdint.h>
#include <stddef.h>


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
    HashGenerator(void) = default;

    /// @brief Destructor
    ~HashGenerator(void);

    /// @brief Starts the hashing process
    /// @param type Supported type of hash that should be generated from this class
    /// @return Whether initalizing and starting the hash calculation was successful or not
    bool start(mbedtls_md_type_t const & type);

    /// @brief Update the current hash value with new data
    /// @param data Data that should be added to generate the hash
    /// @param length Length of data entered
    /// @return Whether updating the hash for the given bytes was successful or not
    bool update(uint8_t const * data, size_t const & length);

    /// @brief Calculates the final hash string representation and stops the hash calculation no further calls to update() will work,
    /// instead the same context can be reused to start another hash calculation operation with start()
    /// @param hash_string Output string that the hash string representation will be copied into, needs to be big enough to hold the string representation of the mbedtls_md_type_t
    /// meaning we require 2 bytes for every single byte of hash data, therefore the biggest hash string representation has a size of 128 bytes.
    /// For the actual hash being SHA256, requiring 256 bits = 64 bytes * 2 for the string representation resulting in 128 bytes + 1 byte for the null termination character.
    /// Recommended size of the array to pass is simply (MBEDTLS_MD_MAX_SIZE * 2) + 1, which will result in 129 bytes, but additionally automatically update as well if the underlying max size changes
    /// @return Whether stopping and caculating the final hash for the given bytes was successful or not
    bool finish(char * hash_string);

  private:
    /// @brief Frees all internally allocated memory to ensure no memory leak occurs, additionally check if a hash calculation was ever started,
    /// before freeing, because freeing without having started a hash calculation causes a crash.
    void free();

    /// @brief Calculates the amount of bytes needed for the hash output.
    /// Is used because we if we don't calculate the exact amount needed the final hash string representation, will always be filled up with hex 00 to the maximum size of the hash string representation being 129 bytes.
    /// Which results in hash string representation with a lot of additional 00 at the end, which might make the further processing of those strings fail
    /// @param type Supported type of hash that should be generated from this class
    /// @return Amount of bytes needed to be allocated by the buffer that will hold the final hash that is then transformed into a string
    size_t mbedtls_type_to_size(mbedtls_md_type_t const & type);

    size_t               m_size = {}; // Actual size in bytes, depend on the mbedtls_md_type_t given in the start method
    mbedtls_md_context_t m_ctx = {};  // Context used to access the already written bytes and update them latter
};

#endif // Hash_Generator_h
