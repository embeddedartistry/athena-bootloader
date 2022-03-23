#!/bin/bash

# Get the description of the build commit. If the commit is in the form "X.Y.Z", we do not
# need to run the build. If it has a "-", that indicates we have commits past the last build,
# and we actually need to run.

GIT_DESCRIBE=`git describe --tags`

if [[ "$GIT_DESCRIBE" == *"-"* ]]; then
	exit 0
fi

# No need to build
exit 1
