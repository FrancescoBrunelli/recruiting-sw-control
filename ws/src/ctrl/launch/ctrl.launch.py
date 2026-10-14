"""Launch file for ctrl + linking of the eagle_sim launcher"""
import os

from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory
from launch.actions import DeclareLaunchArgument, GroupAction, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node, PushRosNamespace
from launch_ros.substitutions import FindPackageShare
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    share = get_package_share_directory('ctrl')
    params = os.path.join(share, 'config', 'params.yaml')
    # Include another launch file (in this case eagle_sim launcher)
    launch_include = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(PathJoinSubstitution([FindPackageShare('eagle_sim'), 'launch/dev.launch.py']))
    )
    return LaunchDescription([

        launch_include,

        # Launch Node(s)
        Node(package='ctrl', executable='ctrl_PID', parameters=[params]),
        Node(package='ctrl', executable='ctrl_centerline'),


    ])



'''
        # Include another launch file (in this case eagle_sim launcher)
        IncludeLaunchDescription(
            PathJoinSubstitution([FindPackageShare('eagle_sim'), 'dev.launch.py']),
        ),
'''
