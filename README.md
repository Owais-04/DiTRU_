# THIS IS A QUANTUM-SAFE CRYPTOSYSTEM

### This is designed over the dihedral group based on the NTRU Round 3 cryptosystem.
### The first layer is KEM (Key Encapsulation Mechanism), followed by the implementation of the dihedral group.

## KEY FEATURES
1. Quantum-safe design
2. Resistance against Shor's algorithm and other quantum attacks
3. Efficient key generation, encryption, and decryption
4. Secure key exchange using the dihedral group structure
5. Based on lattice-based cryptography principles for added security
6. Constant-time implementation in C to prevent side-channel attacks

## IMPLEMENTATION DETAILS
- Uses the NTRU encryption scheme with modifications for dihedral group security
- Implements a hybrid encryption system combining classical and quantum-resistant methods
- Optimized for performance while maintaining high security
- Developed using C with constant-time operations to mitigate timing attacks

## USAGE
### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/DITRU.git
   cd DITRU
   ```
2. Compile the C implementation using Makefile:
   ```bash
   make
   ```

### Running the Cryptosystem
To generate keys:
```bash
./DITRU generate_keys
```

To encrypt a message:
```bash
./DITRU encrypt "Your message here"
```

To decrypt a message:
```bash
./DITRU decrypt "Encrypted message here"
```

## SECURITY CONSIDERATIONS
- The system is designed to withstand quantum computing threats
- Uses a structured noise model to resist attacks
- Parameters are chosen for optimal security-performance tradeoff
- All cryptographic operations are implemented in **constant time** to prevent timing-based side-channel attacks

## FUTURE IMPROVEMENTS
- Implementing post-quantum signature schemes
- Optimizing efficiency for real-world applications
- Extending support for additional cryptographic primitive

## LICENSE
This project is licensed under the MIT License.
