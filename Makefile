##################
# User Variables #
##################

NAME = program
SRC_DIR = src
SRC_FILENAMES = main.c isr.c system.c abort.c gpio.c i2c.c ssd1306.c ssd1306_fonts.c 
BUILD_DIR = build
CONFIG_DIR = mcu_config
CUBE_PATH = ../STM32Cube_FW_F1_V1.8.0
TREE = STM32F1xx
DEVICE = STM32F103xB
FLASH_ADDR = 0x08000000
SERIES_CPU = cortex-m3
SERIES_ARCH = armv7-m
# HAL_DRIVERS should match what's enabled in the HAL config file.
HAL_DRIVERS = cortex gpio rcc flash i2c dma
PREFIX = arm-none-eabi-
DEFINES =
LIBS =

######################
# Internal Variables #
######################

TREE_LOWER = $(shell echo $(TREE) | tr '[:upper:]' '[:lower:]')
DEVICE_LOWER = $(shell echo $(DEVICE) | tr '[:upper:]' '[:lower:]')
DEVICE_UPPER = $(shell echo $(DEVICE) | tr '[:lower:]' '[:upper:]')

HAL_DIR = $(CUBE_PATH)/Drivers/$(TREE)_HAL_Driver
HAL_SRC_DIR = $(HAL_DIR)/Src
HAL_CONFIG_PATH = $(HAL_DIR)/Inc/$(TREE_LOWER)_hal_conf_template.h
CMSIS_DIR = $(CUBE_PATH)/Drivers/CMSIS
CMSIS_TREE_DIR = $(CMSIS_DIR)/Device/ST/$(TREE)
SYSTEM_SRC_PATH = $(CMSIS_TREE_DIR)/Source/Templates/system_$(TREE_LOWER).c
STARTUP_PATH = $(CMSIS_TREE_DIR)/Source/Templates/gcc/startup_$(DEVICE_LOWER).s
LINKER_SCRIPT_PATH = $(CMSIS_TREE_DIR)/Source/Templates/gcc/linker/$(DEVICE_UPPER)_FLASH.ld

CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy

CFLAGS = -Os
CFLAGS += -Wall
CFLAGS += -std=c11
CFLAGS += -mcpu=$(SERIES_CPU)
CFLAGS += -march=$(SERIES_ARCH)
CFLAGS += -mlittle-endian
CFLAGS += -mthumb
CFLAGS += -masm-syntax-unified
CFLAGS += -specs=nosys.specs
CFLAGS += -ffreestanding
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -D$(DEVICE)
CFLAGS += -DUSE_HAL_DRIVER
CFLAGS += -DMCU_HEADER='<$(TREE_LOWER).h>'
CFLAGS += -I$(CMSIS_DIR)/Include
CFLAGS += -I$(CMSIS_TREE_DIR)/Include
CFLAGS += -I$(HAL_DIR)/Inc
CFLAGS += -I$(SRC_DIR)
CFLAGS += -I$(CONFIG_DIR)
CFLAGS += $(addprefix -D,$(DEFINES))

LDFLAGS = -Wl,-T$(LINKER_SCRIPT_PATH)
LDFLAGS += -Wl,--gc-sections

LDLIBS = $(addprefix -l,$(LIBS))

SRC_PATHS = $(addprefix $(SRC_DIR)/,$(SRC_FILENAMES))
OBJ_PATHS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRC_PATHS)))
SYSTEM_OBJ_PATH = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SYSTEM_SRC_PATH)))
HAL_SRC_PATHS = $(HAL_SRC_DIR)/$(TREE_LOWER)_hal.c $(addsuffix .c,$(addprefix $(HAL_SRC_DIR)/$(TREE_LOWER)_hal_,$(HAL_DRIVERS)))
HAL_OBJ_PATHS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(HAL_SRC_PATHS)))
ELF_PATH = $(BUILD_DIR)/$(NAME).elf
BIN_PATH = $(BUILD_DIR)/$(NAME).bin

###########
# Targets #
###########

# Alias for the program target, see below.
all: program

# Use `make program` to build the program, ready to be flashed (see below).
program: $(BUILD_DIR)/$(NAME).bin

# Use `make clean` to clean the build directory.
clean:
	@echo "[RM] $(BUILD_DIR)/*"
	@rm -f $(BUILD_DIR)/*

# Use `make hal_config` to setup the HAL config file from the STM32Cube template file.
# Only needs to be done once usually.
hal_config:
	@echo "[CP] $(HAL_CONFIG_PATH)"
	@mkdir -p $(CONFIG_DIR)
	@cp $(HAL_CONFIG_PATH) $(CONFIG_DIR)/stm32f1xx_hal_conf.h

# Use `make flash` to flash the output binary program to the STM32 device.
flash:
	@echo "[ST-FLASH] $(BUILD_DIR)/$(NAME).bin @ $(FLASH_ADDR)"
	st-flash write $(BUILD_DIR)/$(NAME).bin $(FLASH_ADDR)

.PHONY: all clean hal_config flash program

$(BUILD_DIR):
	@echo "[MKDIR] $@"
	@mkdir -p $(BUILD_DIR)

$(OBJ_PATHS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "[CC] $^"
	@$(CC) $(CFLAGS) -c -o $@ $^

$(SYSTEM_OBJ_PATH): $(SYSTEM_SRC_PATH)
	@echo "[CC] $^"
	@$(CC) $(CFLAGS) -c -o $@ $^

$(HAL_OBJ_PATHS): $(BUILD_DIR)/%.o: $(HAL_SRC_DIR)/%.c
	@echo "[CC] $^"
	@$(CC) $(CFLAGS) -c -o $@ $^

$(ELF_PATH): $(OBJ_PATHS) $(SYSTEM_OBJ_PATH) $(HAL_OBJ_PATHS)
	@echo "[LD] $@"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(STARTUP_PATH) $^ $(LDLIBS)

$(BIN_PATH): $(ELF_PATH)
	@echo "[OBJCOPY] $@"
	@$(OBJCOPY) -O binary $^ $@
