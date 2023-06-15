= Niklas' Algorithm Competition Template Library (NACTL)

#let include_snippet(title, filename) = {
  heading(level: 3, title)
  raw(
    read(filename).trim(regex("(?sm).*// nactl: start\s*"), at: start),
    block: true,
    lang: "cpp",
  )
}

#include_snippet("Template", "template.cpp")

== Data Structures

#include_snippet("UnionFind", "snippets/union_find.hpp")
#include_snippet("Range", "snippets/range.hpp")
#include_snippet("Segment Tree", "snippets/segment_tree.hpp")

== Graph

== Flows

#include_snippet("Dinitz", "snippets/dinitz.hpp")
#include_snippet("Path Decomposition", "snippets/flow_decomp.hpp")

== Trees

== Strings

#include_snippet("Suffix Array", "snippets/suffix_array.hpp")

== Math

#include_snippet("Chinese Remainder Theorem", "snippets/crt.hpp")
#include_snippet("Primality and Factoring", "snippets/factorize.hpp")

Note: FFT repeats after four applications!
#include_snippet("Fast Fourier Transform", "snippets/fft.hpp")
#include_snippet("Number Theoretic Transform", "snippets/ntt.hpp")
