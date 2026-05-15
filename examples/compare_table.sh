#!/bin/bash
# Shows k rows (integers 1..k) comparing three decoding routes for dimension m.
#
# Columns:
#   alpha         — the integer being decoded
#   GTP Vector V  — strictly increasing vector from InverseGTP(alpha, m)
#   DP via GTP    — unrestricted vector from GTP_to_DP(V)  (general route)
#   DP Analitico  — unrestricted vector from the closed-form inverse (m=2,3,4)
#
# Usage: ./compare_table.sh <m> <k>   (2 <= m <= 4)

m=$1
k=$2

if [ -z "$m" ] || [ -z "$k" ]; then
    echo "Usage: $0 <m> <k>   (2 <= m <= 4)"
    exit 1
fi

if [ "$m" -lt 2 ] || [ "$m" -gt 4 ]; then
    echo "Error: m must be between 2 and 4"
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BINARY="$SCRIPT_DIR/../build/compare_dp"

if [ ! -x "$BINARY" ]; then
    echo "Error: '$BINARY' not found. Run 'make' first."
    exit 1
fi

printf "%-6s | %-18s | %-18s | %-18s\n" \
    "alpha" "GTP Vector V" "DP via GTP" "DP Analitico"
printf "%s\n" \
    "-------+--------------------+--------------------+--------------------"

for (( i=0; i<k; i++ ))
do
    "$BINARY" -DIM "$m" -INPUT "$i"
done
