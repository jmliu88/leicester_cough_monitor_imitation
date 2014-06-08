################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../main.cpp \
../trainModel.cpp 

OBJS += \
./main.o \
./trainModel.o 

CPP_DEPS += \
./main.d \
./trainModel.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/software/htk/HTKLib -O0 -g3 -Wall -c -fmessage-length=0  -gdwarf-2 -gstrict-dwarf -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


