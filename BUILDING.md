# Build NeuroCrypt-X

## Quick Start

```bash
# Clone the repository
git clone https://github.com/m3sto/neurocrypt-x.git
cd neurocrypt-x

# Run build script
chmod +x build.sh
./build.sh

# Or manual build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
ctest
```

## Requirements

### Compiler
- GCC 10+ or Clang 12+
- C++20 support required

### Dependencies
- CMake 3.16+
- pthread (included in most systems)

### Optional (for performance)
- AVX2 support (enable with `-DENABLE_AVX2=ON`)
- AVX-512 support (enable with `-DENABLE_AVX512=ON`)

## Build Configuration

### Standard Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

### With SIMD Optimizations
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DENABLE_AVX2=ON \
         -DENABLE_AVX512=ON
```

### Custom Security Parameters
Edit `include/neurocrypt/core.hpp`:
```cpp
struct SecurityParams {
    size_t ring_dimension = 4096;      // Increase for more security
    uint64_t modulus = 1ULL << 60;     // Coefficient modulus
    double noise_std_dev = 3.2;        // Gaussian noise
};
```

## Running Examples

After building, executables are in `build/`:

```bash
# Main demonstration
./build/neurocrypt-bin

# Basic usage example
./build/example_basic

# Homomorphic operations
./build/example_homomorphic

# Cloud computing scenario
./build/example_cloud
```

## Running Tests

```bash
# Run all tests
ctest --output-on-failure

# Run specific test suite
./build/test_neurocrypt

# With verbose output
ctest -V
```

## Running Benchmarks

```bash
# Full benchmark suite
./build/bench_neurocrypt

# Specific benchmarks
./build/bench_neurocrypt --filter=keygen
./build/bench_neurocrypt --filter=encrypt
./build/bench_neurocrypt --filter=homomorphic
```

## Installation

```bash
# Install to system
sudo make install

# Install to custom prefix
cmake .. -DCMAKE_INSTALL_PREFIX=/opt/neurocrypt
make install
```

## Troubleshooting

### Compilation Errors

**Error: C++20 features not available**
```
Solution: Upgrade to GCC 10+ or Clang 12+
```

**Error: CMake not found**
```bash
sudo apt-get install cmake      # Debian/Ubuntu
sudo dnf install cmake          # Fedora
brew install cmake              # macOS
```

### Performance Issues

**Slow encryption/decryption:**
- Enable AVX2/AVX-512 optimizations
- Use Release build type
- Check CPU supports required instruction sets

**High memory usage:**
- Reduce ring dimension in SecurityParams
- Use smaller polynomial degree

### Test Failures

If tests fail:
1. Ensure you're using correct compiler version
2. Check system has sufficient entropy (/dev/urandom)
3. Verify CMake configuration completed successfully

## Platform-Specific Notes

### Linux
Tested on Ubuntu 20.04+, Fedora 34+, Debian 11+

### macOS
```bash
brew install cmake
export CXX=clang++
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### Windows (WSL)
Use WSL2 with Ubuntu for best results.

## Development

### Code Style
```bash
# Format code
clang-format -i src/*.cpp include/*.hpp

# Run linter
clang-tidy src/*.cpp
```

### Adding New Features
1. Create feature branch
2. Implement in src/
3. Add tests in tests/
4. Update documentation
5. Submit pull request

## Performance Tuning

For maximum performance:

1. **Enable SIMD:**
   ```bash
   cmake .. -DENABLE_AVX2=ON -DENABLE_AVX512=ON
   ```

2. **Use native architecture:**
   ```bash
   CXXFLAGS="-march=native" cmake ..
   ```

3. **Link-time optimization:**
   ```bash
   cmake .. -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
   ```

4. **Parallel build:**
   ```bash
   make -j$(nproc)
   ```

## Documentation

- API Reference: `docs/api.md`
- Security Proofs: `docs/security_proofs.pdf`
- Implementation Guide: `docs/implementation.md`

## Support

- GitHub Issues: https://github.com/m3sto/neurocrypt-x/issues
- Discussions: https://github.com/m3sto/neurocrypt-x/discussions

## License

MIT License - See LICENSE file for details.
