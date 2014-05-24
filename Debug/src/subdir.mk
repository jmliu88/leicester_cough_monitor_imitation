################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/WrapHInit.o 

CPP_SRCS += \
../src/CommonTrain.cpp \
../src/CoughTrain.cpp \
../src/EmbeddedTrain.cpp \
../src/FillerTrain.cpp \
../src/HashTable.cpp \
../src/PreProcess.cpp \
../src/Recognition.cpp \
../src/ThreadBase.cpp \
../src/Util.cpp \
../src/WrapHCompV.cpp \
../src/WrapHCopy.cpp \
../src/WrapHERest.cpp \
../src/WrapHHEd.cpp \
../src/WrapHInit.cpp \
../src/WrapHParse.cpp \
../src/WrapHRest.cpp \
../src/WrapHVite.cpp \
../src/glb.cpp \
../src/mfccReader.cpp \
../src/wavformat.cpp 

C_SRCS += \
../src/bstdfile.c 

OBJS += \
./src/CommonTrain.o \
./src/CoughTrain.o \
./src/EmbeddedTrain.o \
./src/FillerTrain.o \
./src/HashTable.o \
./src/PreProcess.o \
./src/Recognition.o \
./src/ThreadBase.o \
./src/Util.o \
./src/WrapHCompV.o \
./src/WrapHCopy.o \
./src/WrapHERest.o \
./src/WrapHHEd.o \
./src/WrapHInit.o \
./src/WrapHParse.o \
./src/WrapHRest.o \
./src/WrapHVite.o \
./src/bstdfile.o \
./src/glb.o \
./src/mfccReader.o \
./src/wavformat.o 

C_DEPS += \
./src/bstdfile.d 

CPP_DEPS += \
./src/CommonTrain.d \
./src/CoughTrain.d \
./src/EmbeddedTrain.d \
./src/FillerTrain.d \
./src/HashTable.d \
./src/PreProcess.d \
./src/Recognition.d \
./src/ThreadBase.d \
./src/Util.d \
./src/WrapHCompV.d \
./src/WrapHCopy.d \
./src/WrapHERest.d \
./src/WrapHHEd.d \
./src/WrapHInit.d \
./src/WrapHParse.d \
./src/WrapHRest.d \
./src/WrapHVite.d \
./src/glb.d \
./src/mfccReader.d \
./src/wavformat.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/software/htk/HTKLib -O0 -g3 -Wall -c -fmessage-length=0  -gdwarf-2 -gstrict-dwarf -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/software/htk/HTKLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


