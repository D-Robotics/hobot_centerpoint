English| [简体中文](./README_cn.md)

# Function Introduction

The laser radar target detection algorithm is the `CenterPoint` algorithm model trained on the [OpenExplorer](https://developer.horizon.ai/api/v1/fileData/horizon_j5_open_explorer_cn_doc/hat/source/examples/centerpoint.html) platform using the [nuscenes](https://www.nuscenes.org/nuscenes) dataset.

The algorithm takes 32-line laser radar point cloud data as input and outputs information including 3D detection boxes of targets, confidence, and category. Supported target detection types include car, truck, bus, barrier, motorcycle, and pedestrian, totaling six major categories.

This example uses local laser radar point cloud files as input, utilizes BPU for algorithm inference, and publishes rendered images containing point cloud data, target detection boxes, and orientation messages, displaying algorithm results on the PC browser.

# Supported Platforms

| Platform                         | System                               |
| ---------------------------- | --------------------------------------------- |
| RDK Ultra               | Ubuntu 20.04 (Foxy) |
| RDK S100               | Ubuntu 22.04 (Humble) |

# Bill of Materials


# Usage

## Function Installation

Run the following commands in the terminal of the RDK system for quick installation:

```bash
sudo apt update
sudo apt install -y tros-humble-hobot-centerpoint
sudo apt install -y tros-humble-websocket
```

## Prepare the Back-Injection Dataset

Run the following commands in the terminal of the RDK system to download and unzip the dataset:

```shell
# Download the point cloud file for back-injection on the board side
cd ~
wget http://sunrise.horizon.cc/TogetheROS/data/hobot_centerpoint_data.tar.gz

# Unzip
mkdir -p ~/centerpoint_data
tar -zxvf ~/hobot_centerpoint_data.tar.gz -C ~/centerpoint_data
```

## Start the Algorithm and Image Visualization

Run the following commands in the terminal of the RDK system to start the algorithm and visualization:

```shell
# Configure the tros.b humble environment
source /opt/tros/humble/setup.bash

# Launch the file
ln -s `ros2 pkg prefix hobot_centerpoint`/lib/qat/ qat
ln -s ~/centerpoint_data centerpoint_data

ros2 launch hobot_centerpoint hobot_centerpoint.launch.py
```

After successful startup, open the browser on the same network computer and access the IP address of RDK http://IP:8000 (IP is the IP address of RDK), you can see the real-time visual effect of the algorithm:

![centerpoint](img/centerpoint.gif)


# Interface Description

## Topics

| Name              | Message Type                       | Description                              |
| ----------------- | ---------------------------------- | ---------------------------------------- |
| /image_jpeg  | sensor_msgs/msg/Image               | Periodically publishes image topics in jpeg format  |

## Parameters

| Name                         | Parameter Value                               | Description                                 |
| ---------------------------- | --------------------------------------------- | ------------------------------------------- |
| save_image               | "True"/"False", default is "False" | Save the rendered image to the path "./render".                    |

# FAQ
