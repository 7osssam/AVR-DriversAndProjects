# wykys Makefile for AVR

ifndef NAME
NAME = PRJ
endif

SRC = ../src/
LIB = ../01-LIB/
MCAL = ../02-MCAL/
HAL = ../03-HAL/
APP = ../APP/Fan_Controller_system/

MCU = atmega32a

CC = avr-gcc -fdiagnostics-color=always
CFLAGS = -mmcu=$(MCU) -Wall -std=c99 -O1 -I$(LIB) -I$(MCAL) -I$(HAL) -I$(APP)
AFLAGS = -mmcu=$(MCU) -Wall -x assembler-with-cpp

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

.PHONY: dependence clean $(NAME)

# build all
all: $(NAME)

# generating dependencies
$(DEP): $(shell find $(SRC) -name '*.c')
	$(CC) -I$(LIB) -I$(MCAL) -I$(HAL) -I$(APP) -MM $(shell find $(SRC) -name '*.c') > $(DEP)

# include generated dependencies
-include $(DEP)

# create object modules
%.o : $(SRC)%.c
	$(CC) -c $(CFLAGS) $< -o $@
%.o : $(LIB)%.c
	$(CC) -c $(CFLAGS) $< -o $@
%.o : $(MCAL)%.c
	$(CC) -c $(CFLAGS) $< -o $@
%.o : $(HAL)%.c
	$(CC) -c $(CFLAGS) $< -o $@
%.o : $(APP)%.c
	$(CC) -c $(CFLAGS) $< -o $@

# create ELF
$(NAME).elf: $(DEP) $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $@ -Wl,-Map=$(NAME).map

# create HEX
$(NAME).hex: $(NAME).elf
	$(OBJCOPY) -R .eeprom -O ihex $(NAME).elf $(NAME).hex

# create EEPROM
EEP.hex: $(NAME).elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $(NAME).elf $@

# disassemble ELF
$(NAME).list: $(NAME).elf
	$(OBJDUMP) -h -S $(NAME).elf > $@

# analyze ELF size
$(NAME): $(NAME).elf $(NAME).hex EEP.hex $(NAME).list
	@$(BINSIZE) -C --mcu=$(MCU) $(NAME).elf

# clean files
clean:
	$(info ===================== Clean =====================)
	$(RM) *.o *.list *.elf *.hex *.map
	
