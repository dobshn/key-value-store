#ifndef CRYPTO_UTIL_H
#define CRYPTO_UTIL_H

#include <string>

std::string encrypt(const std::string& plain);
std::string decrypt(const std::string& cipher_b64);

#endif
