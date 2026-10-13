# Knowledge Graph in C

A command-line graph query engine built with C11, adjacency lists, a binary search tree, a FIFO queue, and a binary min-heap. Load entities and directed, weighted relationships, then answer existence, neighborhood, reachability, and shortest-path queries.

Originally developed as a university project for the 2025–2026 academic year. This portfolio edition retains the educational origin, public test fixtures, and Romanian result format.

## What it does

- Models people, companies, locations, and events as graph vertices.
- Preserves relationship insertion order in adjacency lists.
- Indexes names with an unbalanced binary search tree (BST).
- Processes a batch of queries in FIFO order.
- Finds a fewest-edge path with breadth-first search (BFS).
- Finds a minimum-cost path with Dijkstra's algorithm for non-negative weights.

## Build and run

Use GCC or a compatible C11 compiler, GNU Make, a POSIX shell, and GNU diff. The code also uses POSIX `strdup` and GNU-compatible attributes. On Windows, use an MSYS2 UCRT64 terminal with these tools available; plain PowerShell and MSVC are not the documented build environment.

From the repository root:

```sh
make
./build/knowledge_graph tests/public/pas4/entitati.csv tests/public/pas4/relatii.csv tests/public/pas4/interogari.txt
```

The included example produces:

```text
EXISTS Ana: DA
EXISTS Bob: NU
EDGE Ana Firma: DA
NEIGHBORS Ana: Firma Mara
PATH Ana Iasi: Ana -> Firma -> Iasi
DIJKSTRA Ana Iasi: COST = 1.80; DRUM = Ana -> Firma -> Iasi
```

`DA` means yes, `NU` means no, and `DRUM` means path. Append `--debug` to print graph adjacency lists and queued queries to standard error. The debug BST section currently contains only a placeholder message.

```sh
make test          # public fixtures and graph-growth regression
make public-test   # original four public stages only
make clean         # remove generated build files
```

The former `make build/student_kg` target remains available for compatibility. Compiler options can be overridden, for example with `make CC=gcc`.

## Input files

Both data files must begin with a header: the loader always skips their first line. Records are separated by newlines and fields by semicolons; this is a simple delimiter parser, with no CSV quoting or escaping.

**Entities** (`NUME;TIP`):

```text
NUME;TIP
Ana;PERSON
Firma;COMPANY
Iasi;LOCATION
```

Supported types: `PERSON`, `COMPANY`, `LOCATION`, `EVENT`.

**Relationships** (`SURSA;RELATIE;DESTINATIE;COST`):

```text
SURSA;RELATIE;DESTINATIE;COST
Ana;works_at;Firma;1.20
Firma;located_in;Iasi;0.60
```

Supported relations: `works_at`, `friend_of`, `located_in`, `participates_in`. All edges are directed, including `friend_of`; add a reverse edge explicitly when required. Supply finite, non-negative costs with a decimal point.

**Queries** use uppercase commands, case-sensitive names, and single spaces between tokens:

| Command | Result |
| --- | --- |
| `EXISTS name` | Whether the entity exists |
| `EDGE source destination` | Whether a direct edge exists, regardless of relation type |
| `NEIGHBORS name` | Outgoing neighbors in insertion order, or `NULL` |
| `PATH source destination` | A path with the fewest edges, or `NU` |
| `DIJKSTRA source destination` | A minimum-cost path and its cost, or `NU` |

Use unique, non-empty entity names without whitespace or semicolons. Keep each physical input line below 512 bytes, including its newline. Unknown/malformed records may be skipped; invalid input is not comprehensively rejected.

## Implementation

```text
include/          Module interfaces and ownership comments
src/              Entity, graph, BST, queue, heap, query, and CLI code
tests/public/     Original four stage harnesses and fixtures
tests/regression/ CLI graph-growth regression
Makefile          Application and test builds
```

The loader finishes adding graph vertices before building the BST, so graph-array growth cannot invalidate the pointers stored in that index. Relationships are then loaded, queries are enqueued, and results are written to standard output. The graph owns vertices and edges; the BST holds borrowed vertex pointers.

Let `V` be the number of vertices, `E` the number of edges, `h` the BST height, and `d(v)` a vertex's out-degree. Name/string comparison costs are omitted below.

| Operation | Time |
| --- | --- |
| Name lookup | `O(h)`, up to `O(V)` for the unbalanced BST |
| Edge lookup / neighbors | `O(h + d(v))` |
| BFS path | `O(V + E)` |
| Dijkstra | `O(V log V + E(V + log V))` worst-case upper bound |
| Load graph and index | `O(Vh + Eh + sum(d(v)^2))` upper bound |

The heap's decrease-key operation scans for a vertex before sifting it up; it is **not** an indexed `O(log V)` decrease-key implementation. Likewise, adding an edge scans to the tail of its adjacency list. These details matter when scaling beyond small educational datasets.

## Scope and limitations

This is an educational, in-memory batch program, not a production graph database. It has no persistence service or interactive query language. The parser uses fixed-size line buffers, `strtok`, and `atof`; it does not enforce non-negative costs, strict numeric syntax, unique names, or exact command keywords. Several allocation-failure paths remain unchecked. Use trusted, well-formed inputs.

The BST is recursive and unbalanced, so sorted inputs can produce deep recursion. If reusing the modules directly, finish adding vertices before creating a BST index; subsequent graph growth can invalidate borrowed vertex pointers. The public tests exercise small examples, not exhaustive correctness or memory safety.
