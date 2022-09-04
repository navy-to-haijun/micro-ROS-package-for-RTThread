from glob import glob
from building import *
import rtconfig

# get current directory
cwd     = GetCurrentDir()

LIBS    = []
LIBPATH = []
src = []

if rtconfig.CROSS_TOOL == 'gcc':
		LIBPATH = [cwd + '/build/']

LIBS += ['microros' ]

# The set of source files associated with this SConscript file.
src	= Glob('microros_extensions/*.c')

path	= [cwd]
path	+= [cwd + '/build/include']
path	+= [cwd + '/microros_extensions']

if GetDepend('MICROS_EXAMPLE_PUB_INT32'):
    src    += Glob('examples/micro_ros_pub_int32.c')

if GetDepend('MICRO_ROS_USING_SUB_INT32'):
    src    += Glob('examples/micro_ros_sub_int32.c')

LOCAL_CCFLAGS = ''

group = DefineGroup('microros', src, depend = ['USING_MICROROS'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS, LIBS = LIBS, LIBPATH = LIBPATH)

Return('group')
