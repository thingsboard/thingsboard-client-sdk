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
    m_size = mbedtls_type_to_size(type);
    // Initialize the context
    mbedtls_md_init(&m_ctx);
    // Choose the hash function
    bool init_result = mbedtls_md_setup(&m_ctx, mbedtls_md_info_from_type(type), 0) == 0;
    // Start the hash
    init_result = init_result && (mbedtls_md_starts(&m_ctx) == 0);
    return init_result;
}

bool HashGenerator::update(uint8_t const * data, size_t const & length) {
    return mbedtls_md_update(&m_ctx, data, length) == 0;
}

bool HashGenerator::finish(char * hash_string) {
    unsigned char byte_hash[m_size] = {};
    bool const success = mbedtls_md_finish(&m_ctx, byte_hash) == 0;
    if (!success) {
        return success;
    }
    for (size_t i = 0; i < m_size; ++i) {
        sprintf(hash_string + (i * 2), "%02x", byte_hash[i]);
    }
    hash_string[m_size * 2] = '\0';
    return success;
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

size_t HashGenerator::mbedtls_type_to_size(mbedtls_md_type_t const & type) {
    switch (type) {
#if MBEDTLS_VERSION_MAJOR < 3
        case mbedtls_md_type_t::MBEDTLS_MD_MD2: // Fallthrough same behaviour
        case mbedtls_md_type_t::MBEDTLS_MD_MD4:
#endif // MBEDTLS_VERSION_MAJOR < 3
        case mbedtls_md_type_t::MBEDTLS_MD_MD5:
            return 16U;
        case mbedtls_md_type_t::MBEDTLS_MD_SHA1: // Fallthrough same behaviour
        case mbedtls_md_type_t::MBEDTLS_MD_RIPEMD160:
            return 20U;
        case mbedtls_md_type_t::MBEDTLS_MD_SHA224:
            return 28U;
        case mbedtls_md_type_t::MBEDTLS_MD_SHA256:
            return 32U;
        case mbedtls_md_type_t::MBEDTLS_MD_SHA384:
            return 48U;
        case mbedtls_md_type_t::MBEDTLS_MD_SHA512:
            return 64U;
#if ((MBEDTLS_VERSION_MAJOR == 3 && MBEDTLS_VERSION_MINOR >= 5) || MBEDTLS_VERSION_MAJOR > 3)
        case mbedtls_md_type_t::MBEDTLS_MD_SHA3_224:
            return 28U;
        case mbedtls_md_type_t::MBEDTLS_MD_SHA3_256:
            return 32U;
        case mbedtls_md_type_t::MBEDTLS_MD_SHA3_384:
            return 48U;
        case mbedtls_md_type_t::MBEDTLS_MD_SHA3_512:
            return 64U;
#endif // ((MBEDTLS_VERSION_MAJOR == 3 && MBEDTLS_VERSION_MINOR >= 5) || MBEDTLS_VERSION_MAJOR > 3)
        case mbedtls_md_type_t::MBEDTLS_MD_NONE: // Fallthrough same behaviour
        default:
            return 0U;
    }
}
