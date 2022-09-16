# micro-ROS-package-for-RTThread

This package has been tested in RT-Thread v4.1.0 with [STMH750-ART-Pi](https://github.com/RT-Thread-Studio/sdk-bsp-stm32h750-realthread-artpi).

## Dependencies

This component needs `colcon`  `scons` and other Python 3 packages in order to build micro-ROS packages:

```bash
pip3 install catkin_pkg lark-parser empy colcon-common-extensions
```

Also, the package needs `scons`  in order to build RT-Thread

```bash
sudo apt install scons
```

## Usage

For example for `STM32H750-ART-pi` board:

### Building

Create a ROS 2 workspace and build this package for a given ROS 2 

```bash
source /opt/ros/$ROS_DISTRO/setup.bash

mkdir uros_ws && cd uros_ws

git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup

rosdep update && rosdep install --from-paths src --ignore-src -y

colcon build

source install/local_setup.bash
```

### Creating micro-ROS firmware 

```bash
ros2 run micro_ros_setup create_firmware_ws.sh rtthread art-pi
```

### Configuring micro-ROS firmware

By running this command without any argument the available demo applications and configurations will be shown.

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

Common options available at this configuration step are:

* `--transport` or `-t`: `udp`, `serial` 

for example:

```bash
ros2 run micro_ros_setup configure_firmware.sh micro_ros_ping_pong.c -t serial
```

### Building micro-ROS 

```bash
ros2 run micro_ros_setup build_firmware.sh
```

### Flashing micro-ROS 

```bash
ros2 run micro_ros_setup flash_firmware.sh
```

### Building micro-ROS-Agent

```c
ros2 run micro_ros_setup create_agent_ws.sh
ros2 run micro_ros_setup build_agent.sh
source install/local_setup.sh
```

## Run

