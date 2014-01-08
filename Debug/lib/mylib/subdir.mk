################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../lib/mylib/ConsoleDisplay.cpp \
../lib/mylib/FetchData.cpp \
../lib/mylib/Observer.cpp \
../lib/mylib/Subject.cpp 

OBJS += \
./lib/mylib/ConsoleDisplay.o \
./lib/mylib/FetchData.o \
./lib/mylib/Observer.o \
./lib/mylib/Subject.o 

CPP_DEPS += \
./lib/mylib/ConsoleDisplay.d \
./lib/mylib/FetchData.d \
./lib/mylib/Observer.d \
./lib/mylib/Subject.d 


# Each subdirectory must supply rules for building sources it contributes
lib/mylib/%.o: ../lib/mylib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


