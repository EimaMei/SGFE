# Parameter explanations:
#   CC - compiler used by the Makefile. Setting it to 'DEFAULT' automatically picks
# the default/expected compiler used for the target.
#
#   PLATFORM - sets the platform target to compile for. Current values: WIN32_GNU,
# WIN32_MSVC, OS_X, LINUX, WASM_WASI, WASM_EMCC, 3DS, DEFAULT. Selecting 'DEFAULT'
# makes the Makefile automatically guess which platform to target.
#
# 	MODE - sets the release mode to compile for. Current values: FAST, MODE,
# RELEASE. 'FAST' disables all flags and enables ones requires for fast compilation.
# 'MODE' turns on all warnings as well as flags to help with MODEging/finding
# problematic code. 'RELEASE' turns on all optimisations as well as warnings.
#
# 	LANGUAGE - selects which programming language to target. Currently C and C++
# are the only valid options.

CC        = DEFAULT
PLATFORM  = 3DS
MODE      = FAST
LANGUAGE  = C

# Building options:
#	NAME   - the executable name.
#	SRC    - source file to target.
#	OUTPUT - the directory where all of the output goes to.
NAME   = basic
SRC    = examples/buffer/native.c
OUTPUT = build
EMULATOR = FALSE
CONSOLE_IP = NONE




ifeq ($(LANGUAGE),C)
	GNU_FLAGS = -std=c99 -x c -Wvla
else ifeq ($(LANGUAGE),CPP)
	GNU_FLAGS = -std=c++11 -x c++ -fno-exceptions
endif


ifeq ($(MODE),FAST)
	GNU_FLAGS += -O0 -flto

else
	GNU_FLAGS += \
		-Wall -Wextra -Wpedantic \
		-Wconversion -Wsign-conversion -Wdouble-promotion \
		\
		-Wpointer-arith \
		\
		-Wcast-align \
		\
		\
		-Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition \
		\
		-Wswitch-enum -Wnested-externs \
		\
		-Werror=format-security -Wformat=2 -Wformat-signedness \
		\
		-Winit-self -Wshadow -Wredundant-decls \
		\
		-Wmissing-noreturn \
		\
		-fwrapv -fstrict-aliasing \
		-fstrict-flex-arrays=3 -fno-omit-frame-pointer

	ifneq (,$(filter $(CC),gcc g++))
		GNU_FLAGS += -Wcast-align=strict -Wlogical-op
	endif

	ifeq ($(MODE),DEBUG)
		GNU_FLAGS += \
			-g \
			-fstack-clash-protection \
			-fstack-protector-strong \
			-ftrivial-auto-var-init=pattern

		ifneq ($(PLATFORM),3DS)
			GNU_FLAGS += -fsanitize=undefined -fsanitize=address
		endif
	else ifeq ($(MODE),RELEASE)
		GNU_FLAGS += \
			-O3 \
			-D SI_RELEASE_MODE \
			-fno-delete-null-pointer-checks \
			-fno-strict-aliasing \
			-ftrivial-auto-var-init=zero \
			-flto
	endif
endif

GNU_INCLUDES = -I"." -I"include"

ifeq ($(PLATFORM),3DS)
	ifeq ($(strip $(DEVKITPRO)),)
		$(error DEVKITPRO must be set in your environment: export DEVKITPRO=<path to devkitpro>)
	endif
	ifeq ($(strip $(DEVKITARM)),)
		$(error DEVKITARM must be set in your environment: export DEVKITARM=<path to devkitARM>)
	endif

	ifeq ($(CC),DEFAULT)
	CC = arm-none-eabi-gcc
	endif

	DESCRIPTION = SGFE examples for the Nintendo 3DS
	AUTHOR      = EimaMei
	ICON        = resources/3DS/default_icon.png

	FLAGS = $(GNU_FLAGS) -specs=3dsx.specs -D __3DS__ -mword-relocations -ffunction-sections -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft
	INCLUDES = -I"resources/3DS/include" $(GNU_INCLUDES) -I"$(CURDIR)" -I"$(DEVKITPRO)/libctru/include" -I"$(DEVKITARM)/include"

	LIBS = -L"resources/wii/lib" -lGLASSv2 -lkygx -lrip -L$(DEVKITPRO)/libctru/lib/ -lctru -lm
	EXE_OUT = .3dsx

	export PATH := $(DEVKITPRO)/tools/bin:$(DEVKITARM)/bin:$(DEVKITPRO)/portlibs/3ds/bin:$(PATH)

