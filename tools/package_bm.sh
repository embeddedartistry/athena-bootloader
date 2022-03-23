#!/bin/bash
# Create package for board manager (package_AB_index.json) with the required directory structure

VERSION=`git describe --tags`

OUTPUT_DIR=buildresults/release
PACKAGE_NAME=athena-bootloader
OUTPUT_FILENAME=${OUTPUT_DIR}/athena_bootloaders-bm.zip

# Generate .zip archive for board manager using generated artifacts
mv avr ${PACKAGE_NAME}
zip -X -r $OUTPUT_FILENAME ${PACKAGE_NAME} -x "*.DS_Store*"
mv ${PACKAGE_NAME} avr

# Store checksum and other details for the newly generated package
CHECKSUM=`shasum -a 256 $OUTPUT_FILENAME | awk '{ print $1 }'`
SIZE=`wc -c $OUTPUT_FILENAME | awk '{print $1 }'`

# Generate the new board manager JSON file
cat tools/package_athena-bootloader_index.template.json |
sed s/%%VERSION%%/${VERSION}/ |
sed s/%%CHECKSUM%%/${CHECKSUM}/ |
sed s/%%SIZE%%/${SIZE}/ > package_athena-bootloader_index.json
