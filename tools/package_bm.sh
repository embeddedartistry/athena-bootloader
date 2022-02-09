#!/bin/bash

# Create package for board manager (package_AB_index.json) with the required directory structure

OUTPUT_DIR=./buildresults/release
PACKAGE_TMP_DIR=./buildresults/tmp
PACKAGE_NAME=athena-bootloader

rm -rf ${PACKAGE_TMP_DIR}

mkdir -p ${PACKAGE_TMP_DIR}/${PACKAGE_NAME}
cp -rp avr/* ${PACKAGE_TMP_DIR}/${PACKAGE_NAME}

cd ${PACKAGE_TMP_DIR}
zip -r $OLDPWD/${OUTPUT_DIR}/athena_bootloaders-bm.zip ${PACKAGE_NAME}
