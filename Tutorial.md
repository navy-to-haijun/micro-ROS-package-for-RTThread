

# Tutorial

## 概述

2022开源之夏[2022开源之夏](https://summer-ospp.ac.cn/#/org/prodetail/22f330436)项目：microROS 添加 RT-Thread 支持。第二三问的code使用教程。代码在上测试

将microROS添加到RT-Thread支持，提供以下两种方式：

* 在ROS2的环境中，基于`micro_ros_setup`软件包实现；
* 在cmake的环境中，基于shell脚本实现；

测试环境：[STMH750-ART-Pi](https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi), ROS 2： galactic

## micro_ros_setup

优点：

* 该方式通过脚本适配了单片机的外设，下载环境；

缺点：

* 目前只为art-pi 适配；

### 1. 建立micro_ros_setup环境

```bash
# 下载code
mkdir ros2_rtthread/src
cd ros2_rtthread/src
git clone -b rtthread https://github.com/navy-to-haijun/micro_ros_setup.git
# 编译
rosdep update && rosdep install --from-paths src --ignore-src -y
colcon build
source install/local_setup.bash
```

### 2. 下载microROS相关code

不加参数，可以查看支持的参数

```bash
ros2 run micro_ros_setup create_firmware_ws.sh rtthread art-pi
```

* 下载ament相关包；
* 下载microROS相关包；
* 下载STMH750-ART-Pi相关代码；
* 下载为rtthread提供的transport和example相关code；
* 下载交叉编译链：gcc-arm-none-eabi-5_4-2016q3；

### 3. 为rtthread 配置 app(example)和transport

不加参数，可以查看支持的参数

 ```bash
 ros2 run micro_ros_setup configure_firmware.sh [app name] [options]
 ```

app:

* micro_ros_pub_int32.c
* micro_ros_sub_int32.c
* micro_ros_pub_sub_int32.c
* micro_ros_ping_pong.c
* micro_ros_addtwoints_server.c
* icro_ros_addtwoints_client.c

options:

* -d device ： 使用USB虚拟串口；

* -i  ip : 使用UDP时， agent的IP；

* -p port: 使用UDP时， agent监控的端口；

```bash
# 使用 micro_ros_pub_int32.c demo； 使用USB虚拟串口
ros2 run micro_ros_setup configure_firmware.sh  micro_ros_pub_int32.c -d vcom
# or
# 使用 micro_ros_pub_int32.c demo； 使用UDP
ros2 run micro_ros_setup configure_firmware.sh micro_ros_pub_int32.c -i 192.168.31.130 -p 9999
```

### 4. 编译

```
ros2 run micro_ros_setup build_firmware.sh 
```

* 编译 microROS
* 编译rtthread

### 5. 下载

请提前安装好STM32CubeProgrammer，安装路径默认，

检查STM32CubeProgrammer的默认安装路径是否是`/usr/local/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin`

```bash
ros2 run micro_ros_setup flash_firmware.sh 
```

### 运行

```bash
# 启动 art-pi 确保虚拟串口OK /dev/ttyACM1
#  agent 
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:galactic serial -D /dev/ttyACM1 -v6
# or
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:galactic udp4 --port 9999 -v6
# microROS 
microros_pub_int
# 查看话题
ros2 topic list
ros2 topic echo /micro_ros_rtt_node_publisher
```



## shell 脚本

优点：

* 通过脚本将microROS编译成静态库，理论上适用于所有支持rtthread的工程；

缺点

* microROS编译成静态库的过程，需要提前将rtthread工程转化为支持cmake构建的工程；
* 需要自己实现UDP和uart的驱动；

### 1. 下载 rtthread  code

```bash
# 下载rtthread  （对于其他单片机，使用rtthread仓库）
git clone https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi.git

# 进入目标bsp , 下载 git clone https://github.com/navy-to-haijun/micro-ROS-rtthread-app.git
cd cd sdk-bsp-stm32h750-realthread-artpi/projects/art_pi_wifi/
git clone https://github.com/navy-to-haijun/micro-ROS-rtthread-app.git
```

micro-ROS-rtthread-app:

```bash
.
├── examples									# demo
│   ├── micro_ros_addtwoints_client.c
│   ├── micro_ros_addtwoints_server.c
│   ├── micro_ros_ping_pong.c
│   ├── micro_ros_pub_int32.c
│   ├── micro_ros_pub_sub_int32.c
│   └── micro_ros_sub_int32.c
├── Kconfig										# 可以通过 scons --menuconfig 配置 microROS的通信方式和使用的demo
├── microros									# 建立microROS的脚本文件
│   ├── colcon.meta								# microROS相关包的宏定义，可以根据单片机的内存大小，修改支持的话题、服务、节点的数目
│   ├── generate_microros_library.sh			# 下载，编译microROS
│   └── toolchain.cmake.in						
├── README.md
├── rtconfig.h									# 脚本模式下，无用
├── SConscript
└── transports									# 通信方式
    ├── micro_ros_rtt.h
    ├── rtt_serial_transports.c
    └── rtt_udp_transport.c
```



### 2. rtthead 支持 cmake构建

```bash
# 在rtconfig.py 中 添加 gcc-arm-none-eabi-5_4 的编译路径

if  CROSS_TOOL == 'gcc':
    PLATFORM    = 'gcc'
    EXEC_PATH   = r'/home/haijun/env_released_1.2.0/gcc-arm-none-eabi-5_4-2016q3/bin'

# art- pi 需要
ln -s ../../libraries/ libraries
ln -s ../../rt-thread/ rt-thread
# 支持cmake构建
scons --target=cmake
```

### 3. 修改 工程目录下的`CMakeLists.txt`，支持microROS

代码添加到`ADD_EXECUTABLE()`之前

```cmake
# build micro-ROS : make build_microros
# build micro-ROS : make build_microros
add_custom_target(build_microros
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/micro-ROS-rtthread-app/microros"
	COMMAND sh generate_microros_library.sh ${CMAKE_C_COMPILER} ${CMAKE_CXX_COMPILER} ${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}
	COMMENT "build micro-ROS..."
)

# head files
INCLUDE_DIRECTORIES(micro-ROS-rtthread-app/microros/build/include)
INCLUDE_DIRECTORIES(micro-ROS-rtthread-app/transports)

#  teansport: serial
add_definitions(-DMICROROS_SERIAL)
add_definitions(-DMICROROS_DEVIVE="vcom")
list(APPEND PROJECT_SOURCES micro-ROS-rtthread-app/transports/rtt_serial_transports.c)

# example 
# pub_int32
add_definitions(-DMICROS_EXAMPLE_PUB_INT32)
list(APPEND PROJECT_SOURCES micro-ROS-rtthread-app/examples/micro_ros_pub_int32.c)

link_directories(${CMAKE_SOURCE_DIR}/micro-ROS-rtthread-app/microros/build)
LINK_LIBRARIES(microros)

```

```bash

```

