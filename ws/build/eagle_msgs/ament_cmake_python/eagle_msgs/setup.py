from setuptools import find_packages
from setuptools import setup

setup(
    name='eagle_msgs',
    version='0.1.0',
    packages=find_packages(
        include=('eagle_msgs', 'eagle_msgs.*')),
)
