#!/bin/bash
a=13
for i in *.png; do
  new=$(printf "%04d.jpg" "$a") #04 pad to length of 4
  mv -i -- "$i" "$new"
  let a=a+1
done

for file in *.jpg; do
    mv "$file" "$(basename "$file" .jpg).png"
done
