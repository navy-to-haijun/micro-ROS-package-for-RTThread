# 使用交叉编译 目标：嵌入式平台
SET(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
# 交叉编译链
set(TOOLS /home/haijun/env_released_1.2.0/gcc-arm-none-eabi-5_4-2016q3/bin/)
set(CMAKE_C_COMPILER ${TOOLS}arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLS}arm-none-eabi-g++)


SET(CMAKE_C_COMPILER_WORKS 1 CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_WORKS 1 CACHE INTERNAL "")



# SET HERE YOUR BUILDING FLAGS
set(FLAGS "-O2 -mfloat-abi=softfp -mfpu=fpv5-sp-d16 -mfloat-abi=hard  -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -fno-exceptions -mcpu=cortex-m7 -DF_CPU=480000000L -DARDUINO=10813 -mthumb -DSTM32H750XX" CACHE STRING "" FORCE)


set(CMAKE_C_FLAGS_INIT "-std=c11 ${FLAGS} -DCLOCK_MONOTONIC=0 -D'__attribute__(x)='" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_INIT "-std=c++11 ${FLAGS} -fno-rtti -DCLOCK_MONOTONIC=0 -D'__attribute__(x)='" CACHE STRING "" FORCE)

set(__BIG_ENDIAN__ 0)

