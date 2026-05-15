#!/bin/bash
# This script shows the k Greater-Than Polynomial vectors given the m dimension.
# Usage: ./gtp_table.sh [m] [k]

echo "No | Greater-Than Polynomial Vector"

for (( i=0; i<$2; i++ ))
do
  res=$(polyf -DIM $1 -INPUT $i -FROM int -TO poly -TYPE GTP)
  printf "%-3d| %-10s\n" "$i" "$res"
done