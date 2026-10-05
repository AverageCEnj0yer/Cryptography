# Building the laboratory report

The report is written in `main.tex`. This Markdown file contains build instructions; it is not the PDF source.

From the Cryptography directory, run:

```bash
cd Lab_1/report
tectonic main.tex
```

The resulting PDF is `Lab_1/report/main.pdf` (or simply `main.pdf` from the report directory). Tectonic handles the passes needed for the contents, citations, and cross-references. Its first run may download TeX support files and requires a writable cache.

To retain intermediate files and a log for troubleshooting:

```bash
tectonic -k --keep-logs main.tex
```

Run these commands **from the report directory**: the document uses relative paths for the logo, screenshots, metadata, class, and C++ source excerpts. The configured fonts are Liberation Serif and Liberation Mono, with Times New Roman used for body text when installed.

## Files

- `main.tex`: report text, equations, source excerpts, screenshots, and references.
- `metadata.tex`: Gabriel Balan, FAF-241, supervisor, and repository URL inherited from your metadata. Verify these personal details before submission.
- `utmreport.cls`: the supplied UTM formatting template.
- `screenshots/`: your three original terminal screenshots, referenced by their existing filenames.
- `assets/UTM_Logo_English.jpeg`: the existing title-page logo.
- `../src/main.cpp`: the unchanged application source. LaTeX reads selected line ranges directly, so keep this file alongside the report when rebuilding.

No Java, Maven, or JUnit tooling is needed. No application source changes are required to generate the PDF.

## Reproducing the program runs

From `Lab_1`:

```bash
cmake -S . -B build
cmake --build build
./build/lab1 1
./build/lab1 2
```

Mode 1 uses the standard alphabet; mode 2 requests a keyword. Both console modes encrypt. The source functions implement decryption through their final Boolean argument, but the existing console does not provide an operation selector.

The report accurately distinguishes the supplied screenshots from supplementary checks performed while preparing it. A temporary harness verified both decryption functions and 60 full-alphabet round trips, without changing `main.cpp`. It was not installed as a project test suite. Additional process checks covered malformed encodings and invalid inputs.

The handout asks for screenshots of decryption and an invalid short keyword; your three screenshots do not include those cases. The report includes function-level decryption results and a reproducible short-keyword command, and explicitly describes this evidence gap. Invalid keywords currently exit instead of retrying. These facts are documented without altering your final C++ implementation.
