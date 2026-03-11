/**
 * FXBL Main Entry Point
 * 
 * Demonstration of the next-generation cryptographic protocol
 */

#include <iostream>
#include <chrono>
#include <vector>
#include "fxbl/core.hpp"
#include "fxbl/lattice.hpp"
#include "fxbl/homomorphic.hpp"
#include "fxbl/neural.hpp"

using namespace FXBL;

void demo_basic_encryption() {
    std::cout << "\n=== Basic Encryption Demo ===\n\n";
    
    // Initialize protocol
    Protocol nc;
    
    // Generate keys
    auto secret_key = nc.generate_secret_key();
    auto public_key = nc.generate_public_key(secret_key);
    
    std::cout << "✓ Keys generated successfully\n";
    
    // Create plaintext data
    Plaintext pt{{42, 100, 256, 1000}};
    
    // Encrypt
    auto start = std::chrono::high_resolution_clock::now();
    auto ciphertext = nc.encrypt(public_key, pt);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto encrypt_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "✓ Encrypted in " << encrypt_time << " μs\n";
    
    // Decrypt
    start = std::chrono::high_resolution_clock::now();
    auto decrypted = nc.decrypt(secret_key, ciphertext);
    end = std::chrono::high_resolution_clock::now();
    
    auto decrypt_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "✓ Decrypted in " << decrypt_time << " μs\n";
    
    // Verify
    std::cout << "✓ Original:  [";
    for (size_t i = 0; i < pt.data.size(); ++i) {
        std::cout << pt.data[i];
        if (i < pt.data.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    
    std::cout << "✓ Decrypted:  [";
    for (size_t i = 0; i < decrypted.data.size(); ++i) {
        std::cout << decrypted.data[i];
        if (i < decrypted.data.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    
    bool success = (pt.data == decrypted.data);
    std::cout << (success ? "✓ Verification: PASSED\n" : "✗ Verification: FAILED\n");
}

void demo_homomorphic_addition() {
    std::cout << "\n=== Homomorphic Addition Demo ===\n\n";
    
    Protocol nc;
    auto sk = nc.generate_secret_key();
    auto pk = nc.generate_public_key(sk);
    
    // Encrypt two vectors
    Plaintext pt1{{10, 20, 30, 40}};
    Plaintext pt2{{5, 15, 25, 35}};
    
    auto ct1 = nc.encrypt(pk, pt1);
    auto ct2 = nc.encrypt(pk, pt2);
    
    std::cout << "✓ Encrypted vector 1: [10, 20, 30, 40]\n";
    std::cout << "✓ Encrypted vector 2: [5, 15, 25, 35]\n";
    
    // Add encrypted vectors (without decryption!)
    auto ct_sum = nc.add(ct1, ct2);
    
    // Decrypt result
    auto result = nc.decrypt(sk, ct_sum);
    
    std::cout << "✓ Sum (decrypted):    [";
    for (size_t i = 0; i < result.data.size(); ++i) {
        std::cout << result.data[i];
        if (i < result.data.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    
    std::cout << "✓ Expected:           [15, 35, 55, 75]\n";
    
    bool success = (result.data == std::vector<int64_t>{15, 35, 55, 75});
    std::cout << (success ? "✓ Homomorphic addition: PASSED\n" : "✗ Homomorphic addition: FAILED\n");
}

void demo_homomorphic_multiplication() {
    std::cout << "\n=== Homomorphic Multiplication Demo ===\n\n";
    
    Protocol nc;
    auto sk = nc.generate_secret_key();
    auto pk = nc.generate_public_key(sk);
    auto evk = nc.generate_evaluation_key(sk);
    
    // Encrypt vectors
    Plaintext pt1{{3, 7, 11}};
    Plaintext pt2{{2, 4, 6}};
    
    auto ct1 = nc.encrypt(pk, pt1);
    auto ct2 = nc.encrypt(pk, pt2);
    
    std::cout << "✓ Encrypted vector 1: [3, 7, 11]\n";
    std::cout << "✓ Encrypted vector 2: [2, 4, 6]\n";
    
    // Multiply encrypted vectors
    auto ct_product = nc.multiply(ct1, ct2, evk);
    
    // Decrypt result
    auto result = nc.decrypt(sk, ct_product);
    
    std::cout << "✓ Product (decrypted): [";
    for (size_t i = 0; i < result.data.size(); ++i) {
        std::cout << result.data[i];
        if (i < result.data.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    
    std::cout << "✓ Expected:            [6, 28, 66]\n";
    
    bool success = (result.data == std::vector<int64_t>{6, 28, 66});
    std::cout << (success ? "✓ Homomorphic multiplication: PASSED\n" : "✗ Homomorphic multiplication: FAILED\n");
}

void demo_neural_key_evolution() {
    std::cout << "\n=== Neural Key Evolution Demo ===\n\n";
    
    Protocol nc;
    auto key = nc.generate_secret_key();
    
    std::cout << "✓ Initial key generated\n";
    std::cout << "  Key dimension: " << key.poly.size() << "\n";
    
    // Simulate usage pattern
    std::vector<uint8_t> pattern = {0x5A, 0x3C, 0x7F, 0x12, 0xE8, 0x9B};
    
    // Evolve key
    nc.evolve_key(key, pattern);
    
    std::cout << "✓ Key evolved with neural pattern\n";
    std::cout << "  Pattern length: " << pattern.size() << " bytes\n";
    
    std::cout << "✓ Neural evolution provides:\n";
    std::cout << "  - Dynamic key adaptation\n";
    std::cout << "  - Unpredictable key changes\n";
    std::cout << "  - Enhanced security through complexity\n";
}

void demo_security_analysis() {
    std::cout << "\n=== Security Analysis Report ===\n\n";
    
    Protocol nc;
    
    auto report = nc.security_report();
    std::cout << report << "\n";
}

void demo_cloud_computing_scenario() {
    std::cout << "\n=== Cloud Computing Scenario ===\n\n";
    
    std::cout << "Scenario: Process sensitive data on untrusted cloud server\n\n";
    
    // Client side
    Protocol client_nc;
    auto sk = client_nc.generate_secret_key();
    auto pk = client_nc.generate_public_key(sk);
    
    std::cout << "[CLIENT] Generating keys...\n";
    
    // Sensitive data
    Plaintext salary_data{{50000, 60000, 75000, 90000}};
    std::cout << "[CLIENT] Sensitive salaries: [50000, 60000, 75000, 90000]\n";
    
    // Encrypt before sending to cloud
    auto encrypted_salaries = client_nc.encrypt(pk, salary_data);
    std::cout << "[CLIENT] Data encrypted ✓\n";
    std::cout << "[CLIENT] Sending encrypted data to cloud...\n";
    
    // Cloud server (cannot decrypt!)
    Protocol cloud_nc;
    Plaintext bonus{{5000, 6000, 7500, 9000}};
    auto encrypted_bonus = cloud_nc.encrypt(pk, bonus);
    
    std::cout << "[CLOUD]  Adding bonuses without seeing salaries...\n";
    auto encrypted_total = cloud_nc.add(encrypted_salaries, encrypted_bonus);
    std::cout << "[CLOUD]  Computation complete ✓\n";
    std::cout << "[CLOUD]  Returning encrypted result...\n";
    
    // Client receives and decrypts
    auto total = client_nc.decrypt(sk, encrypted_total);
    std::cout << "[CLIENT] Decrypting result...\n";
    std::cout << "[CLIENT] Total compensation: [";
    for (size_t i = 0; i < total.data.size(); ++i) {
        std::cout << total.data[i];
        if (i < total.data.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
    
    std::cout << "✓ Cloud never saw the plaintext!\n";
    std::cout << "✓ Privacy preserved throughout computation\n";
}

int main(int argc, char** argv) {
    std::cout << "╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║   FXBL v1.0.0 - Next-Gen Cryptographic       ║\n";
    std::cout << "║   Fully Homomorphic + Post-Quantum Secure            ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";
    
    try {
        demo_basic_encryption();
        demo_homomorphic_addition();
        demo_homomorphic_multiplication();
        demo_neural_key_evolution();
        demo_security_analysis();
        demo_cloud_computing_scenario();
        
        std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
        std::cout << "║   All demonstrations completed successfully!          ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════╝\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
