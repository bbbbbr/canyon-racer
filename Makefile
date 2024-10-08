# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifndef GBDK_HOME
	GBDK_HOME = ../../gbdk2020/gbdk-2020-git/build/gbdk/
endif

ZIP = zip
PNG2ASSET = $(GBDK_HOME)bin/png2asset
GBCOMPRESS = $(GBDK_HOME)bin/gbcompress
LCC = $(GBDK_HOME)bin/lcc

VERSION=0.7.7

# You can set flags for LCC here
# For example, you can uncomment the line below to turn on debug output
# LCCFLAGS += -debug

# LCC Verbose
LCCFLAGS += -v

# TODO: For Debug testing and watchpoint trigger, remove when done to free up RAM
# Move data a little furter after shadow oam to try and catch overflows
LCCFLAGS += -Wl-b_DATA=0xC100

# Alternate languages can be passed in as follows
# - 32k_nosave
# - 31k_1kflash
# - mbc5
# make CART_TYPE=<cart type>
ifndef CART_TYPE
#	CART_TYPE=32k_nosave
#	CART_TYPE=mbc5
	CART_TYPE=mbc5_rumble
#	CART_TYPE=31k_1kflash
endif

# Set ROM Title / Name
LCCFLAGS += -Wm-yn"CANYONRACER"
	# Legal chars are from 0x20 - 0x5F
	# Optional Alternate title that generates a Green BG / Red Obj CGB palette (id:0x0E -> checksum 0xBD)
 	# LCCFLAGS += -Wm-yn"CANYONRACERDD" # Or "CANYONRACER["
	#
	# Optional Alternate title that generates a Grey CGB palette (id:0x16 -> checksum 0x58)
 	# LCCFLAGS += -Wm-yn"CANYONRACER-FXX" # "CANYONRACER>GFX" # "CANYONRACER=MAX" # "CANYONRACER!VVV"
    #
	# Brown BG, Green / Blue Sprites
	# checksum 0xA2 or 0xF7
	#    "CANYONRACER$FX" with makefile and shell escapes
	# LCCFLAGS += -Wm-yn"CANYONRACER\$$FX"

# Set CGB Boot ROM color palette to 0x13 (relies on title settings above)
# 1. Old Licensee is already 0x33 -> Use New Licensee
# 2. Sets New Licensee to "01" "(Nintendo)
# 3. (Calculated by Sum of ROM Header title bytes 0x134 - 0x143) & 0xFF = 0x35 -> CGB Boot Pal 0x12 (brown)
#    https://gbdev.io/pandocs/Power_Up_Sequence.html#compatibility-palettes
#    https://tcrf.net/Notes:Game_Boy_Color_Bootstrap_ROM#Manual_Select_Palette_Configurations
LCCFLAGS += -Wm-yk01


# OPTIONAL: If more speed/ROM is needed can turn up optimization (at compile speed cost)
CFLAGS += -DCART_$(CART_TYPE) -D_MAP_FX_$(MAP_FX_TOGGLE)
# CFLAGS += -Wf--max-allocs-per-node50000
# CFLAGS += -Wf--max-allocs-per-node150000 # diminishing (but present) size returns after this
# CFLAGS += -Wf--max-allocs-per-node500000


# Configure platform specific LCC flags here:
LCCFLAGS_gb      = # -Wm-yc # No Color support (DMG mode on the CGB)
LCCFLAGS_pocket  = # -Wm-yc # No Color support (DMG mode on the CGB)
LCCFLAGS_duck    = # No MBC
LCCFLAGS_gbc     =
LCCFLAGS_sms     =
LCCFLAGS_gg      =


### Handle cart specific flags

# MBC5 - *NO* Rumble
ifeq ($(CART_TYPE),mbc5)
	TARGETS=gb pocket
	LCCFLAGS_gb      += -Wl-yt0x1B -Wl-ya1 # Set an MBC for banking:0x1B 	MBC-5 	SRAM 	BATTERY 		8 MB
	LCCFLAGS_pocket  += -Wl-yt0x1B -Wl-ya1 # Same as for .gb
	CART_TYPE_INC_DIR = mbc5
