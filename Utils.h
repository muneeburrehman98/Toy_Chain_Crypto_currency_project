#ifndef UTILS_H
#define UTILS_H

#include "picosha2.h"
#include <string>

/**
 * @brief Utility function to calculate SHA-256 hash of a string.
 */
inline std::string sha256(const std::string &input) {
  return picosha2::hash256_hex_string(input);
}

#endif // UTILS_H
