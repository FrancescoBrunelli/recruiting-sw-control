# Report

## Table of Contents

- [Task Roadmap](#task-roadmap)
- [Code Overview](#code-overview)
- [PID Tuning](#pid-tuning)
- [Additional Notes](#additional-notes)
- [Use of AI](#use-of-ai)

---
## Task Roadmap

1. 

---

## Code Overview

The structure of the code presents two Nodes: `/PDI` (responsible for controlling the vehicle), and `/centerline` (responsible for computing and publishing the center of the track).

![image](images/rqt_graph.png)

## `/PID`




## `/centerline`

The `/centerline` node is responsible for receiving the car-detected cones of the track that are published in the `/perception/cones` topic.

After receiving them, the Node computes the center line of the track and publishes the corresponding path to follow to `/planning/centerline`.

- To calculate the center point, the Node takes each right + left cone pair and computes the middle point between them, which pose coordinates are equal to:
  $$
(\frac{x1 + x2}{2}; \frac{y1 + y2}{2}; \frac{z1 + z2}{2}; w = 1)
  $$
	- (where `w` indicates the orientation, which in our case is never changed since it remains equal from start to finish zone)
- After calculating each `Pose` we need to wrap each of them inside a `PoseStamped` and finally append them into the `Path` object.
- Once the Path has been computed, the Node publishes it

All the steps explained above are executed every time the node receives a message from the `/perception/cones` topic.

Additionally, this node creates a "copy" of the path to follow as a list of green points (each one located at the middle of the corresponding right and left cone pairs) and a continuous blue line that connects each point. Both of such markers are published in the `centerline/marker` topic, which is then used in Rviz for displaying the path that the car is following.

The markers are updated with a frequency of 10Hz.

---
## PID Tuning



---
## Additional Notes and Materials



---
## Use of AI
In this work, AI tools (e.g., Claude) have been used solely for debugging (error explanation) and study (further clarifications about tutorials or documentation) purposes. All code and material provided by this repository has been developed by the author. AI tools were not used to generate or modify the code or the content of this work.
