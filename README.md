# NCDC C Module Project — Huffman Encoder

## Course
**NCDC Cohort 02/2025 — Design Verification (DV)**
NUST Chip Design Centre (NCDC), NUST

## Module
**C/C++ Module** — End-of-Module Project

---

## Overview

This project implements a **lossless data compression system** using **Huffman Coding** in C. Huffman coding assigns shorter binary codes to more frequently occurring characters in a file, achieving compression by exploiting the statistical distribution of symbols. The project includes both an encoder and a decoder, validated against a real-world text corpus.

---

## Algorithm

### Encoding Pipeline
1. **Frequency Analysis** — Scan the input file and count the occurrence of every byte value.
2. **Priority Queue (Min-Heap)** — Insert all symbols as leaf nodes into a min-heap ordered by frequency.
3. **Tree Construction** — Repeatedly extract the two nodes with lowest frequency, merge them into an internal node, and re-insert. Repeat until a single root remains.
4. **Code Generation** — Traverse the Huffman tree: assigning `0` for left branches and `1` for right branches produces a unique prefix-free binary code for each symbol.
5. **Bitstream Output** — Replace each symbol in the input with its Huffman code and write the packed bitstream to disk, along with a header encoding the tree structure for decompression.

### Decoding Pipeline
1. Reconstruct the Huffman tree from the stored header.
2. Read the compressed bitstream bit by bit, traversing the tree at each step.
3. Emit a symbol whenever a leaf node is reached, then restart from the root.

---

## Repository Structure

```
Lab 8/
├── Lab 8 files (huffman)/
│   ├── encode.c          # Encoder — builds Huffman tree and writes compressed output
│   ├── decode.c          # Decoder — reconstructs original file from compressed stream
│   ├── encode             # Compiled encoder binary
│   ├── decode             # Compiled decoder binary
│   ├── book.txt           # Input corpus (sample book text)
│   ├── book_ascii.txt     # ASCII-filtered version of the corpus
│   ├── encoded.txt        # Huffman-encoded output
│   ├── decoded.txt        # Reconstructed output after decoding
│   ├── code.txt           # Generated Huffman code table
│   ├── task1part1.png     # Screenshot — frequency table / code table output
│   ├── task1part2.png     # Screenshot — compression ratio analysis
│   ├── task2part1.png     # Screenshot — encoding verification
│   └── task3.png          # Screenshot — decoding verification
└── NCDC Lab 8.pdf         # Full lab report with design explanation and results
```

---

## How to Build and Run

```bash
# Compile encoder and decoder
gcc -o encode encode.c -Wall
gcc -o decode decode.c -Wall

# Encode a file
./encode book.txt encoded.txt code.txt

# Decode the compressed file
./decode encoded.txt decoded.txt code.txt

# Verify lossless reconstruction
diff book.txt decoded.txt   # Should produce no output if identical
```

---

## Key Concepts Demonstrated
- Binary tree construction and heap-based priority queue in C
- Prefix-free variable-length binary encoding
- Bitwise I/O operations in C (`fread`, `fwrite`, bit manipulation)
- File compression and decompression pipeline
- Compression ratio analysis on real text data
