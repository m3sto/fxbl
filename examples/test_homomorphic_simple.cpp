#include <iostream>
#include <vector>
#include "fxbl/core.hpp"

using namespace FXBL;

int main() {
    std::cout << "=== FXBL Homomorphic Test ===\n\n";
    
    Protocol nc;
    auto sk = nc.generate_secret_key();
    auto pk = nc.generate_public_key(sk);
    
    // Test 1: Encrypted Addition
    std::cout << "Test 1: Sifreli Toplama\n";
    Plaintext a{{10, 20, 30}};
    Plaintext b{{5, 15, 25}};
    
    auto enc_a = nc.encrypt(pk, a);
    auto enc_b = nc.encrypt(pk, b);
    
    std::cout << "    Encrypt: [10, 20, 30] + [5, 15, 25]\n";
    
    auto enc_sum = nc.add(enc_a, enc_b);
    auto sum = nc.decrypt(sk, enc_sum);
    
    std::cout << "    Decrypt: [";
    for (size_t i = 0; i < std::min(sum.data.size(), size_t(3)); ++i) {
        std::cout << sum.data[i];
        if (i < 2) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "    Expected: [15, 35, 55]\n";
    std::cout << "    Result: " << (sum.data[0] == 15 ? "BASARILI" : "BASARISIZ") << "\n\n";
    
    // Test 2: Multiple Operations
    std::cout << "Test 2: Coklu Islemler\n";
    Plaintext x{{100}};
    auto enc_x = nc.encrypt(pk, x);
    
    // Add 3 times
    auto enc_3x = nc.add(nc.add(enc_x, enc_x), enc_x);
    auto result_3x = nc.decrypt(sk, enc_3x);
    
    std::cout << "    3 * 100 = " << result_3x.data[0] << "\n";
    std::cout << "    Result: " << (result_3x.data[0] == 300 ? "BASARILI" : "BASARISIZ") << "\n\n";
    
    std::cout << "=== Homomorphic Tests Complete ===\n";
    
    return 0;
}
