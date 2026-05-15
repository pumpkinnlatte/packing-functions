#!/bin/bash
# This script shows the k Diagonal Polynomial vectors given the m dimension.
# Usage: ./dp_table.sh [m] [k]

echo "No | Diagonal Polynomial Vector"

for (( i=0; i<$2; i++ ))
do
  res=$(polyf -DIM $1 -INPUT $i -FROM int -TO poly -TYPE DP)
  printf "%-3d| %-10s\n" "$i" "$res"
done