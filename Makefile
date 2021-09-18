# ----------------------------
# Makefile Options
# ----------------------------

NAME = TIMPANI
ICON = icon.png
DESCRIPTION = "the Texas Instruments Machine for Percussion And Novel Instruments"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------


include $(shell cedev-config --makefile)

$(info $$FASMGFLAGS is [${FASMGFLAGS}])
$(info $$ASMSOURCES is [${ASMSOURCES}])