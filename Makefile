#
# A Makefile that compiles all .c and .s files in "src" and "res"
# subdirectories and places the output in a "obj" subdirectory
#

# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
GBDK_HOME = ../../gbdk2020/gbdk-2020-git/build/gbdk/

LCC = $(GBDK_HOME)bin/lcc

# You can set flags for LCC here
# For example, you can uncomment the line below to turn on debug output
# LCCFLAGS = -debug

# MBC5 + Ram + Battery, ROM Banks=0, RAM Banks = 2, DMG+CGB support
# -Wf--max-allocs-per-node50000
# -Wl-w   Wide map listing
# -Wl-g_shadow_OAM=0xC800 -Wl-b_DATA=0xc8a0  // Create unallocated RAM area from 0xC000 -> 0xC800 for manual data placement
LCCFLAGS = -debug -Wl-yt0x1B -Wm-yn"CANYON" -Wl-w -Wl-ya1 -Wm-yS -Wm-yc -Wl-g_shadow_OAM=0xC800 -Wl-b_DATA=0xc8a0


# You can set the name of the .gb ROM file here
PROJECTNAME    = canyon_parallax

SRCDIR      = src
OBJDIR      = obj
RESDIR      = res
BINS	    = $(OBJDIR)/$(PROJECTNAME).gb
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
OBJS       = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

all:	prepare $(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> compile.bat

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# If needed, compile .c files i n"src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -S -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS):	$(OBJS)
	$(LCC) $(LCCFLAGS) -o $(BINS) $(OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
#	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	rm -f  $(OBJDIR)/*.*

