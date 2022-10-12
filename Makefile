##############
## Binaries ##
##############

CC = arm-none-eabi-gcc

#################
## Directories ##
#################
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc
DEBUG_DIR = debug
STARTUP_DIR = startup


###########
## Files ##
###########
SRC := $(wildcard $(SRC_DIR)/*.c)
SRC += $(wildcard $(STARTUP_DIR)/*.c)

OBJ := $(patsubst $(SRC_DIR)/%.c, $(SRC_DIR)/$(OBJ_DIR)/%.o, $(SRC))
OBJ := $(patsubst $(STARTUP_DIR)/%.c, $(SRC_DIR)/$(OBJ_DIR)/%.o, $(OBJ))

LS  := $(wildcard $(STARTUP_DIR)/*.ld)

##############################
## DEBUG and FLASHING PATHS ##
##############################
STLINKV2_CONFIG_PATH=/usr/share/openocd/scripts/interface/stlink-v2.cfg
MC_CONFIG_PATH=/usr/share/openocd/scripts/target/stm32f4x.cfg

###########
## FLAGS ##
###########
MARCH = cortex-m4
CFLAGS = -std=c11 -g -Wall -Wextra -mcpu=$(MARCH) -mthumb
LFLAGS = -nostdlib -T $(LS) -Wl,-Map=$(DEBUG_DIR)/main.map

#############
## TARGETS ##
#############
TARGET = $(DEBUG_DIR)/main.elf

all: $(OBJ) $(TARGET)

$(SRC_DIR)/$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | mk_obj_dir
	$(CC) $(CFLAGS) -c -o $@ $^

$(SRC_DIR)/$(OBJ_DIR)/%.o : $(STARTUP_DIR)/%.c | mk_obj_dir
	$(CC) $(CFLAGS) -c -o $@ $^

$(TARGET) : $(OBJ) | mk_debug_dir
	$(CC) $(LFLAGS) -o $@ $^

edit:
	vim -S Session.vim

flash:
	openocd -f $(STLINKV2_CONFIG_PATH) -f $(MC_CONFIG_PATH) &
	gdb-multiarch $(TARGET) -x init.gdb
	pkill openocd

mk_obj_dir:
	mkdir -p $(SRC_DIR)/$(OBJ_DIR)

mk_debug_dir:
	mkdir -p $(DEBUG_DIR)

clean:
	rm -rf $(DEBUG_DIR) $(SRC_DIR)/$(OBJ_DIR)


.PHONY = show clean mk_debug_dir mk_obj_dir flash
