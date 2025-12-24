#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "Block.h"
#include "MerkleTree.h"
#include "Transaction.h"
#include <cstddef>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

/**
 * @brief Manages the chain of blocks and the pending transaction pool.
 */
class Blockchain {
private:
  std::vector<Block> chain;
  std::priority_queue<Transaction> mempool;
  int difficulty;
  int transactionsPerBlock;

public:
  Blockchain() : difficulty(4), transactionsPerBlock(5) {
    // Create Genesis Block
    std::vector<Transaction> genesisTxs;
    genesisTxs.push_back(Transaction("System", "Genesis", 0, 0));
    Block genesis(0, "0", genesisTxs);
    genesis.merkleRoot = "GENESIS_ROOT";
    genesis.mineBlock(difficulty);
    chain.push_back(genesis);
  }

  /**
   * @brief Adds a transaction to the mempool.
   */
  void addTransaction(const Transaction &tx) { mempool.push(tx); }

  /**
   * @brief Gathers pending transactions from the mempool and mines them into a
   * new block.
   */
  void minePendingTransactions(const std::string &minerAddress) {
    std::vector<Transaction> txToMine;

    // Collect up to transactionsPerBlock from the priority queue
    int count = 0;
    while (!mempool.empty() && count < transactionsPerBlock) {
      txToMine.push_back(mempool.top());
      mempool.pop();
      count++;
    }

    if (txToMine.empty()) {
      std::cout << "No transactions to mine." << std::endl;
      return;
    }

    // Special coinbase transaction for the miner
    txToMine.push_back(Transaction("System", minerAddress, 50, 0));

    Block newBlock(static_cast<int>(chain.size()), chain.back().hash, txToMine);
    newBlock.mineBlock(difficulty);
    chain.push_back(newBlock);
  }

  /**
   * @brief Validates the integrity of the blockchain.
   */
  bool isChainValid() const {
    for (std::size_t i = 1; i < chain.size(); ++i) {
      const Block &currentBlock = chain[i];
      const Block &prevBlock = chain[i - 1];

      // 1. Check current block hash
      if (currentBlock.hash != currentBlock.calculateHash()) {
        std::cout << "Invalid hash at block " << i << std::endl;
        return false;
      }

      // 2. Check linkage to previous block
      if (currentBlock.previousHash != prevBlock.hash) {
        std::cout << "Invalid link at block " << i << std::endl;
        return false;
      }

      // 3. Check Merkle Root integrity
      if (currentBlock.merkleRoot !=
          MerkleTree::calculateMerkleRoot(currentBlock.transactions)) {
        std::cout << "Invalid Merkle Root at block " << i << std::endl;
        return false;
      }

      // 4. Check Proof of Work (leading zeros)
      std::string target(difficulty, '0');
      if (currentBlock.hash.length() < static_cast<std::size_t>(difficulty) ||
          currentBlock.hash.substr(0, difficulty) != target) {
        std::cout << "Block " << i << " was not mined correctly" << std::endl;
        return false;
      }
    }
    return true;
  }

  const std::vector<Block> &getChain() const { return chain; }

  /**
   * @brief DANGEROUS: For simulation/testing purposes only.
   */
  Block &getBlockMutable(std::size_t index) { return chain.at(index); }

  int getDifficulty() const { return difficulty; }
  void setDifficulty(int d) { difficulty = d; }
  int getMempoolSize() const { return static_cast<int>(mempool.size()); }
};

#endif // BLOCKCHAIN_H
