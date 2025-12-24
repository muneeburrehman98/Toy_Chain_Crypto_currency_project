#include "Dashboard.h"
#include "GuiApp.h"
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <time.h>
#include <vector>

void testPhase1() {
  std::cout << "\033[1;34m>>> PHASE 1: CRYPTOGRAPHY & TRANSACTIONS <<<\033[0m"
            << std::endl;
  std::string input = "ToyChain";
  std::string hash = sha256(input);
  std::cout << "SHA-256('" << input << "') = " << hash << std::endl;
  assert(hash.length() == 64);

  Transaction tx1("Alice", "Bob", 50.0, 0.5);
  Transaction tx2("Charlie", "Dave", 20.0, 1.0);
  assert(tx1 < tx2); // tx2 has higher fee
  std::cout << "Transaction serialization: " << tx1.toString() << std::endl;
  std::cout << "\033[1;32m[PASS]\033[0m Phase 1 tests successful.\n"
            << std::endl;
}

void testPhase2and3() {
  std::cout
      << "\033[1;34m>>> PHASE 2&3: CORE STRUCTURES & MERKLE TREE <<<\033[0m"
      << std::endl;
  Blockchain toychain;
  toychain.setDifficulty(2);

  std::cout << "Adding transactions and mining Block 1..." << std::endl;
  toychain.addTransaction(Transaction("UserA", "UserB", 10.5, 0.1));
  toychain.addTransaction(Transaction("UserC", "UserA", 5.0, 0.2));
  toychain.minePendingTransactions("Miner01");

  if (toychain.getChain().size() > 1) {
    std::cout << "Block 1 Merkle Root: " << toychain.getChain()[1].merkleRoot
              << std::endl;
  }
  assert(toychain.isChainValid());
  std::cout << "\033[1;32m[PASS]\033[0m Phase 2 & 3 tests successful.\n"
            << std::endl;
}

void testSimulation() {
  std::cout << "\033[1;34m>>> PHASE 4: SYSTEM SIMULATION <<<\033[0m"
            << std::endl;
  Blockchain bc;
  bc.setDifficulty(3);

  std::cout << "Running automatic simulation..." << std::endl;
  for (int i = 0; i < 2; ++i) {
    bc.addTransaction(Transaction("Alice", "Bob", 15.0, 0.1));
    bc.addTransaction(Transaction("Bob", "Charlie", 5.0, 0.5));
    bc.minePendingTransactions("Node_" + std::to_string(i));
  }

  Dashboard::renderChain(bc);
  assert(bc.isChainValid());
  std::cout << "\033[1;32m[PASS]\033[0m Phase 4 simulation successful.\n"
            << std::endl;
}

void testTamperDetection() {
  std::cout << "\033[1;34m>>> PHASE 5: TAMPER DETECTION <<<\033[0m"
            << std::endl;
  Blockchain bc;
  bc.setDifficulty(2);

  bc.addTransaction(Transaction("Alice", "Bob", 100.0, 1.0));
  bc.minePendingTransactions("MinerW");

  std::cout << "Initial validation: " << (bc.isChainValid() ? "OK" : "FAILED")
            << std::endl;
  assert(bc.isChainValid());

  std::cout << "\033[1;31m[TAMPERING]\033[0m Changing amount in Block 1..."
            << std::endl;
  if (bc.getChain().size() > 1) {
    bc.getBlockMutable(1).transactions[0].amount = 9999.0;
  }

  std::cout << "Validation after tamper: "
            << (bc.isChainValid() ? "OK" : "\033[1;31mDETECTED\033[0m")
            << std::endl;
  assert(!bc.isChainValid());
  std::cout << "\033[1;32m[PASS]\033[0m Tamper detection working as expected.\n"
            << std::endl;
}

int main() {
  srand(static_cast<unsigned int>(time(nullptr)));

  try {
    testPhase1();
    testPhase2and3();
    testSimulation();
    testTamperDetection();

    std::cout << "\nLaunch GUI Dashboard? (y/n): ";
    char choice;
    std::cin >> choice;

    if (choice == 'y' || choice == 'Y') {
      Blockchain guiBC;
      guiBC.setDifficulty(2);

      // wxWidgets initialization
      int argc = 0;
      char **argv = NULL;
      wxEntryStart(argc, argv);

      BlockchainFrame *frame = new BlockchainFrame(guiBC);
      frame->Show(true);

      wxTheApp->MainLoop();
      wxEntryCleanup();
    }

    std::cout
        << "\033[1;36m==================================================\033[0m"
        << std::endl;
    std::cout
        << "\033[1;32m     ALL TOYCHAIN CORE TESTS PASSED SUCCESSFULLY! \033[0m"
        << std::endl;
    std::cout
        << "\033[1;36m==================================================\033[0m"
        << std::endl;
    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();
  } catch (const std::exception &e) {
    std::cerr << "\n\033[1;31m[CRITICAL ERROR] \033[0m" << e.what()
              << std::endl;
    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();
    return 1;
  }

  return 0;
}
