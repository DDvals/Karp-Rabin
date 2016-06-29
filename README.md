# Karp-Rabin for FASTA files

[Karp-Rabin](http://www.engr.uconn.edu/~ywu/Courses/CSE5840F15/CSE5840F15-Lecture4.pdf)
pattern-matching algorithm implementation for FASTA files.

Input: FASTA file (fasta or gz), pattern to match and precision.
Output: Number of occurency found and occurency positions

To compile just run `make` in the k-r folder.

Usage:

```
./karp-rabin <fasta_file> <pattern> <precision>
```

If `<precision = -1` Error check on every occurency we find.
