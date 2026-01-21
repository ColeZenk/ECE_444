# Assignment 1
PROJECT = warbling_wire

# Directories
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# MCU flags
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT_ABI = -mfloat-abi=hard

# Compiler flags
CFLAGS = $(CPU) $(FPU) $(FLOAT_ABI) -mthumb
CFLAGS += -O2 -Wall
CFLAGS += -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -I$(INC_DIR)
CFLAGS += -IRTE/_Target_1
CFLAGS += -IRTE/Device/MK22FN512VLH12
CFLAGS += -IRTE/CMSIS
CFLAGS += -DCPU_MK22FN512VLH12

# Linker flags
LDFLAGS = $(CPU) $(FPU) $(FLOAT_ABI) -mthumb
LDFLAGS += -specs=nano.specs -specs=nosys.specs
LDFLAGS += -Tlinker_script.ld
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -nostartfiles

# Source files
SOURCES = \
	$(SRC_DIR)/ADC.c \
	$(SRC_DIR)/DAC.c \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/MCG.c \
	$(SRC_DIR)/TimerInt.c \
	$(SRC_DIR)/uart.c \
	RTE/Device/MK22FN512VLH12/system_MK22F51212.c

ASM_SOURCES = startup_MK22F51212.s

# Object files
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
OBJECTS += $(BUILD_DIR)/startup.o

# Build products
ELF = $(BUILD_DIR)/$(PROJECT).elf
BIN = $(BUILD_DIR)/$(PROJECT).bin

# Default target
all: $(BUILD_DIR) $(BIN)
	@echo "Build complete!"
	@$(SIZE) $(ELF)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	mkdir -p $(BUILD_DIR)/RTE/Device/MK22FN512VLH12

# Link
$(ELF): $(OBJECTS)
	@echo "Linking..."
	$(CC) $(LDFLAGS) $^ -o $@

# Create binary
$(BIN): $(ELF)
	@echo "Creating binary..."
	$(OBJCOPY) -O binary $< $@

# Compile C files
$(BUILD_DIR)/%.o: %.c
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly
$(BUILD_DIR)/startup.o: $(ASM_SOURCES)
	@echo "Assembling..."
	$(CC) $(CFLAGS) -c $< -o $@

# Flash
flash: $(BIN)
	@echo "Flashing..."
	openocd -f interface/jlink.cfg \
		-c "transport select swd" \
		-f target/kx.cfg \
		-c "init" \
		-c "kinetis fcf_source write" \
		-c "program $(BIN) verify reset exit 0x0"

# Mass erase (use if chip is locked)
erase:
	@echo "Mass erasing..."
	openocd -f interface/jlink.cfg \
		-c "transport select swd" \
		-f target/kx.cfg \
		-c "init" \
		-c "kinetis mdm mass_erase" \
		-c "exit"

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Format
format:
	clang-format -i $(SOURCES) $(wildcard $(INC_DIR)/*.h)

.PHONY: all clean flash erase format
