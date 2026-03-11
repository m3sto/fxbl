# FXBL Development Guide

**Version:** 1.0  
**Developed by:** m3sto

---

## Quick Links

- `INTRODUCTION.md` - Project overview and features
- `README.md` - Getting started guide
- `BUILDING.md` - Build instructions
- `TEST_RESULTS.md` - Test documentation

---

## Project Structure

```
FXBL/
├── include/fxbl/       # Header files
│   ├── core.hpp       # Main protocol API
│   ├── lattice.hpp    # Lattice cryptography
│   ├── homomorphic.hpp # FHE operations
│   └── neural.hpp     # Neural key evolution
├── src/
│   ├── core/          # Core implementation
│   └── lattice/       # Lattice math
├── examples/          # Usage examples
├── tests/             # Test suite (TODO)
└── docs/              # Documentation
```

---

## Building

```bash
./build.sh
```

Outputs:
- `build/fxbl-bin` - Main demo
- `build/test_*` - Test executables
- `build/example_*` - Example programs

---

## Testing

Run all tests:

```bash
./build/test_security_attacks
./build/test_homomorphic_simple
./build/test_file_encryption
```

---

## Contributing

See INTRODUCTION.md for contribution guidelines.

---

**Developed by m3sto**  
Version 1.0 | MIT License
