# E-Agle TRT AS Controls Software Task

## Table of Contents

- [Structure of the Repository](#structure-of-the-repository)
- [Get Started](#get-started)
- [How to Run](#how-to-run)
- [Additional Notes](#additional-notes)

---

## Structure of the Repository

``` text
.
├── personal_notes.txt               // Personal notes
├── images
│   ├── path_simulation.png
│   ├── plot_(KP_and_KI).png
│   ├── plot_(KP_KI_KD).png
│   ├── plot_(KP_only).png
│   ├── plot(output_and_speed_during_run).png
│   └── rqt_graph.png
├── README.md
└── ws
   ├── can
   │   └── eagle_task.dbc
   ├── docker
   │   └── Dockerfile 
   ├── README.md  
   ├── rosbag  
   │   ├── metadata.yaml  
   │   └── rosbag_0.db3  
   └── src  
       ├── ctrl             // Package created for this task
       │   ├── can  
       │   │   ├── eagle_task.c  
       │   │   └── eagle_task.h  
       │   ├── CMakeLists.txt  
       │   ├── config  
       │   │   └── params.yaml  
       │   ├── include  
       │   │   └── ctrl  
       │   ├── launch  
       │   │   ├── ctrl.launch.py
       │   ├── package.xml  
       │   ├── rviz  
       │   │   └── ctrl.rviz  
       │   └── src  
       │       ├── ctrl_centerline.cpp  
       │       ├── ctrl_PID.cpp  
       │       └── ctrl_PID.h  
       ├── eagle_msgs  
       │   ├── CMakeLists.txt  
       │   ├── msg  
       │   │   ├── ConeArray.msg  
       │   │   └── Cone.msg  
       │   └── package.xml  
       └── eagle_sim  
           ├── config  
           │   └── track.yaml  
           ├── eagle_sim  
           │   ├── can_gateway.py  
           │   ├── fake_perception.py  
           │   ├── __init__.py  
           │   ├── lateral_controller.py  
           │   ├── track_publisher.py  
           │   ├── track.py  
           │   └── vehicle_sim.py  
           ├── launch  
           │   └── dev.launch.py  
           ├── package.xml  
           ├── resource  
           │   └── eagle_sim  
           ├── rviz  
           │   └── base.rviz  
           ├── setup.cfg  
           └── setup.py
```

---
## Get Started

First, clone this repository to your local machine

``` sh
git clone https://github.com/FrancescoBrunelli/recruiting-sw-control.git
```

### Setting up
First, build the image `Docker/dockerfile`:

``` sh
docker build -t fsproject docker/.
```

After building the docker image, we need to create the virtual CAN bus:

``` sh
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

Verify its status by doing

``` sh
ip link show vcan0
```

After creating the vcan, we need to allow docker to access our display (on X11 server). To do so we need to run:

``` sh
xhost +local:docker
```

### Run the Docker

After following the steps above we can now run the docker. We need to:
- Load and set this repository as workspace
- Pass the vcan
- Provide access to our display

To do so, run the following command inside the root of this directory (`/ws/`)

``` sh
docker run -it --rm --network host -v $(pwd):/home/eagle/ws -e DISPLAY=$DISPLAY --name fsproject -v /tmp/.X11-unix:/tmp/.X11-unix fsproject
```

Verify everything works correctly:

``` sh
pwd                          # Should show /home/eagle/ws
ip link show vcan0           # Should show vcan0 as before
rviz2                        # Should open rviz GUI
```

## How to Run
In order to run the simulation, we first need to load all required packages:

``` sh
colcon build --packages-up-to eagle_sim ctrl
```

Install the loaded packages:

``` sh
source install/setup.bash
```

Finally, launch the simulation:

``` sh
ros2 launch ctrl ctrl.launch.py
```

## Additional Notes

#### Centerline
If the running simulation doesn't display the computed centerline, we need to add it manually by:
- Click "Add"
- Select filter option "By topic"
- Select `/centerline/marker/Marker`
- Click "Ok"

#### Plots
All plots attached in the report have been made via `rqt_plot`. To show them during a run you need to:
- Run in a new terminal (inside the docker): `ros2 run rqt_plot rqt_plot`
- After launching the simulation, data for the plot are published via the `/plot` topic:
	- Under "Topic" add one (or more) of the following data:
		- `/plot/x`: displays the output provided to the car (positive means throttle, negative means brake; the code automatically clamps the values to [-1; +1])
		- `/plot/y`: displays the car's detected speed
		- `/plot/z`: displays the `target_speed` the PID is aiming for
