#!/bin/bash
# This script shows the k Box Polynomial vectors given the m dimension.
# Usage: ./bp_table.sh [m] [k]

echo "No | Box Polynomial Vector"

for (( i=0; i<$2; i++ ))
do
  res=$(polyf -DIM $1 -INPUT $i -FROM int -TO poly -TYPE BP)
  printf "%-3d| %-10s\n" "$i" "$res"
done