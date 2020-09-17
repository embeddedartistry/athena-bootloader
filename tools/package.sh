#!/bin/bash

OUTPUT_DIR=./buildresults/release

# Argument 1 is the (optional) version number
mkdir -p ${OUTPUT_DIR}
zip ${OUTPUT_DIR}/athena_bootloaders.zip avr/bootloaders/athena/*.hex
