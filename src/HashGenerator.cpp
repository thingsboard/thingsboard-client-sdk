// Header include.
#include "HashGenerator.h"

// Library include.
#include <stdio.h>

HashGenerator::~HashGenerator(void) {
    free();
}

bool HashGenerator::start(mbedtls_md_type_t const & type) {
    // Clear the internal structure of any previous attempt, because if we do not the init function will not work correctly
    free();
    // Initialize the context
    mbedtls_md_init(&m_ctx);
    // Choose the hash function
    bool init_result = mbedtls_md_setup(&m_ctx, mbedtls_md_info_from_type(type), 0) == 0;
    // Start the hash
    init_result = init_result && (mbedtls_md_starts(&m_ctx) == 0);
    return init_result;
}

bool HashGenerator::update(uint8_t const * const data, size_t const & length) {
    return mbedtls_md_update(&m_ctx, data, length) == 0;
}

bool HashGenerator::finish(char * hash_string) {
    unsigned char byte_hash[MBEDTLS_MD_MAX_SIZE] = {};
    bool const success = mbedtls_md_finish(&m_ctx, byte_hash) == 0;
    if (!success) {
        return success;
    }
    for (size_t i = 0; i < sizeof(byte_hash); ++i) {
        sprintf(hash_string + (i * 2), "%02x", byte_hash[i]);
    }
    hash_string[sizeof(byte_hash) * 2] = '\0';
}

void HashGenerator::free() {
    // MBEDTLS Version 3 is a major breaking changes were accessing the internal structures requires the MBEDTLS_PRIVATE macro
#if MBEDTLS_VERSION_MAJOR < 3
    if (m_ctx.hmac_ctx != nullptr && m_ctx.md_ctx != nullptr && m_ctx.md_info != nullptr) {
#else
    if (m_ctx.MBEDTLS_PRIVATE(hmac_ctx) != nullptr && m_ctx.MBEDTLS_PRIVATE(md_ctx) != nullptr && m_ctx.MBEDTLS_PRIVATE(md_info) != nullptr) {
#endif
        // Ensures to clean up the mbedtls memory after it has been used
        mbedtls_md_free(&m_ctx);
    }
}
