[English](./README.md) | 简体中文

# 功能介绍

激光雷达目标检测算法是使用[OpenExplorer](https://developer.horizon.ai/api/v1/fileData/horizon_j5_open_explorer_cn_doc/hat/source/examples/centerpoint.html)在[nuscenes](https://www.nuscenes.org/nuscenes)数据集上训练出来的`CenterPoint`算法模型。

算法输入为32线激光雷达点云数据，输出信息包括目标的3D检测框、置信度、类别。支持的目标检测类型包括car、truck、bus、barrier、motorcycle、pedestrian共六大类别。

此示例使用本地激光雷达点云文件作为输入，利用BPU进行算法推理，发布包含点云数据、目标检测框和朝向的渲染图片消息，在PC端浏览器上渲染显示算法结果。

# 支持平台

| 平台                         | 系统                               |
| ---------------------------- | --------------------------------------------- |
| RDK Ultra               | Ubuntu 20.04 (Foxy) |
| RDK S100               | Ubuntu 22.04 (Humble) |
| RDK S600               | Ubuntu 24.04 (Jazzy) |

# 物料清单


# 使用方法

## 功能安装

在RDK系统的终端中运行如下指令，即可快速安装：

```bash
sudo apt update
sudo apt install -y tros-humble-hobot-centerpoint
sudo apt install -y tros-humble-websocket
```

## 准备回灌数据集

在RDK系统的终端中运行如下指令，下载并解压数据集：

```shell
# 板端下载回灌的点云文件
cd ~
wget http://sunrise.horizon.cc/TogetheROS/data/hobot_centerpoint_data.tar.gz

# 解压缩
mkdir -p ~/centerpoint_data
tar -zxvf ~/hobot_centerpoint_data.tar.gz -C ~/centerpoint_data
```

## 启动算法和图像可视化

在RDK系统的终端中运行如下指令，启动算法和可视化：

```shell
# 配置tros.b humble环境
source /opt/tros/humble/setup.bash

# 启动运行脚本
if [ -L qat ]; then rm qat; fi
ln -s `ros2 pkg prefix hobot_centerpoint`/lib/hobot_centerpoint/qat/ qat
ln -s ~/centerpoint_data centerpoint_data

# 启动launch文件
ros2 launch hobot_centerpoint hobot_centerpoint.launch.py
```

启动成功后，打开同一网络电脑的浏览器，访问RDK的IP地址`http://IP:8000`（IP为RDK的IP地址），即可看到算法可视化的实时效果：

![centerpoint](img/centerpoint.gif)


# 接口说明

## 话题

| 名称         | 消息类型                             | 说明                                     |
| ------------ | ------------------------------------ | ---------------------------------------- |
| /image_jpeg  | sensor_msgs/msg/Image                | 周期发布的图像话题，jpeg格式             |

## 参数

| 名称                         | 参数值                                          | 说明                                               |
| ---------------------------- | ----------------------------------------------- | -------------------------------------------------- |
| save_image               | "True"/"False", 默认为"False" | 将渲染后的图像保存到"./render"路径                    |

# 常见问题

通过设置运行时配置文件中的参数，用户可以修改回灌流程和感知结果的输出。

1. 获取运行时配置文件路径。

  在RDK上，使用如下命令查询运行时配置文件路径为：

  ```shell
    source /opt/tros/humble/setup.bash
    ls `ros2 pkg prefix hobot_centerpoint`/lib/hobot_centerpoint/config/centerpoint_pointpillar_nuscenes/workflow_latency.json
  ```

2. 控制回灌速度。

  `time_diff_ms表示每次回灌的间隔时间，单位为毫秒。默认为200毫秒，即每200毫秒回灌一次数据。

  ```json
    "time_diff_ms": 200
  ```