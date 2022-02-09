# you can set this to 1 to see all commands that are being run
export VERBOSE := 0

ifeq ($(VERBOSE),1)
export Q :=
export VERBOSE := 1
else
export Q := @
export VERBOSE := 0
endif

BUILDRESULTS?=buildresults
CONFIGURED_BUILD_DEP = $(BUILDRESULTS)/build.ninja

version := $(subst -, ,$(shell git describe --long --dirty --tags))
export BUILD_TAG :=$(strip $(word 1, $(version)))

.PHONY: all
all: makerelease

# Runs whenever the build has not been configured successfully
$(CONFIGURED_BUILD_DEP):
	$(Q) meson $(BUILDRESULTS)

.PHONY: makerelease
makerelease:
	$(Q)cd avr/bootloaders/athena/src/; ./makerelease

.PHONY: clean
clean:
	$(Q)make -C avr/bootloaders/athena/src/ clean

.PHONY: distclean
distclean:
	$(Q) rm -rf $(BUILDRESULTS)

.PHONY: package
package:
	$(Q)tools/package.sh
	$(Q)tools/package_bm.sh
	$(Q) mv $(BUILDRESULTS)/release/athena_bootloaders.zip $(BUILDRESULTS)/release/athena_bootloaders-$(BUILD_TAG).zip
	$(Q) mv $(BUILDRESULTS)/release/athena_bootloaders-bm.zip $(BUILDRESULTS)/release/athena_bootloaders-bm-$(BUILD_TAG).zip

.PHONY: dist
dist: $(CONFIGURED_BUILD_DEP)
	$(Q) meson dist -C $(BUILDRESULTS) --formats zip
	$(Q) mv $(BUILDRESULTS)/meson-dist/AthenaBootloader-*.zip $(BUILDRESULTS)/meson-dist/AthenaBootloaderProjectFiles-$(BUILD_TAG).zip
	$(Q) mv $(BUILDRESULTS)/meson-dist/AthenaBootloader-*.zip.sha256sum $(BUILDRESULTS)/meson-dist/AthenaBootloaderProjectFiles-$(BUILD_TAG).zip.sha256sum

.PHONY: format
format:
	$(Q)tools/format/clang-format-all.sh

.PHONY : format-diff
format-diff :
	$(Q)tools/format/clang-format-git-diff.sh

.PHONY : format-check
format-check :
	$(Q)tools/format/clang-format-patch.sh
