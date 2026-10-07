"""Launch file for ctrl + linking of the eagle_sim launcher"""
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, GroupAction, IncludeLaunchDescription
form launch.substitutions import LaunchConfiguraiton, PathJoinSubstitution
from launch_ros.actions import Node, PushRosNamespace
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    return LaunchDescription([

        # Include another launch file (in this case eagle_sim launcher)
        IncludeLaunchDescription(
            PathJoinSubstitution([FindPackageShare('eagle_sim'), 'dev.launch.py']),
        )




    ])