else ifeq ($(PLATFORM),WII)
	ifeq ($(strip $(DEVKITPRO)),)
		$(error DEVKITPRO must be set in your environment: export DEVKITPRO=<path to devkitpro>)
	endif
	ifeq ($(strip $(DEVKITPPC)),)
		$(error DEVKITPPC must be set in your environment: export DEVKITPPC=<path to devkitARM>)
	endif

	ifeq ($(CC),DEFAULT)
	CC = powerpc-eabi-gcc
	endif

	DESCRIPTION = SGFE examples for the Nintendo Wii
	AUTHOR      = EimaMei
	ICON        = resources/wii/default_icon.png

	FLAGS = $(GNU_FLAGS) -D GEKKO -mrvl -mcpu=750 -meabi -mhard-float
	INCLUDES = -I"resources/wii/include" $(GNU_INCLUDES) -I"$(CURDIR)" -I"$(DEVKITPRO)/libogc/include"

	LIBS = -L"resources/wii/lib" -L$(DEVKITPRO)/libogc/lib/wii -lm -lwiiuse -lbte -logc
	EXE_OUT = .dol

	export PATH := $(DEVKITPRO)/tools/bin:$(DEVKITPPC)/bin:$(DEVKITPRO)/portlibs/wii/bin:$(PATH)

else
	$(error Unsupported platform. Please refer to the Makefile for supported platofmrs.)

endif

EXE = $(OUTPUT)/$(NAME)$(EXE_OUT)


# 'make'
all: $(OUTPUT) $(EXE) run

# Run the executable.
run: $(EXE)
ifeq ($(PLATFORM),3DS)
	3dslink -r -1 $(EXE)
else ifeq ($(PLATFORM),WII)
    ifneq ($(CONSOLE_IP),NONE)
	export WIILOAD=tcp:$(CONSOLE_IP) && wiiload $(EXE)
	else
	wiiload $(EXE)
    endif
endif

# Clean the 'build' folder.
clean:
	rm -rf $(OUTPUT)/*

$(EXE): $(SRC) SGFE.h Makefile examples/*
ifeq ($(PLATFORM),3DS)
	$(CC) $(FLAGS) $(EXTRA_FLAGS) $(INCLUDES) $(SRC) $(LIBS) -o "$(OUTPUT)/$(NAME).elf"
	smdhtool --create "$(NAME)" "$(DESCRIPTION)" "$(AUTHOR)" "$(ICON)" "$(OUTPUT)/$(NAME).smdh"
	3dsxtool "$(OUTPUT)/$(NAME).elf" "$(OUTPUT)/$(NAME).3dsx" --smdh="$(OUTPUT)/$(NAME).smdh"

else ifeq ($(PLATFORM),WII)
	$(CC) $(FLAGS) $(EXTRA_FLAGS) $(INCLUDES) $(SRC) $(LIBS) -o "$(OUTPUT)/$(NAME).elf"
	elf2dol "$(OUTPUT)/$(NAME).elf" "$(OUTPUT)/$(NAME).dol"

else
	$(CC) $(FLAGS) $(SRC) $(INCLUDES) $(LIBS) $(CC_OUT)"$@"
endif

# If 'build' doesn't exist, create it
$(OUTPUT):
	mkdir $(OUTPUT)
