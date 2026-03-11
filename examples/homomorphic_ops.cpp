/**
 * Homomorphic Operations Example
 * 
 * Demonstrates addition and multiplication on encrypted data
 */

#include <iostream>
#include <vector>
#include "fxbl/core.hpp"
#include "fxbl/homomorphic.hpp"

using namespace FXBL;

int main() {
    std::cout << "NeuroCrypt-X Homomorphic Operations\n";
    std::cout << "====================================\n\n";
    
    // Initialize
    Protocol nc;
    auto sk = nc.generate_secret_key();
    auto pk = nc.generate_public_key(sk);
    auto evk = nc.generate_evaluation_key(sk);
    
    std::cout << "Keys generated (secret, public, evaluation)\n\n";
    
    // ===== Homomorphic Addition =====
    std::cout << "=== Homomorphic Addition ===\n\n";
    
    Plaintext pt1{{10, 20, 30}};
    Plaintext pt2{{5, 15, 25}};
    
    std::cout << "Plaintext 1: [10, 20, 30]\n";
    std::cout << "Plaintext 2: [5, 15, 25]\n\n";
    
    // Encrypt both
    auto ct1 = nc.encrypt(pk, pt1);
    auto ct2 = nc.encrypt(pk, pt2);
    std::cout << "✓ Both plaintexts encrypted\n";
    
    // Add ciphertexts
    auto ct_sum = nc.add(ct1, ct2);
    std::cout << "✓ Ciphertexts added (homomorphically)\n";
    
    // Decrypt result
    auto result = nc.decrypt(sk, ct_sum);
    std::cout << "✓ Result decrypted\n\n";
    
    std::cout << "Result: [";
    for (size_t i = 0; i < result.data.size(); ++i) {
        std::cout << result.data[i];
        if (i < result.data.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "Expected: [15, 35, 55]\n\n";
    
    // ===== Homomorphic Multiplication =====
    std::cout << "=== Homomorphic Multiplication ===\n\n";
    
    Plaintext pt3{{3, 7}};
    Plaintext pt4{{2, 4}};
    
    std::cout << "Plaintext 3: [3, 7]\n";
    std::cout << "Plaintext 4: [2, 4]\n\n";
    
    auto ct3 = nc.encrypt(pk, pt3);
    auto ct4 = nc.encrypt(pk, pt4);
    std::cout << "✓ Both plaintexts encrypted\n";
    
    // Multiply ciphertexts (requires evaluation key)
    auto ct_product = nc.multiply(ct3, ct4, evk);
    std::cout << "✓ Ciphertexts multiplied (homomorphically)\n";
    
    auto result_mult = nc.decrypt(sk, ct_product);
    std::cout << "✓ Result decrypted\n\n";
    
    std::cout << "Result: [";
    for (size_t i = 0; i < result_mult.data.size(); ++i) {
        std::cout << result_mult.data[i];
        if (i < result_mult.data.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "Expected: [6, 28]\n\n";
    
    std::cout << "Homomorphic operations completed successfully!\n";
    std::cout << "\nKey insight: All computations were performed on ENCRYPTED data.\n";
    std::cout << "The server never saw the plaintext values!\n";
    
    return 0;
}
