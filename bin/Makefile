#  Makefile for AVR

ifndef NAME
NAME = PRJ
endif

SRC = ../src/
LIB = ../01-LIB/
MCAL = ../02-MCAL/
HAL = ../03-HAL/
APP = ../APP/Door_Locker_Security_System/

MCU = atmega32

CC = avr-gcc -fdiagnostics-color=always
COMMON_FLAGS = -mmcu=$(MCU) -Wall
C_STANDARD = c99
OPTIMIZATION_LEVEL = O3
CFLAGS = $(COMMON_FLAGS) -std=$(C_STANDARD) -$(OPTIMIZATION_LEVEL) -I$(LIB) -I$(MCAL) -I$(HAL) -I$(APP)
AFLAGS = $(COMMON_FLAGS) -x assembler-with-cpp

OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
BINSIZE = avr-size
RM = rm -rf

DEP = dependence.list

OBJFILES = $(shell find $(SRC) -name '*.c' -printf '%f ' | sed -e 's/\.c/\.o/g')
OBJFILES += $(shell find $(LIB) -name '*.c' -printf '%f ' | sed -e 's/\.c/\.o/g')
OBJFILES += $(shell find $(MCAL) -name '*.c' -printf '%f ' | sed -e 's/\.c/\.o/g')
OBJFILES += $(shell find $(HAL) -name '*.c' -printf '%f ' | sed -e 's/\.c/\.o/g')
OBJFILES += $(shell find $(APP) -name '*.c' -printf '%f ' | sed -e 's/\.c/\.o/g')

# Define color codes
COLOR_RED=\033[31m
COLOR_GREEN=\033[32m
COLOR_RESET=\033[0m

.PHONY: dependence clean $(NAME)

# build all
all: $(NAME)

# generating dependencies
$(DEP): $(shell find $(SRC) -name '*.c')
	@echo -e "$(COLOR_GREEN)===================== Generating dependencies =====================$(COLOR_RESET)"
	$(CC) -I$(LIB) -I$(MCAL) -I$(HAL) -I$(APP) -MM $(shell find $(SRC) -name '*.c') > $(DEP)

# include generated dependencies
-include $(DEP)

# create object modules
%.o : $(SRC)%.c
	@echo -e "$(COLOR_GREEN)===================== Compiling $< =====================$(COLOR_RESET)"
	$(CC) -c $(CFLAGS) $< -o $@
%.o : $(LIB)%.c
	@echo -e "$(COLOR_GREEN)===================== Compiling $< =====================$(COLOR_RESET)"
	$(CC) -c $(CFLAGS) $< -o $@
%.o : $(MCAL)%.c
	@echo -e "$(COLOR_GREEN)===================== Compiling $< =====================$(COLOR_RESET)"
	$(CC) -c $(CFLAGS) $< -o $@
%.o : $(HAL)%.c
	@echo -e "$(COLOR_GREEN)===================== Compiling $< =====================$(COLOR_RESET)"
	$(CC) -c $(CFLAGS) $< -o $@
%.o : $(APP)%.c
	@echo -e "$(COLOR_GREEN)===================== Compiling $< =====================$(COLOR_RESET)"
	$(CC) -c $(CFLAGS) $< -o $@

# create ELF
$(NAME).elf: $(DEP) $(OBJFILES)
	@echo -e "$(COLOR_GREEN)===================== Linking =====================$(COLOR_RESET)"
	$(CC) $(CFLAGS) $(OBJFILES) -o $@ -Wl,-Map=$(NAME).map

# create HEX
$(NAME).hex: $(NAME).elf
	@echo -e "$(COLOR_GREEN)===================== Creating HEX =====================$(COLOR_RESET)"
	$(OBJCOPY) -R .eeprom -O ihex $(NAME).elf $(NAME).hex

# create EEPROM
EEP.hex: $(NAME).elf
	@echo -e "$(COLOR_GREEN)===================== Creating EEPROM =====================$(COLOR_RESET)"
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $(NAME).elf $@

# disassemble ELF
$(NAME).list: $(NAME).elf
	@echo -e "$(COLOR_GREEN)===================== Disassembling ELF =====================$(COLOR_RESET)"
	$(OBJDUMP) -h -S $(NAME).elf > $@

# analyze ELF size
$(NAME): $(NAME).elf $(NAME).hex EEP.hex $(NAME).list
	@echo -e "$(COLOR_GREEN)===================== Analyzing ELF Size =====================$(COLOR_RESET)"
	@$(BINSIZE) -C --mcu=$(MCU) $(NAME).elf

# clean files
clean:
	@echo -e "$(COLOR_RED)===================== Cleaning =====================$(COLOR_RESET)"
	$(RM) *.o *.list *.elf *.hex *.map