endif

# MBC5 - *WITH* Rumble
ifeq ($(CART_TYPE),mbc5_rumble)
	TARGETS=gb pocket megaduck
	LCCFLAGS_gb      += -Wl-yt0x1E -Wl-ya1 # Set an MBC for banking:0x1E   MBC-5   SRAM   BATTERY   RUMBLE   8 MB
	LCCFLAGS_pocket  += -Wl-yt0x1E -Wl-ya1 # Same as for .gb
	CART_TYPE_INC_DIR = mbc5
endif

# 31K+1k cart loses 1024 bytes at the end for flash storage
ifeq ($(CART_TYPE),31k_1kflash)
	# No reason to build .pocket for the 31K + 1k flash cart
	TARGETS=gb
	# Add the flash 1K region as an exclusive no-use area for rom usage calcs
	ROMUSAGE_flags = -e:FLASH_SAVE:7C00:400
	CART_TYPE_INC_DIR = 31k_1kflash
endif

# Generic 32 Cart with no save support
ifeq ($(CART_TYPE),32k_nosave)
	TARGETS=gb pocket megaduck megaduck-nofx
	CART_TYPE_INC_DIR = 32k_nosave
endif


# Targets can be forced with this override, but normally they will be controlled per-cart type above
#
# Set platforms to build here, spaced separated. (These are in the separate Makefile.targets)
# They can also be built/cleaned individually: "make gg" and "make gg-clean"
# Possible are: gb gbc pocket megaduck sms gg
# TARGETS=gb pocket


LCCFLAGS += $(LCCFLAGS_$(EXT)) # This adds the current platform specific LCC Flags

# Super Game Boy (border) support (not-enabled)
# LCCFLAGS += -Wm-ys

# No autobanking needed for 32k ROM
# CFLAGS += -Wl-j -Wm-yoA -Wm-ya4 -autobank -Wb-ext=.rel -Wb-v # MBC + Autobanking related flags
# CFLAGS += -v     # Uncomment for lcc verbose output

# Add pre-compiled Music Driver to lib path
# LIBS_INC = -Wl-llib/hUGEDriver.lib
ifeq ($(PLAT),duck)
	OBJ_MUSIC_DRIVER_COMPILED = lib/hUGEDriver_megaduck_with_compression_sm83.obj.o
else
	OBJ_MUSIC_DRIVER_COMPILED = lib/hUGEDriver_with_compression_sm83.obj.o
endif
LCCFLAGS += $(LIBS_INC)

# You can set the name of the ROM file here
PROJECTNAME    = canyon_racer_$(VERSION)_$(CART_TYPE)

# -Wl-w   Wide map listing
CFLAGS += -debug -Wl-w
# CFLAGS += -Wf-MMD -Wf-Wp-MP -Wf--verbose
CFLAGS += -Wf-MMD -Wf-Wp-MP

# Add include path for type of flash cart if enabled
CFLAGS += -Wf-I"$(CART_TYPE_DIR)/"

# Add language directory to include path
CFLAGS += -Wf-I"$(LANGDIR)/"
SRCDIR         = src
AUDIODIR       = $(SRCDIR)/audio
SFXDIR         = $(SRCDIR)/audio/sfx
SONGSDIR       = $(SRCDIR)/audio/songs
CART_TYPE_DIR  = $(SRCDIR)/cart_$(CART_TYPE_INC_DIR)
PACKAGE_DIR    = package
BUILD_DIR      = build

# Add audio dir to include path
CFLAGS += -Wf-I"$(AUDIODIR)/"

OBJDIR      = obj/$(EXT)/$(CART_TYPE)
RESDIR      = res
BINDIR      = $(BUILD_DIR)/$(EXT)
MKDIRS      = $(OBJDIR) $(BINDIR) $(BUILD_DIR) $(PACKAGE_DIR) # See bottom of Makefile for directory auto-creation

