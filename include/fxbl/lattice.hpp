**
 * @version 1.0
 * @author m3sto
 */
/**
 * @version 1.0
 * @author m3sto
 * FXBL Lattice-Based Cryptography Core
 * @version 1.0
 * @author m3sto
 * 
 * @version 1.0
 * @author m3sto
 * Ring-LWE (Learning With Errors) implementation
 * @version 1.0
 * @author m3sto
 * The mathematical foundation providing post-quantum security
 * @version 1.0
 * @author m3sto
 */

#ifndef FXBL_LATTICE_HPP
#define FXBL_LATTICE_HPP

#include <vector>
#include <cstdint>
#include <cmath>
#include <random>

namespace FXBL {
namespace Lattice {

/**
 * Polynomial ring R_q = Z_q[X]/(X^n + 1)
 * All operations are performed in this ring structure
 */
class PolynomialRing {
public:
    explicit PolynomialRing(size_t degree, uint64_t modulus);
    
    // Addition in R_q
    std::vector<int64_t> add(const std::vector<int64_t>& a,
                            const std::vector<int64_t>& b) const;
    
    // Subtraction in R_q
    std::vector<int64_t> sub(const std::vector<int64_t>& a,
                            const std::vector<int64_t>& b) const;
    
    // Multiplication in R_q using NTT (Number Theoretic Transform)
    std::vector<int64_t> mul(const std::vector<int64_t>& a,
                            const std::vector<int64_t>& b) const;
    
    // Negation
    std::vector<int64_t> negate(const std::vector<int64_t>& poly) const;
    
    // Scalar multiplication
    std::vector<int64_t> scalar_mul(const std::vector<int64_t>& poly,
                                    int64_t scalar) const;
    
    // Reduce coefficients to [0, q)
    void mod_reduce(std::vector<int64_t>& poly) const;
    
    // Center-lift to (-q/2, q/2]
    void center_lift(std::vector<int64_t>& poly) const;
    
private:
    size_t n_;      // Ring dimension
    uint64_t q_;    // Modulus
    
    // NTT precomputed tables for fast multiplication
    std::vector<int64_t> ntt_table_;
    std::vector<int64_t> inv_ntt_table_;
    std::vector<int64_t> bit_reverse_table_;
    
    void precompute_ntt_tables();
    void ntt_forward(std::vector<int64_t>& poly) const;
    void ntt_inverse(std::vector<int64_t>& poly) const;
};

/**
 * Gaussian noise sampling for LWE errors
 * Uses discrete Gaussian distribution D_{Z,σ}
 */
class GaussianSampler {
public:
    explicit GaussianSampler(double sigma, size_t dimension);
    
    /**
     * Sample from discrete Gaussian distribution
     * Returns vector of small integers centered at 0
     */
    std::vector<int64_t> sample() const;
    
    /**
     * Sample single coefficient
     */
    int64_t sample_coefficient() const;
    
    /**
     * Compute probability mass function
     */
    double pmf(int64_t x) const;
    
private:
    double sigma_;           // Standard deviation
    size_t dimension_;       // Number of coefficients
    mutable std::mt19937_64 rng_;
    
    // Precomputed constants
    double exp_factor_;
    int64_t tail_bound_;
};

/**
 * Ternary distribution sampler {-1, 0, 1}
 * Used for secret key generation
 */
class TernarySampler {
public:
    explicit TernarySampler(size_t dimension, double prob_zero = 0.5);
    
    std::vector<int64_t> sample() const;
    
private:
    size_t dimension_;
    double prob_zero_;
    mutable std::mt19937_64 rng_;
};

/**
 * Uniform random sampler U([0, q))
 */
class UniformSampler {
public:
    explicit UniformSampler(size_t dimension, uint64_t modulus);
    
    std::vector<uint64_t> sample() const;
    uint64_t sample_single() const;
    
private:
    size_t dimension_;
    uint64_t modulus_;
    mutable std::mt19937_64 rng_;
};

/**
 * Ring-LWE Problem Instance Generator
 * 
 * Generates (a, b = a·s + e) where:
 * - a ← U(R_q) is uniform random
 * - s ← D_ternary is secret key
 * - e ← D_gaussian is error term
 */
class RingLWE {
public:
    RingLWE(size_t ring_dim, uint64_t modulus, double noise_std);
    
    /**
     * Generate Ring-LWE instance
     * @param secret_key Optional: use provided secret key
     * @return pair (a, b) forming the LWE instance
     */
    std::pair<std::vector<uint64_t>, std::vector<uint64_t>> 
    generate_instance(const std::vector<int64_t>& secret_key = {}) const;
    
    /**
     * Get the generated secret key
     */
    const std::vector<int64_t>& get_secret_key() const { return secret_key_; }
    
    /**
     * Security analysis
     * Estimates hardness against known attacks
     */
    struct SecurityEstimate {
        double bkz_cost;          // Cost of BKZ lattice reduction
        double primal_cost;       // Primal attack cost
        double dual_cost;         // Dual attack cost
        int classical_bits;       // Classical security bits
        int quantum_bits;         // Quantum security bits
    };
    
    SecurityEstimate estimate_security() const;
    
private:
    size_t ring_dim_;
    uint64_t modulus_;
    double noise_std_;
    
    PolynomialRing ring_;
    GaussianSampler gaussian_;
    TernarySampler ternary_;
    UniformSampler uniform_;
    
    std::vector<int64_t> secret_key_;
};

/**
 * Key Encapsulation Mechanism (KEM) based on Ring-LWE
 * Provides IND-CCA2 secure key exchange
 */
class KEM {
public:
    explicit KEM(size_t ring_dim = 4096, uint64_t modulus = (1ULL << 60),
                 double noise_std = 3.2);
    
    // Key generation
    struct KeyPair {
        std::vector<uint8_t> public_key;
        std::vector<uint8_t> secret_key;
    };
    
    KeyPair generate_keypair() const;
    
    // Encapsulation: encrypt a random key
    struct Encapsulation {
        std::vector<uint8_t> ciphertext;
        std::vector<uint8_t> shared_secret;
    };
    
    Encapsulation encapsulate(const std::vector<uint8_t>& pk) const;
    
    // Decapsulation: recover shared secret
    std::vector<uint8_t> decapsulate(
        const std::vector<uint8_t>& ciphertext,
        const std::vector<uint8_t>& secret_key) const;
    
private:
    RingLWE lwe_;
};

} // namespace Lattice
} // namespace FXBL

#endif // FXBL_LATTICE_HPP
