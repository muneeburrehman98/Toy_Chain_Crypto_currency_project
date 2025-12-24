#ifndef BLOCK_H
#define BLOCK_H

#include "MerkleTree.h"
#include "Transaction.h"
#include "Utils.h"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>

/**
 * @brief Represents a single block in the blockchain.
 */
class Block {
public:
  int index;
  time_t timestamp;
  std::string previousHash;
  std::string merkleRoot;
  long long nonce;
  std::vector<Transaction> transactions;
  std::string hash;

  Block(int idx, const std::string &prevHash,
        const std::vector<Transaction> &txs)
      : index(idx), previousHash(prevHash), transactions(txs), nonce(0) {
    timestamp = time(nullptr);
    merkleRoot = MerkleTree::calculateMerkleRoot(transactions);
    hash = calculateHash();
  }

  std::string calculateHash() const {
    std::stringstream ss;
    ss << index << static_cast<long long>(timestamp) << previousHash
       << merkleRoot << nonce;
    for (const auto &tx : transactions) {
      ss << tx.toString();
    }
    return sha256(ss.str());
  }

  void mineBlock(int difficulty) {
    std::string target(difficulty, '0');
    while (hash.substr(0, difficulty) != target) {
      nonce++;
      hash = calculateHash();
    }
    std::cout << "Block Mined! Hash: " << hash << std::endl;
  }
};

#endif // BLOCK_H
