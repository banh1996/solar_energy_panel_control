PROJECT_NAME := SMART_KEY

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
$(abspath application/main.c) \
$(abspath application/app_button_control.c) \
$(abspath application/app_gps_task.c) \
$(abspath application/app_motor_control.c) \
$(abspath application/app_photoresistor_sensor.c) \
$(abspath application/app_led_control.c) \


#Library files
C_SOURCE_FILES += $(abspath ../../components/libraries/util/app_error.c) \
$(abspath nRF/CMSIS/Device/Src/system_nrf52.c) \
$(abspath ../../components/libraries/util/app_error_handler_gcc.c) \
$(abspath ../../components/libraries/util/app_error_weak.c) \
$(abspath ../../components/libraries/timer/app_timer.c) \
$(abspath ../../components/libraries/button/app_button.c) \
$(abspath ../../components/libraries/util/app_util_platform.c) \
$(abspath ../../components/libraries/util/nrf_assert.c) \
$(abspath ../../components/libraries/strerror/nrf_strerror.c) \
$(abspath ../../components/libraries/atomic/nrf_atomic.c) \
$(abspath ../../modules/nrfx/drivers/src/nrfx_clock.c) \
$(abspath ../../modules/nrfx/drivers/src/nrfx_power_clock.c) \
$(abspath ../../modules/nrfx/drivers/src/nrfx_power.c) \
$(abspath ../../modules/nrfx/drivers/src/nrfx_gpiote.c) \
$(abspath ../../modules/nrfx/drivers/src/nrfx_timer.c) \
$(abspath ../../components/libraries/experimental_memobj/nrf_memobj.c) \
$(abspath ../../components/libraries/balloc/nrf_balloc.c) \
$(abspath ../../external/fprintf/nrf_fprintf.c) \
$(abspath ../../external/fprintf/nrf_fprintf_format.c) \
$(abspath ../../components/libraries/atomic_flags/nrf_atflags.c) \
$(abspath ../../components/libraries/fds/fds.c) \
$(abspath ../../components/libraries/atomic_fifo/nrf_atfifo.c) \
$(abspath ../../components/libraries/experimental_section_vars/nrf_section_iter.c) \
$(abspath ../../components/libraries/pwr_mgmt/nrf_pwr_mgmt.c) \

#Segger files an
C_SOURCE_FILES += $(abspath SeggerRTT/SEGGER_RTT.c) \
$(abspath SeggerRTT/SEGGER_RTT_Syscalls_GCC.c) \
$(abspath SeggerRTT/SEGGER_RTT_printf.c) 

#Log files
C_SOURCE_FILES += $(abspath ../../components/libraries/experimental_log/src/nrf_log_backend_rtt.c) \
$(abspath ../../components/libraries/experimental_log/src/nrf_log_backend_serial.c) \
$(abspath ../../components/libraries/experimental_log/src/nrf_log_default_backends.c) \
$(abspath ../../components/libraries/experimental_log/src/nrf_log_frontend.c) \
$(abspath ../../components/libraries/experimental_log/src/nrf_log_str_formatter.c) 

#BLE files
C_SOURCE_FILES += $(abspath ../../components/ble/common/ble_advdata.c) \
  $(abspath ../../components/ble/ble_advertising/ble_advertising.c) \
  $(abspath ../../components/ble/common/ble_conn_params.c) \
  $(abspath ../../components/ble/common/ble_conn_state.c) \
  $(abspath ../../components/ble/common/ble_srv_common.c) \
  $(abspath ../../components/ble/peer_manager/gatt_cache_manager.c) \
  $(abspath ../../components/ble/peer_manager/gatts_cache_manager.c) \
  $(abspath ../../components/ble/peer_manager/id_manager.c) \
  $(abspath ../../components/ble/nrf_ble_gatt/nrf_ble_gatt.c) \
  $(abspath ../../components/ble/nrf_ble_qwr/nrf_ble_qwr.c) \
  $(abspath ../../components/ble/peer_manager/peer_data_storage.c) \
  $(abspath ../../components/ble/peer_manager/peer_database.c) \
  $(abspath ../../components/ble/peer_manager/peer_id.c) \
  $(abspath ../../components/ble/peer_manager/peer_manager.c) \
  $(abspath ../../components/ble/peer_manager/pm_buffer.c) \
  $(abspath ../../components/ble/peer_manager/pm_mutex.c) \
  $(abspath ../../components/ble/peer_manager/security_dispatcher.c) \
  $(abspath ../../components/ble/peer_manager/security_manager.c) \
  $(abspath ../../components/softdevice/common/nrf_sdh.c) \
  $(abspath ../../components/softdevice/common/nrf_sdh_ble.c) \
  $(abspath ../../components/softdevice/common/nrf_sdh_soc.c) \

#FDS
C_SOURCE_FILES +=  $(abspath ../../components/libraries/fstorage/nrf_fstorage.c) \
$(abspath ../../components/libraries/fstorage/nrf_fstorage_sd.c) \


