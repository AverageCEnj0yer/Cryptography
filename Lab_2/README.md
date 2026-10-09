# Lab 2 — Cryptanalysis of Monoalphabetic Ciphers

An English monoalphabetic-substitution cryptogram solved manually through letter frequencies, word patterns, and contextual clues using [Crypto Corner](https://crypto.interactive-maths.com/frequency-analysis-breaking-the-code.html). This laboratory has no application code.

## Generate the report

From the repository root:

```bash
cd Lab_2/report
tectonic main.tex
```

The output is `Lab_2/report/main.pdf`. Tectonic handles cross-references and the table of contents automatically. It may download missing TeX support files on its first build. The document uses the Liberation Serif and Liberation Mono fonts.

From inside `Lab_2`, use `cd report` followed by the same Tectonic command. No CMake or Python command is needed.

## Report files

- `report/main.tex`: document setup and cover page.
- `report/metadata.tex`: Balan Gabriel, FAF-241, Maia Zaica, and the report topic.
- `report/sections/`: English theory, analysis, all 19 deductions, progressive extracts, full readable plaintext, and conclusions.
- `report/screenshots/`: the four supplied frequency images, unchanged.
- `report/assets/`: the university logo.
- `report/data/`: supplied ciphertext, literal plaintext, intermediate extracts, substitution key, and measured counts.

Ciphertext `M` does not occur; its mapping to `z` is explicitly marked as inferred by elimination. The readable plaintext restores missing spaces and capitalization; the literal version preserves the original spacing and punctuation.

For troubleshooting, retain Tectonic's intermediate files and log:

```bash
tectonic -k --keep-logs main.tex
```
