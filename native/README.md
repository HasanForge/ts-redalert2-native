# ra2cpp native deterministic core

This repository contains the native C++/MinGW port for the Red Alert 2 reference implementation.

Current native milestone:
- DataStream
- CRC32
- FNV32A
- PRNG
- ordered helpers
- INI parsing
- MixEntry hashing
- deterministic headless smoke harness
- pass/fail unit tests

Directory layout:
- native/include/: reusable C++ headers + implementations
- native/src/: native executable entry points
- native/tests/: pass/fail tests

Build (Windows/MinGW 32-bit target):
  make -C native

Run:
  ./native/ra2cpp.exe --headless
