# HORS Digital Signature Scheme

> **Course Project | Trustworthy Infrastructure | University of South Florida**
> Implementation of the Hash to Obtain Random Subset (HORS) one-time signature scheme in C

---

## Overview

**HORS** is a cryptographic one-time signature scheme built on hash functions. It is extremely fast to verify and provides strong security guarantees based on the one-wayness of SHA-256. This project implements the full HORS pipeline:

1. **Key Generation** — derive a Secret Key (SK) and Public Key (PK) from a seed
2. **Signing** — sign a message using the SK
3. **Verification** — verify a signature using the PK

All cryptographic primitives (ChaCha20 PRNG and SHA-256) are provided by **LibTomCrypt**.

---

## How HORS Works

### Parameters
| Parameter | Description |
|-----------|-------------|
| `t` | Number of secret key elements (e.g. 128 or 1024) |
| `k` | Number of elements revealed per signature |
| `l = log2(t)` | Bits used per index from the message hash |

### Key Generation
- For each `i` from `0` to `t-1`, generate `SK[i]` using ChaCha20 PRNG seeded with `seed || counter`
- Compute `PK[i] = SHA-256(SK[i])` for each element
- Output `SK.txt` and `PK.txt`

### Signing
- Hash the message with SHA-256
- Split the hash bits into `k` groups of `l` bits each → get `k` indexes
- Output `Signature.txt` containing `SK[index_0], SK[index_1], ..., SK[index_k-1]`

### Verification
- Hash the message with SHA-256 → extract the same `k` indexes
- For each `i`, compute `SHA-256(Signature[i])` and compare to `PK[index_i]`
- If all match → valid; write indexes to `Verification.txt`
- If any mismatch → write `"Verification Failed"`

---

## File Structure

```
├── RequiredFunctionsHORS.c     # Shared utility library
├── KeyGen-1.c                  # Key generation
├── Sign-1.c                    # Message signing
├── Verify-1.c                  # Signature verification
└── TestVectors/                # Official test vectors (t=128, t=1024)
    ├── Seed1.txt / Seed2.txt
    ├── Message1.txt / Message2.txt
    ├── CorrectSK1t128.txt ...
    ├── CorrectPK1t128.txt ...
    ├── CorrectSignature1t128.txt ...
    └── CorrectVerification1t128.txt ...
```

---

## Dependencies

- **LibTomCrypt** — for ChaCha20 PRNG and SHA-256
- GCC with C99 or later

Install LibTomCrypt on Ubuntu:
```bash
sudo apt-get install libtomcrypt-dev
```

---

## Compilation & Usage

### Compile
```bash
gcc KeyGen-1.c -o KeyGen -ltomcrypt -lm
gcc Sign-1.c -o Sign -ltomcrypt -lm
gcc Verify-1.c -o Verify -ltomcrypt -lm
```

### Key Generation
```bash
./KeyGen <Seed.txt> <t>
# Example:
./KeyGen Seed1.txt 128
# Outputs: SK.txt, PK.txt
```

### Sign
```bash
./Sign <Message.txt> <SK.txt> <t> <k>
# Example:
./Sign Message1.txt SK.txt 128 8
# Outputs: Signature.txt
```

### Verify
```bash
./Verify <Message.txt> <Signature.txt> <PK.txt> <message-length> <t> <k>
# Example:
./Verify Message1.txt Signature.txt PK.txt 50 128 8
# Outputs: Verification.txt
```

---

## Test Vectors

Two test cases are provided for both `t=128` and `t=1024`:

| Test | Seed | Message | t |
|------|------|---------|---|
| Test 1 | Seed1.txt | Message1.txt | 128 / 1024 |
| Test 2 | Seed2.txt | Message2.txt | 128 / 1024 |

Run the provided shell script to validate your output against all test vectors:
```bash
bash VerifyingYourHORSSolution.sh
```

---

## Key Concepts Demonstrated

- **HORS one-time signature scheme** — hash-based post-quantum cryptography
- **ChaCha20 PRNG** — cryptographically secure pseudorandom number generation
- **SHA-256 hashing** — one-way function used for both key derivation and verification
- **LibTomCrypt** — industry-standard C cryptography library
- **Bitwise operations** — binary hash parsing for index extraction
- **File I/O in C** — reading seeds/messages, writing keys/signatures/verification output
