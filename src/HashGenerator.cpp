#if defined(ESP8266) || defined(ESP32)

// Header include.
#include <HashGenerator.h>

// Library includes.
#include <sstream>
#include <iomanip>

HashGenerator::HashGenerator(const mbedtls_md_type_t& type) {
    // Initialize the context
    mbedtls_md_init(&m_ctx);
    // Choose the hash function
    mbedtls_md_setup(&m_ctx, mbedtls_md_info_from_type(type), 0);
    // Start the hash
    mbedtls_md_starts(&m_ctx);
}

HashGenerator::~HashGenerator(void) {
    // Ensures to clean up the mbedtls memory after it has been used.
    mbedtls_md_free(&m_ctx);
}

const bool HashGenerator::update(const uint8_t* data, const size_t& len) {
    return mbedtls_md_update(&m_ctx, data, len) == 0;
}

const std::string HashGenerator::get_hash_string() {
    // Calculate the current hash value
    uint8_t hash[MBEDTLS_MD_MAX_SIZE];
    finish(hash);

    // Convert the hash value to a string
    std::stringstream ss;
    for (size_t i = 0; i < mbedtls_md_get_size(m_ctx.md_info); i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    return ss.str();
}

void HashGenerator::finish(unsigned char *hash) {
    mbedtls_md_finish(&m_ctx, hash);
}

#endif // defined(ESP8266) || defined(ESP32)
