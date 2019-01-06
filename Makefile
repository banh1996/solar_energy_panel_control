PROJECT_NAME := SOLAR_PANEL

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

abspath../..:= ../../
GNU_INSTALL_ROOT := ../../TOOL_CHAIN/7_2017-q4-major
GNU_PREFIX := arm-none-eabi

MK := mkdir -p
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES += \
$(abspath ./User/main.c) \
$(abspath ./application/app_button_control.c) \
$(abspath ./application/app_gps_task.c) \
$(abspath ./application/app_motor_control.c) \
$(abspath ./application/app_photoresistor_sensor.c) \
$(abspath ./application/app_led_control.c) \
$(abspath ./application/app_mpu6050_task.c) \
$(abspath ./application/app_lcd_task.c) \
$(abspath ./application/app_battery_task.c) \

#Library files
C_SOURCE_FILES += $(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/src/misc.c) \
$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c) \
$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c) \
$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c) \
$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c) \
$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c) \
$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c) \
$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c) \

#TM lib
C_SOURCE_FILES += $(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_delay.c) \
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_disco.c) \
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_gpio.c 
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_dma.c) \
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_usart.c) \
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_usart_dma.c) \
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_adc.c) \
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_pwm.c) \
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_timer_properties.c) \
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_i2c.c) \
$(abspath ../00-STM32F429_LIBRARIES/tm_stm32f4_watchdog.c) \

#system files
C_SOURCE_FILES += $(abspath ./User/stm32f4xx_it.c) \
$(abspath ./User/system_stm32f4xx.c) \

#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/CMSIS/Device/ST/STM32F4xx/Source/Templates/arm/startup_stm32f40_41xxx.s)



#includes common to all targets
INC_PATHS  = -I$(abspath ./User)
INC_PATHS  = -I$(abspath ./application)
INC_PATHS += -I$(abspath ../00-STM32F429_LIBRARIES)
INC_PATHS += -I$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/inc)
INC_PATHS += -I$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/STM32F4xx_StdPeriph_Driver/src)
INC_PATHS += -I$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/CMSIS/Device/ST/STM32F4xx/Include)
INC_PATHS += -I$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/CMSIS/Device/ST/STM32F4xx/Source/Templates/arm)
INC_PATHS += -I$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/CMSIS/Include)
INC_PATHS += -I$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/CMSIS/Device/ST/STM32F4xx/Include/stm32f4xx.h)
INC_PATHS += -I$(abspath ../00-STM32F4xx_STANDARD_PERIPHERAL_DRIVERS/CMSIS/Device/ST/STM32F4xx/Include/system_stm32f4xx.h)



ifeq ($(config), Debug)
	CONFIG := DEBUG
	OPT := -O0 -g3
	BOARD += -DDEBUG_NRF
	BOARD += -DDEBUG
	VERSION := $(shell grep VERSION Inc/SOLAR_PANEL.h | cut -d \" -f2)
	OBJECT_DIRECTORY = build/debug
else
	CONFIG := RELEASE
	OPT := -O3 -g0
	BOARD += -DNDEBUG_NRF
	VERSION := $(shell grep VERSION Inc/SOLAR_PANEL.h | cut -d \" -f2)
	OBJECT_DIRECTORY = build/release
endif

LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)/bin

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )



#flags common to all targets
CFLAGS  = -STM32F40_41xxx
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -DARM_MATH_CM4
CFLAGS += -D__FPU_PRESENT=1
CFLAGS += -DSTM32F4XX
CFLAGS += -DTM_DISCO_STM32F401_DISCOVERY
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DHSE_VALUE=8000000
CFLAGS += -DPLL_M=8
CFLAGS += -DPLL_N=336
CFLAGS += -DPLL_P=2
CFLAGS += -DPLL_Q=7
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror
CFLAGS += $(OPT)
CFLAGS += $(BOARD)
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums 
# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L Startup -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS += $(OPT)
LDFLAGS += $(BOARD)
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys
# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += $(OPT)
ASMFLAGS += $(BOARD)
ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF52
ASMFLAGS += -DNRF52832_XXAA
ASMFLAGS += -DNRF52_PAN_74
ASMFLAGS += -DSWI_DISABLE0
ASMFLAGS += -D__HEAP_SIZE=8192
ASMFLAGS += -D__STACK_SIZE=8192


.PHONY: default help clean

#default target - first one defined
default: clean SOLAR_PANEL

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e SOLAR_PANEL

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	SOLAR_PANEL
	@echo 	flash_softdevice

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

SOLAR_PANEL: OUTPUT_FILENAME := SOLAR_$(CONFIG)_$(VERSION)_$(shell date +'%Y.%m.%d')
SOLAR_PANEL: LINKER_SCRIPT=Startup/SOLAR_PANEL.ld

SOLAR_PANEL: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) -MP -MD $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Assembly file: $(notdir $<)
	$(NO_ECHO)$(CC) -MP -MD $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<
# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex
echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash: SOLAR_PANEL
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	nrfjprog --program $(OUTPUT_BINARY_DIRECTORY)/$<.hex -f nrf52  --sectorerase --verify
	nrfjprog --reset -f nrf52

# Flash softdevice
flash_softdevice:
	@echo Flashing: s132_nrf52_6.0.0_softdevice.hex
	nrfjprog -f nrf52 --program ../../components/softdevice/s132/hex/s132_nrf52_6.0.0_softdevice.hex --sectorerase
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall