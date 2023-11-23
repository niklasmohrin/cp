#let title = "Niklas' Algorithm Competition Template Library (NACTL)"

#set page(
  flipped: true,
  margin: (top: 1cm, rest: 0.7cm),
  header: [
    #set text(8pt)

    #rect(
      stroke: (bottom: black),
      inset: (x: 3pt),
      outset: (x: 5pt),
      grid(
        columns: (1fr, 1fr, 1fr),
        align(left, {
          box(baseline: 1pt, image("assets/hpi.png", alt: "HPI Logo", height: 7pt))
          text(7pt, font: "Liberation Mono", " Burnoutverbot (HPI)")
          link("https://github.com/niklasmohrin/cp")[
            #box(baseline: 1pt, image("assets/github-mark.png", alt: "GH:", height: 7pt))
            #text(7pt, font: "Liberation Mono", "niklasmohrin/cp")
          ]
        }),
        align(center, text(weight: 700, title)),
        align(right, counter(page).display("1 / 1", both: true)),
      )
    )
  ],
)
#set text(8.5pt)

#show: doc => columns(3, doc)

#set heading(numbering: "1.1")
#outline(indent: true)

#let include_snippet(title, filename, level: 2, lang: "cpp", description: none) = {
  heading(level: level, title)
  description
  raw(
    read(filename).trim(regex("(?sm).*// nactl: start\s*"), at: start).trim(regex("(?sm)\s*// nactl: end.*"), at: end),
    block: true,
    lang: lang,
  )
  line(length: 100%, stroke: 0.1pt)
}

#line(length: 100%, stroke: 0.1pt)

= Setup

#include_snippet("Build", "toolbin/build", lang: "sh")
#include_snippet("Test", "toolbin/t", lang: "sh")
#include_snippet("Template", "template.cpp", level: 1)

= Utilities

#include_snippet("Range", "snippets/range.hpp")
#include_snippet("Integer hash", "snippets/hash.hpp")
#include_snippet("Vector", "snippets/vec.hpp")

= Data Structures

#include_snippet("Union Find", "snippets/union_find.hpp")
#include_snippet("Sparse Table", "snippets/sparse_table.hpp")
#include_snippet("Segment Tree", "snippets/segment_tree.hpp")
#include_snippet("Treap", "snippets/treap.hpp")
#include_snippet("2-SAT", "snippets/two_sat.cpp")

= Graph

#include_snippet("Dijkstra", "snippets/dijkstra.hpp")
#include_snippet("Floyd Warshall", "snippets/floyd_warshall.hpp")
#include_snippet("Kruskal", "snippets/kruskal.hpp")
#include_snippet("Topological Order", "snippets/topo.hpp")
#include_snippet("Bridges", "snippets/bridges.hpp")
#include_snippet("Cut-Vertices", "snippets/cut_vertices.hpp")
#include_snippet("SCC", "snippets/scc.hpp")

= Flows

#include_snippet("Ford Fulkerson", "snippets/ford_fulkerson.hpp")
#include_snippet("Dinitz", "snippets/dinitz.hpp")
#include_snippet("Path Decomposition", "snippets/flow_decomp.hpp")
#include_snippet("Min Cost Flow", "snippets/min_cost_flow.hpp")

= Matchings

== Matching, Vertex Cover and Independent Set

For a bipartite graph over $A union B$, if $(S, T)$ is a minimum cut and we define $A_S union.dot A_T = A$ and $B_S union.dot B_T = B$, then $A_T union B_S$ is a minimum vertex cover. The complement is a maximum Independent set.

== Maximum Weight Bipartite Matching

Use min-cost-flow, but make sure to look at current cost in each iteration, because max-flow might not be max-weight.

#include_snippet("Stable Matching (Gale Shapley)", "snippets/stable_matching.hpp")

= Trees

#include_snippet("Binary lifting", "snippets/binary_lifts.hpp")
#include_snippet("Heavy Light Decomposition", "snippets/heavy_light.hpp")
#include_snippet("Centroids", "snippets/centroid.hpp")
Note: Slides mention centroid tree for radius queries.

= Strings

#include_snippet("P and Z functions", "snippets/string_p_z.hpp", description: [
  Definition:
  - $p(i)$ is the length of the longest proper prefix of $s$ that is also a suffix of $s[..i]$.
  - $z(i)$ is the length of the longest common prefix of $s$ and $s[i..]$.
  #image("assets/p-z-functions.png")
])
#include_snippet("Rolling hashes", "snippets/string_hashing.hpp")
#include_snippet("Suffix Array", "snippets/suffix_array.hpp")
#include_snippet("Aho-Corasick", "snippets/aho_corasick.hpp")

= Math

Primes: $3$, $5$, $10007$, $10009$, $10037$.
Primes for NTT: $M = 998244353$ with primitive root $3$, $M = 880803841$ with primitive root $26$.

#include_snippet("Chinese Remainder Theorem", "snippets/crt.hpp")
#include_snippet("Primality and Factoring", "snippets/factorize.hpp")
#include_snippet("Fast Fourier Transform", "snippets/fft.hpp")
#include_snippet("Number Theoretic Transform", "snippets/ntt.hpp")
