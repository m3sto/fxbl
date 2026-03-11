**
 * @version 1.0
 * @author m3sto
 */
/**
 * @version 1.0
 * @author m3sto
 * FXBL Core Protocol Header
 * @version 1.0
 * @author m3sto
 * 
 * @version 1.0
 * @author m3sto
 * Main interface for the FXBL cryptographic protocol
 * @version 1.0
 * @author m3sto
 * Combining lattice-based security with homomorphic capabilities
 * @version 1.0
 * @author m3sto
 */

#ifndef FXBL_CORE_HPP
#define FXBL_CORE_HPP

#include <vector>
#include <array>
#include <cstdint>
#include <string>
#include <memory>

namespace FXBL {

// Security parameters
struct SecurityParams {
    size_t ring_dimension = 4096;      // n: polynomial ring degree
    uint64_t modulus = 1ULL << 60;     // q: coefficient modulus
    double noise_std_dev = 3.2;        // σ: Gaussian noise standard deviation
    size_t plaintext_modulus = 65537;  // t: plaintext modulus (prime)
    
    int classical_security_bits() const { return 256; }
    int post_quantum_security_bits() const { return 128; }
};

// Ciphertext structure
struct Ciphertext {
    std::vector<uint64_t> c0;  // First polynomial component
    std::vector<uint64_t> c1;  // Second polynomial component
    size_t level = 0;          // Multiplicative depth remaining
    SecurityParams params;
};

// Plaintext structure  
struct Plaintext {
    std::vector<int64_t> data;
    SecurityParams params;
};

// Key types
struct SecretKey {
    std::vector<int64_t> poly;  // Secret polynomial
    SecurityParams params;
};

struct PublicKey {
    std::vector<uint64_t> a;    // Random public polynomial
    std::vector<uint64_t> b;    // b = a*s + e
    SecurityParams params;
};

struct EvaluationKey {
    std::vector<Ciphertext> key_switching_table;
    SecurityParams params;
};

// Main Protocol Class
class Protocol {
public:
    explicit Protocol(const SecurityParams& params = SecurityParams());
    ~Protocol();
    
    // ========== Key Generation ==========
    
    /**
     * Generate neural-evolving secret key
     * Uses pattern-based key derivation for dynamic evolution
     */
    SecretKey generate_secret_key();
    
    /**
     * Generate corresponding public key
     */
    PublicKey generate_public_key(const SecretKey& sk);
    
    /**
     * Generate evaluation key for homomorphic multiplication
     */
    EvaluationKey generate_evaluation_key(const SecretKey& sk);
    
    /**
     * Neural key evolution - evolves key based on usage pattern
     */
    void evolve_key(SecretKey& sk, const std::vector<uint8_t>& pattern);
    
    // ========== Encryption / Decryption ==========
    
    /**
     * Encrypt plaintext using Ring-LWE based scheme
     */
    Ciphertext encrypt(const PublicKey& pk, const Plaintext& pt);
    
    /**
     * Decrypt ciphertext
     */
    Plaintext decrypt(const SecretKey& sk, const Ciphertext& ct);
    
    // ========== Homomorphic Operations ==========
    
    /**
     * Add two ciphertexts (homomorphic addition)
     * Result encrypts sum of plaintexts
     */
    Ciphertext add(const Ciphertext& ct1, const Ciphertext& ct2);
    
    /**
     * Subtract two ciphertexts
     */
    Ciphertext subtract(const Ciphertext& ct1, const Ciphertext& ct2);
    
    /**
     * Multiply two ciphertexts (homomorphic multiplication)
     * Requires evaluation key for relinearization
     */
    Ciphertext multiply(const Ciphertext& ct1, const Ciphertext& ct2, 
                       const EvaluationKey& evk);
    
    /**
     * Multiply ciphertext by plaintext
     */
    Ciphertext multiply_plain(const Ciphertext& ct, const Plaintext& pt);
    
    // ========== Advanced Operations ==========
    
    /**
     * Bootstrap ciphertext to restore noise budget
     * Enables unlimited homomorphic operations
     */
    Ciphertext bootstrap(const Ciphertext& ct, const EvaluationKey& evk);
    
    /**
     * Rotate/shift encrypted vector
     */
    Ciphertext rotate(const Ciphertext& ct, int steps, const EvaluationKey& evk);
    
    // ========== Utility Functions ==========
    
    /**
     * Get protocol information and security parameters
     */
    std::string get_info() const;
    
    /**
     * Serialize ciphertext to binary blob
     */
    std::vector<uint8_t> serialize(const Ciphertext& ct);
    
    /**
     * Deserialize ciphertext from binary blob
     */
    Ciphertext deserialize(const std::vector<uint8_t>& data);
    
    /**
     * Get security analysis report
     */
    std::string security_report() const;
    
private:
    SecurityParams params_;
    
    // Internal helper functions
    std::vector<uint64_t> sample_gaussian_noise(size_t n);
    std::vector<int64_t> sample_ternary(size_t n);
    std::vector<uint64_t> sample_uniform(size_t n, uint64_t modulus);
    
    void mod_reduce(std::vector<int64_t>& poly, uint64_t modulus);
    void center_lift(std::vector<int64_t>& poly, uint64_t modulus);
    
    // Polynomial arithmetic in R_q = Z_q[X]/(X^n + 1)
    std::vector<int64_t> poly_add(const std::vector<int64_t>& a,
                                   const std::vector<int64_t>& b);
    std::vector<int64_t> poly_sub(const std::vector<int64_t>& a,
                                   const std::vector<int64_t>& b);
    std::vector<int64_t> poly_mul(const std::vector<int64_t>& a,
                                   const std::vector<int64_t>& b);
};

} // namespace FXBL

#endif // FXBL_CORE_HPP
