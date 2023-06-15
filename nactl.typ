#set page(flipped: true, margin: 1cm)
#set text(8.5pt)

#align(center, underline(text(17pt, weight: 700, "Niklas' Algorithm Competition Template Library (NACTL)")))

#show: doc => columns(3, doc)

#set heading(numbering: "1.a.")
#outline(indent: true)
#colbreak()

#let include_snippet(title, filename) = {
  heading(level: 2, title)
  raw(
    read(filename).trim(regex("(?sm).*// nactl: start\s*"), at: start),
    block: true,
    lang: "cpp",
  )
  line(length: 100%)
}

#include_snippet("Template", "template.cpp")

= Data Structures

#include_snippet("Union Find", "snippets/union_find.hpp")
#include_snippet("Range", "snippets/range.hpp")
#colbreak()
#include_snippet("Sparse Table", "snippets/sparse_table.hpp")
#include_snippet("Segment Tree", "snippets/segment_tree.hpp")
#include_snippet("Treap", "snippets/treap.hpp")

= Graph

#include_snippet("Topological Order", "snippets/topo.hpp")
#include_snippet("Bridges", "snippets/bridges.hpp")
#include_snippet("Cut-Vertices", "snippets/cut_vertices.hpp")
#include_snippet("SCC", "snippets/scc.hpp")

= Flows

#include_snippet("Dinitz", "snippets/dinitz.hpp")
#include_snippet("Path Decomposition", "snippets/flow_decomp.hpp")
#include_snippet("Min Cost Flow", "snippets/min_cost_flow.hpp")

= Trees

#include_snippet("Binary lifting", "snippets/binary_lifts.hpp")
#include_snippet("Heavy Light Decomposition", "snippets/heavy_light.hpp")
#include_snippet("Centroids", "snippets/centroid.hpp")
Note: Slides mention centroid tree for radius queries.

= Strings

#include_snippet("P and Z functions", "snippets/string_p_z.hpp")
#include_snippet("Rolling hashes", "snippets/string_hashing.hpp")
#include_snippet("Suffix Array", "snippets/suffix_array.hpp")
#include_snippet("Aho-Corasick", "snippets/aho_corasick.hpp")

= Math

#include_snippet("Chinese Remainder Theorem", "snippets/crt.hpp")
#include_snippet("Primality and Factoring", "snippets/factorize.hpp")

#include_snippet("Fast Fourier Transform", "snippets/fft.hpp")
#include_snippet("Number Theoretic Transform", "snippets/ntt.hpp")
