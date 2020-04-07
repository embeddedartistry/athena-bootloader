#!/bin/bash

OUTPUT_DIR=./buildresults/release

# Argument 1 is the (optional) version number
mkdir -p ${OUTPUT_DIR}
tar -cf ${OUTPUT_DIR}/ariadne_bootloaders.tgz avr/bootloaders/ariadne*
