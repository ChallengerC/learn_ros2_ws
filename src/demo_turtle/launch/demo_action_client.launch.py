from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

def generate_launch_description():
    # 目标点坐标
    x = 8.3
    y = 9.1
    theta = 0.0
    name = "t3"

    # 
    spawn_turtle = ExecuteProcess(
        cmd=['ros2', 'service', 'call', '/spawn', 'turtlesim/srv/Spawn', f'{{x: {x}, y: {y}, theta: {theta}, name: "{name}"}}'],
        output='screen'
    )

    # 启动 action 客户端节点，通过 arguments 传递目标坐标 (对应 argv[1] argv[2] argv[3])
    action_client_node = Node(
        package='demo_turtle',
        executable='demo_action_client',
        name='demo_action_client',
        output='screen',
        arguments=[str(x), str(y), str(theta)]
    )

    return LaunchDescription([spawn_turtle, action_client_node])