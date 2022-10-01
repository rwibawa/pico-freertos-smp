# pico-freertos-smp

FreeRTOS Kernel SMP for the Raspberry Pi Pico.

* [Video Tutorial](https://youtu.be/nD8XeWjn-2w)
* [sample github](https://github.com/jondurrant/RPIPicoFreeRTOSSMPExp)
* [FreeRTOS SMP Docs](https://www.freertos.org/symmetric-multiprocessing-introduction.html)

## 1. Setup

```sh
$ mkdir pico-freertos-smp
$ cd pico-freertos-smp/
$ git init
$ git submodule add https://github.com/FreeRTOS/FreeRTOS-Kernel
$ cd FreeRTOS-Kernel/
$ git checkout V202110.00-SMP
$ cd ..
$ mkdir lib
$ mv FreeRTOS-Kernel lib/

# modify the git module's path
$ vi .gitmodules

# cmake presets
$ export PICO_SDK_PATH=~/workspace_pico/pico-sdk
$ export PICO_TOOLCHAIN_PATH=/c/Program\ Files\ \(x86\)/Arm\ GNU\ Toolchain\ arm-none-eabi/12.2\ mpacbti-bet1/bin
$ export PICO_PLATFORM=rp2040
$ export FREERTOS_KERNEL_PATH=~/workspace_pico/pico-freertos-smp/lib/FreeRTOS-Kernel

# import pico sdk
$ cp $PICO_SDK_PATH/external/pico_sdk_import.cmake ./

# import FreeRTOS-SMP
$ cp lib/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake ./
# fix the FREERTOS_KERNEL_RP2040_RELATIVE_PATH
$ vi lib/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake
$ diff FreeRTOS_Kernel_import.cmake lib/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake 
11,13d10
< else ()
<       set(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel")
<     message("Using local FREERTOS_KERNEL_PATH '${FREERTOS_KERNEL_PATH}')")
18,19c15
< # set(FREERTOS_KERNEL_RP2040_BACK_PATH "../../../..")
<
---
> set(FREERTOS_KERNEL_RP2040_BACK_PATH "../../../..")
66,69c62
< add_subdirectory(${FREERTOS_KERNEL_PATH}/${FREERTOS_KERNEL_RP2040_RELATIVE_PATH} FREERTOS_KERNEL)
<
< #FORCE LIBRARY BUILD
< include(${FREERTOS_KERNEL_PATH}/${FREERTOS_KERNEL_RP2040_RELATIVE_PATH}/library.cmake)
---
> add_subdirectory(${FREERTOS_KERNEL_PATH}/${FREERTOS_KERNEL_RP2040_RELATIVE_PATH} FREERTOS_KERNEL)
\ No newline at end of file
```

## 2. Build the solution

Open the *msys mingw64* terminal:
![*msys mingw64* terminal](img/build-01.jpg)

```sh
$ rm -rf build/
$ mkdir build
$ cd build/
$ cmake --version
cmake version 3.24.2

$ cmake -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
Using PICO_SDK_PATH from environment ('C:/msys64/home/WRY/workspace_pico/pico-sdk')
PICO_SDK_PATH is C:/msys64/home/WRY/workspace_pico/pico-sdk
Using PICO_PLATFORM from environment ('rp2040')
Defaulting PICO platform compiler to pico_arm_gcc since not specified.       
PICO compiler is pico_arm_gcc
-- The C compiler identification is GNU 12.2.0
-- The CXX compiler identification is GNU 12.2.0
-- The ASM compiler identification is GNU
-- Found assembler: C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/12.2 mpacbti-bet1/bin/arm-none-eabi-gcc.exe
Build type is Debug
Using regular optimized debug build (set PICO_DEOPTIMIZED_DEBUG=1 to de-optimize)
Defaulting PICO target board to pico since not specified.
Using board configuration from C:/msys64/home/WRY/workspace_pico/pico-sdk/src/boards/include/boards/pico.h
-- Found Python3: C:/msys64/mingw64/bin/python3.10.exe (found version "3.10.7") found components: Interpreter
TinyUSB available at C:/msys64/home/WRY/workspace_pico/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040; enabling build support for USB.
Compiling TinyUSB with CFG_TUSB_DEBUG=1
cyw43-driver available at C:/msys64/home/WRY/workspace_pico/pico-sdk/lib/cyw43-driver
lwIP available at C:/msys64/home/WRY/workspace_pico/pico-sdk/lib/lwip
Using FREERTOS_KERNEL_PATH from environment ('C:/msys64/home/WRY/workspace_pico/pico-freertos-smp/lib/FreeRTOS-Kernel')
-- Configuring done
-- Generating done
-- Build files have been written to: C:/msys64/home/WRY/workspace_pico/pico-freertos-smp/build

$ make -j8

# gdb-server
$ cd ~/workspace_pico/openocd
$ src/openocd.exe -f tcl/interface/picoprobe.cfg -f tcl/target/rp2040.cfg -s tcl
```
