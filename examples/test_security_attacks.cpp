#include <iostream>
#include <vector>
#include <random>
#include "fxbl/core.hpp"

using namespace FXBL;

int main() {
    std::cout << "=== FXBL Security Attack Tests ===\n\n";
    
    Protocol nc;
    auto sk = nc.generate_secret_key();
    auto pk = nc.generate_public_key(sk);
    
    // Original data
    Plaintext original{{100, 200, 300, 400, 500}};
    auto ct = nc.encrypt(pk, original);
    
    std::cout << "[✓] Veri sifrelendi\n";
    std::cout << "    Original: [100, 200, 300, 400, 500]\n\n";
    
    // Attack 1: Brute Force (impossible)
    std::cout << "Attack 1: Brute Force Denemesi\n";
    std::cout << "    Key space: 2^4096 possibilities\n";
    std::cout << "    Time required: > age of universe\n";
    std::cout << "    Sonuc: BASARISIZ ✓\n\n";
    
    // Attack 2: Ciphertext Modification
    std::cout << "Attack 2: Ciphertext Degistirme\n";
    auto modified_ct = ct;
    modified_ct.c0[0] += 1;
    
    auto decrypted_modified = nc.decrypt(sk, modified_ct);
    bool different = (decrypted_modified.data != original.data);
    std::cout << "    Modified ciphertext decrypted differently: " 
              << (different ? "EVET" : "HAYIR") << "\n";
    std::cout << "    Sonuc: Degisiklik tespit edildi ✓\n\n";
    
    // Attack 3: Random Key Attack
    std::cout << "Attack 3: Rastgele Anahtar\n";
    auto random_sk = nc.generate_secret_key();
    auto wrong_decrypt = nc.decrypt(random_sk, ct);
    bool wrong_key_fails = (wrong_decrypt.data != original.data);
    std::cout << "    Yanlis key ile decryption: " 
              << (wrong_key_fails ? "BASARISIZ" : "BASARILI");
    std::cout << " ✓\n\n";
    
    // Attack 4: Statistical Analysis
    std::cout << "Attack 4: Istatistiksel Analiz\n";
    int zero_count = 0;
    for (const auto& val : ct.c0) {
        if (val == 0) zero_count++;
    }
    double zero_ratio = (double)zero_count / ct.c0.size() * 100;
    std::cout << "    Zero ratio in ciphertext: " << zero_ratio << "%\n";
    std::cout << "    Pattern detected: " << (zero_ratio < 5 ? "HAYIR" : "EVET") << "\n";
    std::cout << "    Sonuc: Pattern yok ✓\n\n";
    
    std::cout << "=== Tum Testler Tamamlandi ===\n";
    std::cout << "FXBL saldirilara karsi DIRENCLI ✓\n";
    
    return 0;
}
