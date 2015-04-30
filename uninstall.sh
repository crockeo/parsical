#!/bin/bash

while read p; do
    rm $p
done < install_manifest.txt
