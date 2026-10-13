# Public-release preparation

## Changes

- Replaced the classroom-oriented README with build/run instructions, real input/output examples, architecture, accurate complexity bounds, and known limitations.
- Recorded the project's educational origin in `ATTRIBUTION.md`.
- Corrected the default build: the original `make` required missing private `checker/` sources. The default now builds `build/knowledge_graph`; `build/student_kg` remains available.
- Added header dependencies, a `make test` alias, and a growth regression. Shell scripts are invoked through `sh`, so ZIP extraction does not need to preserve executable permissions.
- Made public output comparison ignore CRLF versus LF line endings using GNU diff.
- Built the BST only after entity loading completes, avoiding stale pointers when the graph reallocates beyond its initial capacity of 16 vertices.
- Added focused ignore rules and Git line-ending metadata.
- Excluded the original four ELF test executables and four generated `*_actual.txt` files. Expected-output fixtures remain included.

## Remaining limitations

The implementation still has unchecked allocation-failure paths, permissive input parsing, recursive unbalanced BST operations, and a linear scan in heap decrease-key. Dijkstra assumes non-negative finite edge costs, which the loader does not validate. These are documented in the README rather than presented as production guarantees.

No Git history was included in the input ZIP. A scan of the distributed text files found no apparent credentials or private keys; this is not an audit of any existing remote repository's history.
