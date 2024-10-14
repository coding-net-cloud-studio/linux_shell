#!/bin/bash

input_file="meme.txt"
output_file="kk.txt"

while IFS= read -r line
do
  echo "${line}\": true," >> "$output_file"
done < "$input_file"