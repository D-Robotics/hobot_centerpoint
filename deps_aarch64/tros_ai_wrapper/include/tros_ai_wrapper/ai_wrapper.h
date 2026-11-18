// Copyright (c) 2024，D-Robotics.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>
#include <string>
#include <vector>
#include <thread>

struct FrameInfo {
  int frame_id;
  // us
  uint64_t infer_duration;
  uint64_t pp_duration;
  uint64_t pre_duration;
};

// SDK 扩展: 解码后的 3D 检测框 (CenterPoint 等 lidar 模型)
// x,y,z 为框底面中心; w,l,h 为尺寸; yaw 绕 z; label 为类别 id
struct DetBox3D {
  float x, y, z;
  float w, l, h;
  float yaw;
  float score;
  int label;
};

class AIWrapper {
public:
  AIWrapper();

  ~AIWrapper();

  bool is_running_ = false;

  void Init(int argc, char **argv);
  void Start();
  void Stop();

  // 扩展版回调: imgs 为渲染图, boxes 为结构化检测结果
  void SetOutputCallback(
    std::function<void(const std::vector<cv::Mat>& imgs,
                       std::shared_ptr<FrameInfo> frame,
                       const std::vector<DetBox3D>& boxes)> callback);
};