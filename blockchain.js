/**
 * ToyChain Core Logic (JS/TS Port)
 */

class Transaction {
    constructor(sender, receiver, amount, fee) {
        this.sender = sender;
        this.receiver = receiver;
        this.amount = amount;
        this.fee = fee;
        this.timestamp = Date.now();
    }

    toString() {
        return `${this.sender}${this.receiver}${this.amount}${this.fee}`;
    }
}

class MerkleTree {
    static calculateMerkleRoot(transactions) {
        if (transactions.length === 0) return "0";
        let hashes = transactions.map(tx => CryptoJS.SHA256(tx.toString()).toString());

        while (hashes.length > 1) {
            let nextLevel = [];
            for (let i = 0; i < hashes.length; i += 2) {
                if (i + 1 < hashes.length) {
                    nextLevel.push(CryptoJS.SHA256(hashes[i] + hashes[i + 1]).toString());
                } else {
                    nextLevel.push(hashes[i]);
                }
            }
            hashes = nextLevel;
        }
        return hashes[0];
    }
}

class Block {
    constructor(index, previousHash, transactions) {
        this.index = index;
        this.previousHash = previousHash;
        this.transactions = transactions;
        this.timestamp = Date.now();
        this.nonce = 0;
        this.merkleRoot = MerkleTree.calculateMerkleRoot(transactions);
        this.hash = this.calculateHash();
    }

    calculateHash() {
        return CryptoJS.SHA256(
            this.index +
            this.previousHash +
            this.timestamp +
            this.nonce +
            this.merkleRoot
        ).toString();
    }

    mineBlock(difficulty) {
        const target = Array(difficulty + 1).join("0");
        while (this.hash.substring(0, difficulty) !== target) {
            this.nonce++;
            this.hash = this.calculateHash();
        }
        console.log(`Block Mined: ${this.hash}`);
    }
}

class Blockchain {
    constructor() {
        this.chain = [this.createGenesisBlock()];
        this.difficulty = 2;
        this.mempool = [];
        this.transactionsPerBlock = 3;
        this.miningReward = 50;
    }

    createGenesisBlock() {
        const genesisTx = new Transaction("System", "Genesis", 0, 0);
        const block = new Block(0, "0", [genesisTx]);
        block.merkleRoot = "GENESIS_ROOT";
        block.hash = block.calculateHash();
        return block;
    }

    getLatestBlock() {
        return this.chain[this.chain.length - 1];
    }

    addTransaction(tx) {
        this.mempool.push(tx);
        // Sort pool by fee (highest first) like the C++ priority queue
        this.mempool.sort((a, b) => b.fee - a.fee);
    }

    minePendingTransactions(minerAddress) {
        const txToMine = this.mempool.splice(0, this.transactionsPerBlock);

        if (txToMine.length === 0) return null;

        // Add reward for miner
        txToMine.push(new Transaction("System", minerAddress, this.miningReward, 0));

        const newBlock = new Block(
            this.chain.length,
            this.getLatestBlock().hash,
            txToMine
        );

        newBlock.mineBlock(this.difficulty);
        this.chain.push(newBlock);
        return newBlock;
    }

    isChainValid() {
        for (let i = 1; i < this.chain.length; i++) {
            const currentBlock = this.chain[i];
            const prevBlock = this.chain[i - 1];

            if (currentBlock.hash !== currentBlock.calculateHash()) return false;
            if (currentBlock.previousHash !== prevBlock.hash) return false;
            if (currentBlock.merkleRoot !== MerkleTree.calculateMerkleRoot(currentBlock.transactions)) return false;
        }
        return true;
    }
}
