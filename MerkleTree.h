#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include "Transaction.h"
#include "Utils.h"
#include <cstddef>
#include <string>
#include <vector>


/**
 * @brief Utility for calculating the Merkle Root of a set of transactions.
 */
class MerkleTree {
public:
  static std::string calculateMerkleRoot(const std::vector<Transaction> &txs) {
    if (txs.empty()) {
      return sha256("");
    }

    std::vector<std::string> layer;
    for (const auto &tx : txs) {
      layer.push_back(sha256(tx.toString()));
    }

    return computeRoot(layer);
  }

private:
  static std::string computeRoot(std::vector<std::string> hashes) {
    if (hashes.empty())
      return "";
    if (hashes.size() == 1)
      return hashes[0];

    if (hashes.size() % 2 != 0) {
      hashes.push_back(hashes.back());
    }

    std::vector<std::string> nextLayer;
    for (std::size_t i = 0; i < hashes.size(); i += 2) {
      nextLayer.push_back(sha256(hashes[i] + hashes[i + 1]));
    }

    return computeRoot(nextLayer);
  }
};

#endif // MERKLE_TREE_H
