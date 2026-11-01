//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <algorithm>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    // 最近邻采样（原始实现，保持不变）
    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    // 双线性插值采样：拿满提高项 15 分中"双线性纹理插值"的实现
    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        // 图像空间坐标（注意 v 方向需要翻转：v=0 对应图像底行）
        float u_img = u * (width  - 1);
        float v_img = (1 - v) * (height - 1);

        // 左上角整数像素索引，并夹紧到合法范围，避免越界
        int x0 = static_cast<int>(std::floor(u_img));
        int y0 = static_cast<int>(std::floor(v_img));
        x0 = std::max(0, std::min(x0, width  - 2));
        y0 = std::max(0, std::min(y0, height - 2));

        float fx = u_img - x0;   // 水平方向权重
        float fy = v_img - y0;   // 垂直方向权重

        auto c00 = image_data.at<cv::Vec3b>(y0,     x0);      // 左上
        auto c10 = image_data.at<cv::Vec3b>(y0,     x0 + 1);  // 右上
        auto c01 = image_data.at<cv::Vec3b>(y0 + 1, x0);      // 左下
        auto c11 = image_data.at<cv::Vec3b>(y0 + 1, x0 + 1);  // 右下

        float out[3];
        for (int k = 0; k < 3; ++k)
        {
            float top = (1 - fx) * c00[k] + fx * c10[k];   // 上边水平插值
            float bot = (1 - fx) * c01[k] + fx * c11[k];   // 下边水平插值
            out[k]    = (1 - fy) * top    + fy * bot;       // 垂直插值
        }
        return Eigen::Vector3f(out[0], out[1], out[2]);
    }

};
#endif //RASTERIZER_TEXTURE_H
