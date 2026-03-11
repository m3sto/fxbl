/**
 * Basic Usage Example for FXBL
 * 
 * Demonstrates fundamental encryption and decryption operations
 */

#include <iostream>
#include <vector>
#include "fxbl/core.hpp"

using namespace FXBL;

int main() {
    std::cout << "NeuroCrypt-X Basic Usage Example\n";
    std::cout << "================================\n\n";
    
    // Step 1: Initialize the protocol
    Protocol nc;
    std::cout << "[1] Initializing NeuroCrypt protocol...\n";
    
    // Step 2: Generate cryptographic keys
    auto secret_key = nc.generate_secret_key();
    auto public_key = nc.generate_public_key(secret_key);
    std::cout << "[2] Generated keypair (secret + public)\n";
    
    // Step 3: Prepare data to encrypt
    std::vector<int64_t> sensitive_data = {42, 100, 256, 1000, 9999};
    Plaintext plaintext{sensitive_data};
    std::cout << "[3] Prepared plaintext: ";
    for (const auto& val : sensitive_data) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    
    // Step 4: Encrypt the data
    std::cout << "[4] Encrypting data...\n";
    auto ciphertext = nc.encrypt(public_key, plaintext);
    std::cout << "    ✓ Encryption complete\n";
    
    // Step 5: Decrypt the data
    std::cout << "[5] Decrypting data...\n";
    auto decrypted = nc.decrypt(secret_key, ciphertext);
    std::cout << "    ✓ Decryption complete\n";
    
    // Step 6: Verify correctness
    std::cout << "[6] Verifying result...\n";
    bool success = (plaintext.data == decrypted.data);
    
    if (success) {
        std::cout << "    ✓ SUCCESS: Decrypted data matches original!\n";
    } else {
        std::cout << "    ✗ FAILED: Data mismatch!\n";
        return 1;
    }
    
    // Step 7: Display protocol information
    std::cout << "\n[7] Protocol Information:\n";
    std::cout << nc.get_info() << "\n";
    
    std::cout << "Example completed successfully!\n";
    return 0;
}
