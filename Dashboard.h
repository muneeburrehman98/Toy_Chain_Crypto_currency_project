#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "Blockchain.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


/**
 * @brief Provides a rich console visualizer for the ToyChain blockchain.
 */
class Dashboard {
public:
  static void renderHeader(const std::string &title) {
    std::cout << "\n\033[1;36m================================================="
                 "=\033[0m"
              << std::endl;
    std::cout << "\033[1;37m        " << title << "\033[0m" << std::endl;
    std::cout
        << "\033[1;36m==================================================\033[0m"
        << std::endl;
  }

  static void renderBlock(const Block &block) {
    std::cout << "\n\033[1;33m[ BLOCK #" << block.index << " ]\033[0m"
              << std::endl;
    std::cout << "\033[1;34m  Hash: \033[0m" << block.hash << std::endl;
    std::cout << "\033[1;34m  Prev: \033[0m" << block.previousHash << std::endl;
    std::cout << "\033[1;34m  Merkle Root: \033[0m" << block.merkleRoot
              << std::endl;
    std::cout << "\033[1;34m  Nonce: \033[0m" << block.nonce << std::endl;
    std::cout << "\033[1;34m  Transactions: \033[0m"
              << block.transactions.size() << std::endl;

    for (const auto &tx : block.transactions) {
      std::cout << "    - " << tx.sender << " -> " << tx.receiver
                << " : \033[1;32m" << tx.amount << " TC\033[0m (Fee: " << tx.fee
                << ")" << std::endl;
    }
    std::cout
        << "\033[1;36m--------------------------------------------------\033[0m"
        << std::endl;
  }

  static void renderChain(const Blockchain &bc) {
    renderHeader("TOYCHAIN BLOCKCHIAIN VISUALIZER");
    for (const auto &block : bc.getChain()) {
      renderBlock(block);
    }
    std::cout << "\n\033[1;35m  CHAIN STATUS: \033[0m"
              << (bc.isChainValid() ? "\033[1;32mVALID\033[0m"
                                    : "\033[1;31mINVALID/TAMPERED\033[0m")
              << std::endl;
    std::cout
        << "\033[1;36m==================================================\033[0m"
        << std::endl;
  }
};

#endif // DASHBOARD_H
