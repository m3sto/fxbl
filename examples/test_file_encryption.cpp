#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "fxbl/core.hpp"

using namespace FXBL;

int main() {
    std::cout << "=== FXBL File Encryption Test ===\n\n";
    
    // Create test file
    std::ofstream test("test_data.txt");
    test << "Gizli mesaj: Bu cok onemli bir bilgi!\n";
    test << "Sifre: TopSecret123\n";
    test << "Kredi karti: 1234-5678-9012-3456\n";
    test.close();
    std::cout << "[✓] Test dosyasi olusturuldu: test_data.txt\n";
    
    // Read file
    std::ifstream input("test_data.txt", std::ios::binary);
    std::vector<char> buffer((std::istreambuf_iterator<char>(input)),
                              std::istreambuf_iterator<char>());
    input.close();
    
    std::cout << "[✓] Dosya okundu: " << buffer.size() << " bytes\n\n";
    
    // Encrypt
    Protocol nc;
    auto sk = nc.generate_secret_key();
    auto pk = nc.generate_public_key(sk);
    
    Plaintext pt{std::vector<int64_t>(buffer.begin(), buffer.end())};
    auto ct = nc.encrypt(pk, pt);
    
    std::cout << "[✓] Veri sifrelendi\n";
    std::cout << "    Ciphertext size: " << ct.c0.size() << " coefficients\n\n";
    
    // Decrypt
    auto decrypted = nc.decrypt(sk, ct);
    
    std::cout << "[✓] Sifre cozuldu\n";
    
    // Verify
    bool match = (pt.data == decrypted.data);
    std::cout << "[✓] Dogrulama: " << (match ? "BASARILI" : "BASARISIZ") << "\n\n";
    
    if (match) {
        std::cout << "Original: \"Gizli mesaj: Bu cok onemli...\"\n";
        
        // Find first null or non-printable
        size_t end = 0;
        for (size_t i = 0; i < decrypted.data.size() && i < 100; ++i) {
            if (decrypted.data[i] < 32 || decrypted.data[i] > 126) {
                end = i;
                break;
            }
        }
        
        std::string recovered;
        for (size_t i = 0; i < end && i < 30; ++i) {
            recovered += (char)decrypted.data[i];
        }
        
        std::cout << "Recovered: \"" << recovered << "...\"\n";
    }
    
    return match ? 0 : 1;
}