#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath Startup/gcc_startup_nrf52.s)



#includes common to all targets
INC_PATHS  = -I$(abspath ./CMSIS_4/CMSIS/Include)
INC_PATHS += -I$(abspath ./nRF/CMSIS/Device/Include)
INC_PATHS += -I$(abspath ./SeggerRTT)
INC_PATHS += -I$(abspath ./Src)
INC_PATHS += -I$(abspath ./Src/BLE)
INC_PATHS += -I$(abspath ./Inc)
INC_PATHS += -I$(abspath ../../components)
INC_PATHS += -I$(abspath ../../components/boards)
INC_PATHS += -I$(abspath ../../components/libraries/balloc)
INC_PATHS += -I$(abspath ../../components/libraries/bsp)
INC_PATHS += -I$(abspath ../../components/libraries/delay)
INC_PATHS += -I$(abspath ../../components/libraries/experimental_log)
INC_PATHS += -I$(abspath ../../components/libraries/experimental_log/src)
INC_PATHS += -I$(abspath ../../components/libraries/experimental_memobj)
INC_PATHS += -I$(abspath ../../components/libraries/experimental_section_vars)
INC_PATHS += -I$(abspath ../../components/libraries/atomic)
INC_PATHS += -I$(abspath ../../components/libraries/atomic_fifo)
INC_PATHS += -I$(abspath ../../components/libraries/scheduler)
INC_PATHS += -I$(abspath ../../components/libraries/strerror)
INC_PATHS += -I$(abspath ../../components/libraries/timer)
INC_PATHS += -I$(abspath ../../components/libraries/button)
INC_PATHS += -I$(abspath ../../components/libraries/util)
INC_PATHS += -I$(abspath ../../components/libraries/fstorage)
INC_PATHS += -I$(abspath ../../components/libraries/fds) 
INC_PATHS += -I$(abspath ../../components/softdevice/common)
INC_PATHS += -I$(abspath ../../components/softdevice/s132/headers)
INC_PATHS += -I$(abspath ../../components/softdevice/s132/headers/nrf52)
INC_PATHS += -I$(abspath ../../modules/nrfx)
INC_PATHS += -I$(abspath ../../modules/nrfx/drivers/include)
INC_PATHS += -I$(abspath ../../modules/nrfx/hal)
INC_PATHS += -I$(abspath ../../modules/nrfx/mdk)
INC_PATHS += -I$(abspath ../../modules/nrfx/soc)
INC_PATHS += -I$(abspath ../../integration/nrfx/legacy)
INC_PATHS += -I$(abspath ../../external/fprintf)
INC_PATHS += -I$(abspath ../../components/libraries/atomic_flags)
INC_PATHS += -I$(abspath ../../components/ble/ble_advertising)
INC_PATHS += -I$(abspath ../../components/ble/common)
INC_PATHS += -I$(abspath ../../components/ble/peer_manager)
INC_PATHS += -I$(abspath ../../components/ble/nrf_ble_gatt)
INC_PATHS += -I$(abspath ../../components/ble/nrf_ble_qwr)
INC_PATHS += -I$(abspath ../../components/libraries/pwr_mgmt)
INC_PATHS += -I$(abspath ../../components/libraries/mutex)



ifeq ($(config), Debug)
	CONFIG := DEBUG
	OPT := -O0 -g3
	BOARD += -DDEBUG_NRF
	BOARD += -DDEBUG
	VERSION := $(shell grep VERSION Inc/SMART_KEY.h | cut -d \" -f2)
	OBJECT_DIRECTORY = build/debug
else
	CONFIG := RELEASE
	OPT := -O3 -g0
	BOARD += -DNDEBUG_NRF
	VERSION := $(shell grep VERSION Inc/SMART_KEY.h | cut -d \" -f2)
	OBJECT_DIRECTORY = build/release
endif

LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)/bin

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )



#flags common to all targets
CFLAGS  = -DBSP_DEFINES_ONLY
CFLAGS += -DBOARD_PCA10040
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52
CFLAGS += -DNRF52832_XXAA
CFLAGS += -DNRF52_PAN_74
CFLAGS += -DSWI_DISABLE0
CFLAGS += -DNRF_SD_BLE_API_VERSION=6
CFLAGS += -DS132
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror
CFLAGS += $(OPT)
CFLAGS += $(BOARD)
CFLAGS += -D__HEAP_SIZE=8192
CFLAGS += -D__STACK_SIZE=8192
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
default: clean SMART_KEY

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e SMART_KEY

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	SMART_KEY
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

SMART_KEY: OUTPUT_FILENAME := SMARTKEY_$(CONFIG)_$(VERSION)_$(shell date +'%Y.%m.%d')
SMART_KEY: LINKER_SCRIPT=Startup/SMART_KEY.ld

SMART_KEY: $(BUILD_DIRECTORIES) $(OBJECTS)
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

flash: SMART_KEY
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