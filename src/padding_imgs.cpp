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

#include <fstream>
#include <rclcpp/rclcpp.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

int read_binary_file(std::string &file_path, char **bin, int *length) {
  std::ifstream ifs(file_path.c_str(), std::ios::in | std::ios::binary);
  if (!ifs) {
    RCLCPP_ERROR(rclcpp::get_logger("padding_imgs"),
        "Open %s failed", file_path.data());
    return -1;
  }
  ifs.seekg(0, std::ios::end);
  *length = ifs.tellg();
  ifs.seekg(0, std::ios::beg);
  *bin = new char[sizeof(char) * (*length)];
  ifs.read(*bin, *length);
  ifs.close();
  return 0;
}


int main(int argc, char **argv) {
  if (argc < 4) {
    RCLCPP_ERROR(rclcpp::get_logger("padding_imgs"), "Please int imgs path, list, padding ratio");
    return -1;
  }

  rclcpp::init(argc, argv);

  std::string path = argv[1];
  std::string fname_list = argv[2];
  float padding_ratio = std::stof(argv[3]);
  int mode_cvt = 0;
  if (argc == 5) {
    mode_cvt = std::stoi(argv[4]);
  }

  RCLCPP_INFO(rclcpp::get_logger("padding_imgs"),
    "path: %s, fname_list: %s, padding_ratio: %.2f, mode_cvt: %d",
    path.c_str(), fname_list.c_str(), padding_ratio, mode_cvt);

  if (access(path.data(), 0) != 0) {
    RCLCPP_ERROR(rclcpp::get_logger("padding_imgs"),
      "path %s not exist", path.c_str());
    return -1;
  }

  std::vector<std::string> img_lists;
  std::ifstream fin(fname_list);
  if (!fin) {
    RCLCPP_ERROR(rclcpp::get_logger("padding_imgs"),
      "fname_list file %s not exist", fname_list.c_str());
    return -1;
  }

  std::string image_path;
  while (std::getline(fin, image_path)) {
    img_lists.push_back(image_path);
  }
  RCLCPP_INFO(rclcpp::get_logger("padding_imgs"),
    "img_lists size: %ld", img_lists.size());

  // 0.png
  int img_frameid = 0;
  std::string img_prefix = path + "/render_";
  while (rclcpp::ok()) {
    std::string img_name = img_prefix + std::to_string(img_frameid) + ".jpeg";
    cv::Mat mat1 = cv::imread(img_name);
    if (mat1.empty()) {
      RCLCPP_ERROR(rclcpp::get_logger("padding_imgs"),
        "image file %s not exist", img_name.c_str());
      return -1;
    }

    if (img_frameid >= img_lists.size()) {
      RCLCPP_INFO(rclcpp::get_logger("padding_imgs"),
        "img_frameid: %d, img_lists size: %ld", img_frameid, img_lists.size());
    }
    // cv::Mat mat2 = cv::imread(img_lists.at(img_frameid));


    std::string image_path = img_lists.at(img_frameid);
    auto height = 512;
    auto stride = 960;

    int32_t data_length = 0;
    char *data_buffer = nullptr;
    auto ret = read_binary_file(image_path, &data_buffer, &data_length);

    cv::Mat out_image;
    out_image.create(height * 3 / 2, stride, CV_8UC1);
    auto y_size = height * stride;
    auto uv_size = y_size / 2;

    auto y_addr =
        reinterpret_cast<uint8_t *>(data_buffer);
    auto uv_addr = y_addr + y_size;

    auto dst_addr = out_image.data;
    memcpy(dst_addr, y_addr, y_size);
    memcpy(dst_addr + y_size, uv_addr, uv_size);

    cv::Mat mat2;
    cv::cvtColor(out_image, mat2, CV_YUV2BGR_NV12);
    if (mat2.empty()) {
      RCLCPP_ERROR(rclcpp::get_logger("padding_imgs"),
        "image file %s not exist", img_lists.at(img_frameid).c_str());
      return -1;
    }

    if (mode_cvt) {
      static std::string out_path = "./convert";
      if (!out_path.empty()) {
        static int count = 0;
        if (access(out_path.c_str(), 0) != 0) {
          RCLCPP_WARN(rclcpp::get_logger("padding_imgs"),
            "mkdir %s", out_path.c_str());
          mkdir(out_path.c_str(), 0777);
        }
        std::string saving_path = out_path + "/render_" + std::to_string(count++) + ".jpeg";
        RCLCPP_WARN(rclcpp::get_logger("padding_imgs"),
          "Save result to file: %s", saving_path.c_str());
        cv::imwrite(saving_path, mat2);
      }
    } else {
      static std::string out_path = "./render_padding";
      float resize_ratio = std::min(mat1.cols * padding_ratio  / mat2.cols, mat1.rows * padding_ratio  / mat2.rows);
      cv::resize(mat2, mat2,
        cv::Size(mat2.cols * resize_ratio, mat2.rows * resize_ratio));
      RCLCPP_INFO(rclcpp::get_logger("padding_imgs"),
        "resize_ratio: %.2f, mat1: %d, %d, mat2: %d, %d, padding to %d, %d, %d, %d",
        resize_ratio,
        mat1.cols, mat1.rows,
        mat2.cols, mat2.rows,
        mat1.cols - mat2.cols, 0, mat2.cols, mat2.rows
      );
      mat2.copyTo(mat1(cv::Rect(mat1.cols - mat2.cols, 0, mat2.cols, mat2.rows)));
  
      if (!out_path.empty()) {
        static int count = 0;
        if (access(out_path.c_str(), 0) != 0) {
          RCLCPP_WARN(rclcpp::get_logger("padding_imgs"),
            "mkdir %s", out_path.c_str());
          mkdir(out_path.c_str(), 0777);
        }
        std::string saving_path = out_path + "/render_" + std::to_string(count++) + ".jpeg";
        RCLCPP_WARN(rclcpp::get_logger("padding_imgs"),
          "Save result to file: %s", saving_path.c_str());
        cv::imwrite(saving_path, mat1);
      }
    }
    img_frameid++;
  }

  rclcpp::shutdown();

  return 0;
}