BINS	      = $(OBJDIR)/$(PROJECTNAME).$(EXT)
CSOURCES      = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c)))
CSOURCES      += $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
CSOURCES      += $(foreach dir,$(AUDIODIR),$(notdir $(wildcard $(dir)/*.c)))
CSOURCES      += $(foreach dir,$(SFXDIR),$(notdir $(wildcard $(dir)/*.c)))
CSOURCES      += $(foreach dir,$(SONGSDIR),$(notdir $(wildcard $(dir)/*.c)))
CSOURCES_CART = $(foreach dir,$(CART_TYPE_DIR),$(notdir $(wildcard $(dir)/*.c)))

ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
ASMSOURCES_CART = $(foreach dir,$(CART_TYPE_DIR),$(notdir $(wildcard $(dir)/*.s)))

OBJS        = $(CSOURCES:%.c=$(OBJDIR)/%.o)
OBJS        += $(CSOURCES_CART:%.c=$(OBJDIR)/%.o)

OBJS        += $(ASMSOURCES:%.s=$(OBJDIR)/%.o)
OBJS        += $(ASMSOURCES_CART:%.s=$(OBJDIR)/%.o)

# Builds all targets sequentially
all: $(TARGETS)

# Dependencies
DEPS = $(OBJS:%.o=%.d)

-include $(DEPS)

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "audio/" to .o object files
$(OBJDIR)/%.o:	$(AUDIODIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "audio/" to .o object files
$(OBJDIR)/%.o:	$(SFXDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "audio/" to .o object files
$(OBJDIR)/%.o:	$(SONGSDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "src/<CART_TYPE_DIR>/" to .o object files
$(OBJDIR)/%.o:	$(CART_TYPE_DIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(CART_TYPE_DIR)/%.s
	$(LCC) $(CFLAGS) -c -o $@ $<


# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(CFLAGS) -c -o $@ $<


# If needed, compile .c files i n"src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(CFLAGS) -S -o $@ $<PROJECTNAME

# Link the compiled object files into a .gb ROM file
$(BINS):	$(OBJS)
	$(LCC) $(LCCFLAGS) $(CFLAGS) -o $(BINDIR)/$(PROJECTNAME).$(EXT) $(OBJS) $(OBJ_MUSIC_DRIVER_COMPILED)
# Optional Packaging
# - at start of line ignores error code for zip, which returns non-success if file was already "up to date"
# -j removes paths
ifdef ADD_ZIP
	-cd $(BUILD_DIR); $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_ROMs_$(VERSION).zip $(EXT)/$(PROJECTNAME).$(EXT)
endif
# Optional Cart flash
# 6. insideGadgets 2 MByte 128KB SRAM Flash Cart (ULP) (Optional Rumble)
ifdef FLASH_CART
	-cd tools/gbxcart_gb; ./gbxcart_rw_flasher_v1.50 ../../$(BUILD_DIR)/$(EXT)/$(PROJECTNAME).$(EXT) 42
endif


# No longer need this with -MP now available
# Rule to absorb .h header files which have been removed but may be
# mentioned in a .d dependency file (generated by previous -MMD to compiler)
#
# SDCC lacks a -MP to generate the per-header phony rules
# And PHONY doesn't support wildcards, so just have a target for .h which does nothing
#
# Prevents: make[1]: *** No rule to make target 'someheader.h', needed by 'someobjfile.o'.  Stop.
# %.h:
#	
# Don't remove the tab-only line direcly above


# Convert source PNGs -> C source
# These are TARGET independent
assets:
	# Canyon BG Map
	$(PNG2ASSET) $(RESDIR)/map_canyon.png -map -noflip
	# Ship
	# -pw/h is hitbox
	# px/y -8, -16 offset is to remove GB hardware sprite offset
	# Actual ship is (0,0)-(31, 25)
	# Make hitbox forgiving (8 x 18), see entity_ship.h for alignement details
	$(PNG2ASSET) $(RESDIR)/sprite_ship.png -sw 24 -sh 32 -px -8 -py -16 -pw 8 -ph 17  -spr8x16 -c $(RESDIR)/sprite_ship.c
	# Old, stricter hitbox:
	# $(PNG2ASSET) $(RESDIR)/sprite_ship.png -sw 24 -sh 32 -px -8 -py -16 -pw 8 -ph 24  -spr8x16 -c $(RESDIR)/sprite_ship.c

	# Obstacles
	# -pw/h is hitbox, -8, -16 offset is to remove GB hardware sprite offset
	$(PNG2ASSET) $(RESDIR)/sprite_obstacles.png -sw 32 -sh 16 -px -8 -py -16 -pw 32 -ph 8  -spr8x16 -c $(RESDIR)/sprite_obstacles.c
	# Font Numbers Sprites
	$(PNG2ASSET) $(RESDIR)/font_nums.png -keep_duplicate_tiles -keep_palette_order -sw 8 -sh 16 -noflip -tiles_only -spr8x16 -c $(RESDIR)/tiles_font_nums.c
	# Pause Sprites
	$(PNG2ASSET) $(RESDIR)/sprite_pause.png -keep_duplicate_tiles -keep_palette_order -sw 8 -sh 16 -noflip -tiles_only -spr8x16 -c $(RESDIR)/sprite_pause.c
	# Ready Sprites
	$(PNG2ASSET) $(RESDIR)/sprite_ready.png -keep_duplicate_tiles -keep_palette_order -sw 8 -sh 16 -noflip -tiles_only -spr8x16 -c $(RESDIR)/sprite_ready.c
	# Splash Logo
	# Non-compressed version
	# $(PNG2ASSET) $(RESDIR)/splash_logo.png -map -c $(RESDIR)/splash_logo.c

	# Splash Logo screen COMPRESSED version (saves ~800 bytes)
	# -> .bin -> compress -> .c
	$(PNG2ASSET) $(RESDIR)/splash_logo.png -map -noflip -bin -c $(RESDIR)/splash_logo_data.c
	$(GBCOMPRESS) -v --cout --varname=splash_logo_map_comp   $(RESDIR)/splash_logo_data_map.bin   $(RESDIR)/splash_logo_map_comp.c
	$(GBCOMPRESS) -v --cout --varname=splash_logo_tiles_comp $(RESDIR)/splash_logo_data_tiles.bin $(RESDIR)/splash_logo_tiles_comp.c

	# Font Numbers used on BG with Intro Logo
	$(PNG2ASSET) $(RESDIR)/font_nums_bg.png -keep_palette_order -map -tiles_only -c $(RESDIR)/tiles_font_nums_bg.c
	# Game over Sprites
	# -8, -16 offset is to remove GB hardware sprite offset
	$(PNG2ASSET) $(RESDIR)/game_over.png -tiles_only -keep_palette_order -px -8 -py -16  -spr8x16 -c $(RESDIR)/game_over.c

	# Intro credits screen
	# Non-compressed version
	#  Needs both -bpp 1 -pack_mode 1bpp
	# $(PNG2ASSET) $(RESDIR)/intro_credits.png -bpp 1 -pack_mode 1bpp -map -noflip -c res/intro_credits_data.c
	#
	# Intro credits screen COMPRESSED version (saves 300+ bytes)
	# -> .bin -> compress -> .c
	rm -f $(RESDIR)/intro_credits*.c $(RESDIR)/intro_credits*.h $(RESDIR)/intro_credits*.bin
	$(PNG2ASSET) $(RESDIR)/intro_credits.png -bpp 1 -pack_mode 1bpp -map -noflip -bin -c $(RESDIR)/intro_credits_data.c
	$(GBCOMPRESS) -v --cout --varname=intro_credits_map_comp   $(RESDIR)/intro_credits_data_map.bin   $(RESDIR)/intro_credits_map_comp.c
	$(GBCOMPRESS) -v --cout --varname=intro_credits_tiles_comp $(RESDIR)/intro_credits_data_tiles.bin $(RESDIR)/intro_credits_tiles_comp.c

	# Help screen
	# Non-compressed version
	#  Needs both -bpp 1 -pack_mode 1bpp
	# $(PNG2ASSET) $(RESDIR)/help_screen.png -bpp 1 -pack_mode 1bpp -map -noflip -c res/help_screen_data.c

	# Sound Test screen
	# Sinlge 8x8 sprite cursor
	$(PNG2ASSET) $(RESDIR)/sprite_sound_test_cursor.png -sw 8 -sh 8 -spr8x8 -c $(RESDIR)/sprite_sound_test_cursor.c

	# Intro credits screen COMPRESSED version (saves ~250 bytes)
	# -> .bin -> compress -> .c
	rm -f $(RESDIR)/help_screen*.c $(RESDIR)/help_screen*.h $(RESDIR)/help_screen*.bin
	$(PNG2ASSET) $(RESDIR)/help_screen.png -bpp 1 -pack_mode 1bpp -map -noflip -bin -c $(RESDIR)/help_screen_data.c
	$(GBCOMPRESS) -v --cout --varname=help_screen_map_comp   $(RESDIR)/help_screen_data_map.bin   $(RESDIR)/help_screen_map_comp.c
	$(GBCOMPRESS) -v --cout --varname=help_screen_tiles_comp $(RESDIR)/help_screen_data_tiles.bin $(RESDIR)/help_screen_tiles_comp.c
	#
	# MegaDuck on AP Notice
	#
	# Mega Duck AP Notice screen COMPRESSED version (saves 300+ bytes)
	# -> .bin -> compress -> .c
	rm -f $(RESDIR)/megaduck_on_ap*.c $(RESDIR)/megaduck_on_ap*.h $(RESDIR)/megaduck_on_ap*.bin
	$(PNG2ASSET) $(RESDIR)/megaduck_on_ap.png -bpp 1 -pack_mode 1bpp -map -noflip -bin -c $(RESDIR)/megaduck_on_ap_data.c
	$(GBCOMPRESS) -v --cout --varname=megaduck_on_ap_map_comp   $(RESDIR)/megaduck_on_ap_data_map.bin   $(RESDIR)/megaduck_on_ap_map_comp.c
	$(GBCOMPRESS) -v --cout --varname=megaduck_on_ap_tiles_comp $(RESDIR)/megaduck_on_ap_data_tiles.bin $(RESDIR)/megaduck_on_ap_tiles_comp.c
	# Add in the ifdef so the data doesn't get included for GB
	echo "#include <gbdk/platform.h>" > $(RESDIR)/megaduck_on_ap_map_comp.c.new
	echo "#include <gbdk/platform.h>" > $(RESDIR)/megaduck_on_ap_tiles_comp.c.new
	echo "#ifdef MEGADUCK" >> $(RESDIR)/megaduck_on_ap_map_comp.c.new
	echo "#ifdef MEGADUCK" >> $(RESDIR)/megaduck_on_ap_tiles_comp.c.new
	cat $(RESDIR)/megaduck_on_ap_map_comp.c >> $(RESDIR)/megaduck_on_ap_map_comp.c.new
	cat $(RESDIR)/megaduck_on_ap_tiles_comp.c >> $(RESDIR)/megaduck_on_ap_tiles_comp.c.new
	echo "#endif" >> $(RESDIR)/megaduck_on_ap_map_comp.c.new
	echo "#endif" >> $(RESDIR)/megaduck_on_ap_tiles_comp.c.new
	# overwrite original with ifdef version
	mv -f $(RESDIR)/megaduck_on_ap_map_comp.c.new $(RESDIR)/megaduck_on_ap_map_comp.c
	mv -f $(RESDIR)/megaduck_on_ap_tiles_comp.c.new $(RESDIR)/megaduck_on_ap_tiles_comp.c

carts:
	${MAKE} CART_TYPE=31k_1kflash
	${MAKE} CART_TYPE=mbc5
	${MAKE} CART_TYPE=mbc5_rumble
	${MAKE} CART_TYPE=32k_nosave

package: zip-carts

zip-fonts:
	-cd font; $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_fonts.zip Readme.md
	-cd font; $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_fonts.zip notify_font/*.png
	-cd font; $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_fonts.zip splash_font/*.png
	-cd font; $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_fonts.zip gameover_font/*.png
	-cd font; $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_fonts.zip sound_test_font/*.png
	-cd font; $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_fonts.zip notify_font/ttf/*.ttf
	-cd font; $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_fonts.zip splash_font/ttf/*.ttf
	-cd font; $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_fonts.zip gameover_font/ttf/*.ttf
	-cd font; $(ZIP) -u ../$(PACKAGE_DIR)/canyon_racer_fonts.zip sound_test_font/ttf/*.ttf

zip-carts:
	${MAKE} ADD_ZIP=YES  CART_TYPE=31k_1kflash
	${MAKE} ADD_ZIP=YES  CART_TYPE=mbc5
	${MAKE} ADD_ZIP=YES  CART_TYPE=mbc5_rumble
	${MAKE} ADD_ZIP=YES  CART_TYPE=32k_nosave
	-$(ZIP) -u $(PACKAGE_DIR)/canyon_racer_ROMs_$(VERSION).zip README.md
# -j junk path
	-$(ZIP) -j -u $(PACKAGE_DIR)/canyon_racer_ROMs_$(VERSION).zip info/Which\ ROM\ File\ To\ Use.txt
	-$(ZIP) -j -u $(PACKAGE_DIR)/canyon_racer_ROMs_$(VERSION).zip info/Changelog.md

zip-clean:
	rm -f $(PACKAGE_DIR)/canyon_racer_ROMs_$(VERSION).zip

carts-clean:
	${MAKE} CART_TYPE=31k_1kflash clean
	${MAKE} CART_TYPE=mbc5 clean
	${MAKE} CART_TYPE=mbc5_rumble clean
	${MAKE} CART_TYPE=32k_nosave clean


run:
	java -jar ~/gbdev/Emulators/Emulicious/Emulicious.jar $(BUILD_DIR)/gb/$(PROJECTNAME).gb &

romusage:
# Ignores failure if romusage not in path
	-romusage $(BUILD_DIR)/gb/$(PROJECTNAME).noi -sRd -g $(ROMUSAGE_flags) -e:STACK:DEFF:100 -e:SHADOW_OAM:C000:A0 -E
	-romusage $(BUILD_DIR)/gb/$(PROJECTNAME).noi $(ROMUSAGE_flags) -e:STACK:DEFF:100 -e:SHADOW_OAM:C000:A0 -E > romusage.txt

# Needs stock inside gadgets firmware to work, can use flashgbx ui to swap it out if needed
# Make sure 32K cart is specified
flashduck:
	${MAKE} CART_TYPE=32k_nosave flashduck_run

flashduck_run:
	-cd tools/gbxcart_duck; ./gbxcart_rw_megaduck_32kb_flasher ../../$(BUILD_DIR)/duck/$(PROJECTNAME).duck &

flashcart:
	${MAKE} FLASH_CART=YES

save-clear:
	rm -f $(BUILD_DIR)/gb/$(PROJECTNAME).sav

clean:
	@echo Cleaning
	@for target in $(TARGETS); do \
		$(MAKE) $$target-clean; \
	done

# Include available build targets
include Makefile.targets


# create necessary directories after Makefile is parsed but before build
# info prevents the command from being pasted into the makefile
ifneq ($(strip $(EXT)),)           # Only make the directories if EXT has been set by a target
$(info $(shell mkdir -p $(MKDIRS)))
endif
