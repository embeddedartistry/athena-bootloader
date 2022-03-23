#!/bin/bash
# Create package for board manager (package_AB_index.json) with the required directory structure

VERSION=`git describe --tags`

ORIGIN_DIR=`pwd`
OUTPUT_DIR=${ORIGIN_DIR}/buildresults/release
PACKAGE_TMP_DIR=buildresults/tmp
PACKAGE_NAME=athena-bootloader
OUTPUT_FILENAME=${OUTPUT_DIR}/athena_bootloaders-bm.zip

# Generate .zip archive for board manager using generated artifacts
rm -rf ${PACKAGE_TMP_DIR}
mkdir -p ${PACKAGE_TMP_DIR}/${PACKAGE_NAME}
cp -rp avr/* ${PACKAGE_TMP_DIR}/${PACKAGE_NAME}
cd ${PACKAGE_TMP_DIR}
zip -X -r $OUTPUT_FILENAME ${PACKAGE_NAME} -x "*.DS_Store*"

# Store checksum and other details for the newly generated package
CHECKSUM=`shasum -a 256 $OUTPUT_FILENAME | awk '{ print $1 }'`
SIZE=`wc -c $OUTPUT_FILENAME | awk '{print $1 }'`

# Generate the new board manager JSON file
cd ${ORIGIN_DIR}
cat tools/package_athena-bootloader_index.template.json |
sed s/%%VERSION%%/${VERSION}/ |
sed s/%%CHECKSUM%%/${CHECKSUM}/ |
sed s/%%SIZE%%/${SIZE}/ > package_athena-bootloader_index.json
