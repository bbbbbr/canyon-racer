#
# A Makefile that compiles all .c and .s files in "src" and "res"
# subdirectories and places the output in a "obj" subdirectory
#

# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
GBDK_HOME = ../../gbdk2020/gbdk-2020-git/build/gbdk/

PNG2ASSET = $(GBDK_HOME)bin/png2asset
LCC = $(GBDK_HOME)bin/lcc

# You can set flags for LCC here
# For example, you can uncomment the line below to turn on debug output
# LCCFLAGS = -debug

# MBC5 + Ram + Battery, ROM Banks=0, RAM Banks = 2, DMG+CGB support
# -Wf--max-allocs-per-node50000
# -Wl-w   Wide map listing
# -Wm-yc <- Nope, operate in DMG compatibility mode
LCCFLAGS = -debug -Wl-yt0x1B -Wl-w -Wl-ya1 -Wm-yS -debug

# Set ROM Title / Name
LCCFLAGS += -Wm-yn"CANYONRACER"

# Set CGB Boot ROM color palette to 0x13
# 1. Old Licensee is already 0x33 -> Use New Licensee
# 2. Sets New Licensee to "01" "(Nintendo)
# 3. (Sum of ROM Header title bytes 0x134 - 0x143) & 0xFF = 0x35 -> CGB Boot Pal 0x12
#    https://tcrf.net/Notes:Game_Boy_Color_Bootstrap_ROM#Manual_Select_Palette_Configurations
LCCFLAGS += -Wm-yk01


# Platform megaduck: -msm83:duck


# You can set the name of the .gb ROM file here
PROJECTNAME    = canyon_parallax

SRCDIR      = src
OBJDIR      = obj
RESDIR      = res
BINS	    = $(OBJDIR)/$(PROJECTNAME).gb
NOI_FILE    = $(OBJDIR)/$(PROJECTNAME).noi
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
OBJS       = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

all:	prepare $(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | sed /mkdir -p/mkdir/ | grep -v make >> compile.bat

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

assets:
	# Canyon BG Map
	$(PNG2ASSET) $(RESDIR)/map_canyon.png -map -noflip
	# Ship
	# -pw/h is hitbox, -8, -16 offset is to remove GB hardware sprite offset
	$(PNG2ASSET) $(RESDIR)/sprite_ship.png -sw 24 -sh 32 -px -8 -py -16 -pw 8 -ph 24  -spr8x16 -c $(RESDIR)/sprite_ship.c
	# Obstacles
	# -pw/h is hitbox, -8, -16 offset is to remove GB hardware sprite offset
	$(PNG2ASSET) $(RESDIR)/sprite_obstacles.png -sw 32 -sh 16 -px -8 -py -16 -pw 32 -ph 8  -spr8x16 -c $(RESDIR)/sprite_obstacles.c
	# Font Numbers
	$(PNG2ASSET) $(RESDIR)/font_nums.png -keep_duplicate_tiles -keep_palette_order -sw 8 -sh 16 -noflip -tiles_only -spr8x16 -c $(RESDIR)/tiles_font_nums.c
	# Intro Logo
	$(PNG2ASSET) $(RESDIR)/splash_logo.png -map -c $(RESDIR)/splash_logo.c

romusage:
	romusage $(NOI_FILE) -g

prepare:
	mkdir -p $(OBJDIR)

clean:
#	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	rm -f  $(OBJDIR)/*.*

