################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ABP_presure.c \
../Core/Src/IHMctrl.c \
../Core/Src/VCNL3040.c \
../Core/Src/com.c \
../Core/Src/dma.c \
../Core/Src/feeder.c \
../Core/Src/gpio.c \
../Core/Src/hardControl.c \
../Core/Src/i2c.c \
../Core/Src/ioext.c \
../Core/Src/main.c \
../Core/Src/motor.c \
../Core/Src/option.c \
../Core/Src/spi.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/ABP_presure.o \
./Core/Src/IHMctrl.o \
./Core/Src/VCNL3040.o \
./Core/Src/com.o \
./Core/Src/dma.o \
./Core/Src/feeder.o \
./Core/Src/gpio.o \
./Core/Src/hardControl.o \
./Core/Src/i2c.o \
./Core/Src/ioext.o \
./Core/Src/main.o \
./Core/Src/motor.o \
./Core/Src/option.o \
./Core/Src/spi.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/ABP_presure.d \
./Core/Src/IHMctrl.d \
./Core/Src/VCNL3040.d \
./Core/Src/com.d \
./Core/Src/dma.d \
./Core/Src/feeder.d \
./Core/Src/gpio.d \
./Core/Src/hardControl.d \
./Core/Src/i2c.d \
./Core/Src/ioext.d \
./Core/Src/main.d \
./Core/Src/motor.d \
./Core/Src/option.d \
./Core/Src/spi.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -DDEBUG -c -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ABP_presure.d ./Core/Src/ABP_presure.o ./Core/Src/IHMctrl.d ./Core/Src/IHMctrl.o ./Core/Src/VCNL3040.d ./Core/Src/VCNL3040.o ./Core/Src/com.d ./Core/Src/com.o ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/feeder.d ./Core/Src/feeder.o ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/hardControl.d ./Core/Src/hardControl.o ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/ioext.d ./Core/Src/ioext.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/motor.d ./Core/Src/motor.o ./Core/Src/option.d ./Core/Src/option.o ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/usart.d ./Core/Src/usart.o

.PHONY: clean-Core-2f-Src

