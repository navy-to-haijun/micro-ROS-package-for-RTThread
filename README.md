# micro-ROS-package-for-RTThread

This package has been tested in RT-Thread v4.1.0 with [STMH750-ART-Pi](https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi).

## Dependencies

This component needs `colcon`  `scons` and other Python 3 packages in order to build micro-ROS packages:

```bash
pip3 install catkin_pkg lark-parser empy colcon-common-extensions
```

## Usage1

### Supported board

 RT-Thread v4.1.0 with [STMH750-ART-Pi](https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi)

### Creating micro-ROS firmware 

```bash
ros2 run micro_ros_setup create_firmware_ws.sh rtthread art-pi
```

### Configuring micro-ROS firmware

```bash
ros2 run micro_ros_setup configure_firmware.sh [app name] [options]
```

Available apps：

* micro_ros_pub_int32.c
* micro_ros_sub_int32.c
* micro_ros_pub_sub_int32.c
* micro_ros_ping_pong.c
* micro_ros_addtwoints_server.c
* icro_ros_addtwoints_client.c

available options:

* `-d` device 

* `-i ` IP `-p` port

for example:

```bash
# serial
ros2 run micro_ros_setup configure_firmware.sh  micro_ros_pub_int32.c -d vcom
# UDP
ros2 run micro_ros_setup configure_firmware.sh micro_ros_pub_int32.c -i 192.168.31.130 -p 9999
```

### Building micro-ROS 

```bash
ros2 run micro_ros_setup build_firmware.sh
```

### Flashing micro-ROS 

```bash
ros2 run micro_ros_setup flash_firmware.sh
```

## agent

```bash
# serial
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:galactic serial -D [DEVICE] -v6
# UDP
docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:galactic udp4 --port [PORT] -v6
```



## Usage2

## Supported platforms

[BSP](https://github.com/RT-Thread/rt-thread/tree/master/bsp) with rtthread

1. Clone rtthread rpeo:

```bash
git clone -b v4.1.1 https://github.com/RT-Thread/rt-thread.git
```

2. Select  BSP. For example: using  BSP of stm32h750-artpi 

```bash
cd  bsp/stm32/stm32h750-artpi
```

3. Clone this rpeo

```bash
git clone https://github.com/navy-to-haijun/micro-ROS-rtthread-app
```

4. Configure the driver code for UART or UDP by myself.
5. Rtthread supports cmake：

```bash
scons --target=cmake
```

Modify the `CMakeLists.txt` to include the following code before the build the project：

```cmake
# build micro-ROS : make build_microros
add_custom_target(build_microros
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/micro-ROS-rtthread-app/microros"
	COMMAND sh generate_microros_library.sh ${CMAKE_C_COMPILER} ${CMAKE_CXX_COMPILER} ${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}
	COMMENT "build micro-ROS..."
)

# head files
INCLUDE_DIRECTORIES(micro-ROS-rtthread-app/microros/build/include)
INCLUDE_DIRECTORIES(micro-ROS-rtthread-app/transports)

#  teansport: serial（optional）
add_definitions(-DMICROROS_SERIAL)
add_definitions(-DMICROROS_DEVIVE="vcom")
list(APPEND PROJECT_SOURCES micro-ROS-rtthread-app/transports/rtt_serial_transports.c)

#  teansport: serial（optional）
# add_definitions(-DMICROROS_UDP)
# add_definitions(-DMICROROS_IP="192.168.31.130")
# add_definitions(-DMICROROS_PORT=9999)
# list(APPEND PROJECT_SOURCES micro-ROS-rtthread-app/transports/rtt_udp_transport.c.c)

# example 
# pub_int32 （optional）
add_definitions(-DMICROS_EXAMPLE_PUB_INT32)
list(APPEND PROJECT_SOURCES micro-ROS-rtthread-app/examples/micro_ros_pub_int32.c)

link_directories(${CMAKE_SOURCE_DIR}/micro-ROS-rtthread-app/microros/build)
LINK_LIBRARIES(microros)
```

6. Execute

```bash
cd build
cmake ..
# generate microROS static library
make build_microros
# build all project
make
```

