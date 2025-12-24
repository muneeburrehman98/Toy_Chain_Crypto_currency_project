#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>


/**
 * @brief Represents a value transfer in the ToyChain system.
 */
struct Transaction {
  std::string sender;
  std::string receiver;
  double amount;
  double fee;
  time_t timestamp;

  Transaction(std::string s, std::string r, double amt, double f)
      : sender(s), receiver(r), amount(amt), fee(f) {
    timestamp = time(nullptr);
  }

  /**
   * @brief Serializes transaction data for hashing.
   */
  std::string toString() const {
    std::stringstream ss;
    ss << sender << receiver << std::fixed << std::setprecision(8) << amount
       << fee << static_cast<long long>(timestamp);
    return ss.str();
  }

  /**
   * @brief Enables Priority Queue sorting by highest fee first.
   */
  bool operator<(const Transaction &other) const {
    if (fee == other.fee) {
      return timestamp > other.timestamp; // FIFO for same fee
    }
    return fee < other.fee; // Max-heap: largest fee at top
  }
};

#endif // TRANSACTION_H
