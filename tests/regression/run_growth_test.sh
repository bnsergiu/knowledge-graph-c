#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
CASE_DIR="$ROOT_DIR/build/growth-test"
mkdir -p "$CASE_DIR"

# More than 16 vertices forces several graph-array growth operations.
{
    printf 'NUME;TIP\n'
    i=0
    while [ "$i" -lt 100 ]; do
        printf 'Node%03d;PERSON\n' "$i"
        i=$((i + 1))
    done
} > "$CASE_DIR/entities.csv"
{
    printf 'SURSA;RELATIE;DESTINATIE;COST\n'
    printf 'Node000;friend_of;Node050;1.25\n'
    printf 'Node050;friend_of;Node099;2.50\n'
} > "$CASE_DIR/relations.csv"
{
    printf 'EXISTS Node000\nEXISTS Node099\nEXISTS Missing\n'
    printf 'EDGE Node000 Node050\nNEIGHBORS Node050\n'
    printf 'PATH Node000 Node099\nDIJKSTRA Node000 Node099\n'
    printf 'PATH Node099 Node000\n'
} > "$CASE_DIR/queries.txt"

"$ROOT_DIR/build/knowledge_graph" "$CASE_DIR/entities.csv" \
    "$CASE_DIR/relations.csv" "$CASE_DIR/queries.txt" > "$CASE_DIR/actual.txt"
diff -u --strip-trailing-cr "$ROOT_DIR/tests/regression/growth_expected.txt" "$CASE_DIR/actual.txt"
printf '[OK] graph growth (100 vertices)\n'
