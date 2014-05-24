################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../htk/HCompV.c \
../htk/HCopy.c \
../htk/HERest.c \
../htk/HHEd.c \
../htk/HInit.c \
../htk/HParse.c \
../htk/HRest.c \
../htk/HVite.c \
../htk/flags.c 

OBJS += \
./htk/HCompV.o \
./htk/HCopy.o \
./htk/HERest.o \
./htk/HHEd.o \
./htk/HInit.o \
./htk/HParse.o \
./htk/HRest.o \
./htk/HVite.o \
./htk/flags.o 

C_DEPS += \
./htk/HCompV.d \
./htk/HCopy.d \
./htk/HERest.d \
./htk/HHEd.d \
./htk/HInit.d \
./htk/HParse.d \
./htk/HRest.d \
./htk/HVite.d \
./htk/flags.d 


# Each subdirectory must supply rules for building sources it contributes
htk/%.o: ../htk/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/software/htk/HTKLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


