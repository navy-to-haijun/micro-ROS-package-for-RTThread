#include <rtthread.h>
#include <rtdevice.h>
#include <micro_ros_rtt.h>

#if defined  MICROS_EXAMPLE_ADDTWOINTS_CLIENT
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include "example_interfaces/srv/add_two_ints.h"

#include <stdio.h>
#include <unistd.h>

static rcl_client_t client;
static rcl_allocator_t allocator;
static rclc_support_t support;
static rcl_node_t node;
static rclc_executor_t executor;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){rt_kprintf("Failed status on line %d: %d. Aborting.\n",__LINE__,(int)temp_rc); return;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){rt_kprintf("Failed status on line %d: %d. Continuing.\n",__LINE__,(int)temp_rc);}}

static example_interfaces__srv__AddTwoInts_Request req;
static example_interfaces__srv__AddTwoInts_Response res;


void client_callback(const void * msg){
  example_interfaces__srv__AddTwoInts_Response * msgin = (example_interfaces__srv__AddTwoInts_Response * ) msg;
  rt_kprintf("Received service response %ld + %ld = %ld\n", req.a, req.b, msgin->sum);
}

static void microros_addtwoint_client_thread_entry(void *parameter)
{
    while(1)
    {
        rt_thread_mdelay(100);
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    }
}


static void microros_addtwoints_client(int argc, char* argv[])
{
#if defined MICROROS_SERIAL
    // Serial setup
     set_microros_transports();
#endif

#if defined MICROROS_UDP
    // TCP setup
     set_microros_udp_transports(MICROROS_IP, MICROROS_PORT);
#endif

    allocator = rcl_get_default_allocator();
    // create init_options
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
    // create node
    RCCHECK(rclc_node_init_default(&node, "add_twoints_client_rclc", "", &support));
    // create client 
    RCCHECK(rclc_client_init_default(&client, &node, ROSIDL_GET_SRV_TYPE_SUPPORT(example_interfaces, srv, AddTwoInts), "/add_two_ints"));
    // create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
    RCCHECK(rclc_executor_add_client(&executor, &client, &res, client_callback));

    int64_t seq; 
    example_interfaces__srv__AddTwoInts_Request__init(&req);
    req.a = 24;
    req.b = 42;
    rt_thread_mdelay(2000); // Sleep a while to ensure DDS matching before sending request
    RCCHECK(rcl_send_request(&client, &req, &seq))

    rt_kprintf("Send service request %ld + %ld. Seq %ld\n",req.a, req.b, seq);

    rt_kprintf("[micro_ros] micro_ros init successful.\n");
    rt_thread_t thread = rt_thread_create("server_client", microros_addtwoint_client_thread_entry, RT_NULL, 2048, 25, 10);
    if(thread != RT_NULL)
    {
        rt_thread_startup(thread);
        rt_kprintf("[micro_ros] New thread mr_pubint32\n");
    }
    else
    {
        rt_kprintf("[micro_ros] Failed to create thread mr_pubint32\n");
    }

}

MSH_CMD_EXPORT(microros_addtwoints_client, micro ros ping_pong example);

#endif