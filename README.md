# Cryptography Laboratories

University laboratory work for the Cryptography course at the Technical University of Moldova. Each laboratory has its own directory containing the implementation, build configuration, and report.

## Lab 1 — Caesar Cipher

A C++ implementation of the Caesar cipher for the 31-letter Romanian alphabet, with two variants:

- **One key:** a numeric shift from 1 to 30.
- **Two keys:** a numeric shift and a keyword that determines the alphabet order.

The implementation includes custom UTF-8 ↔ UTF-32 conversion, Romanian uppercase and diacritic normalization, and input validation. Cipher shifts use positions in the Romanian alphabet rather than character encoding values.

## Build and run

Requirements: **CMake 3.20+**, a **C++23-capable compiler** such as GCC or Clang, and a build tool such as Make or Ninja. The application uses only the C++ standard library.

From the repository root:

```bash
cmake -S Lab_1 -B Lab_1/build
cmake --build Lab_1/build
```

Run the standard Caesar cipher:

```bash
./Lab_1/build/lab1 1
```

Run the keyword-based variant:

```bash
./Lab_1/build/lab1 2
```

Enter a message using Romanian letters and spaces, followed by a shift between **1 and 30**. Mode `2` also asks for a keyword containing at least **7 Romanian letters**, with no spaces. Repeated keyword letters are allowed; only their first occurrence is kept when constructing the alphabet.

Messages are converted to uppercase and spaces are removed before encryption. The console currently exposes encryption; both cipher functions also support decryption through their final `decrypt` argument.

## Repository structure

```text
Lab_1/
├── CMakeLists.txt
├── src/
│   └── main.cpp
└── report/
    ├── main.tex
    ├── metadata.tex
    ├── utmreport.cls
    ├── report.md
    ├── assets/
    └── screenshots/
```

## Generate the report

With [Tectonic](https://tectonic-typesetting.github.io/) installed, run:

```bash
cd Lab_1/report
tectonic main.tex
```

This creates `main.pdf` in the report directory. The first build may download TeX support files. See [the report instructions](Lab_1/report/report.md) for more details.

These implementations are educational exercises. Caesar ciphers are not suitable for protecting confidential data.
