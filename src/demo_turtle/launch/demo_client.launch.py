from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess


def generate_launch_description():
    #
    x = 3
    y = 4
    theta = 0.0
    name = "t2"

    spawn = ExecuteProcess(
        cmd=["ros2 service call /spawn turtlesim/srv/Spawn \"{'x': "
                + str(x) + ",'y': " + str(y) + ",'theta': " + str(theta) + ",'name': '" + name + "'}\""],
        # output="both",
        shell=True
    )

    # 客户端
    client = Node(package="demo_turtle",executable="demo_client", arguments=[str(x), str(y), str(theta)])

    return LaunchDescription([spawn, client])