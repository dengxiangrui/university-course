// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>


rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

auto to_vec4(const Eigen::Vector3f& v, float w = 1.0f)
{
    return Vector4f(v.x(), v.y(), v.z(), w);
}


// 使用重心坐标法判断点 (x, y) 是否在三角形内
// 调用已有的 computeBarycentric2D 函数计算重心坐标
// 若三个坐标均 >= 0（或均 <= 0），则点在三角形内
static bool insideTriangle(int x, int y, const Vector3f* _v)
{
    float c1, c2, c3;
    std::tie(c1, c2, c3) = computeBarycentric2D(x, y, _v);
    // 支持 CCW 和 CW 两种 winding order
    return (c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0);
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec /= vec.w();
        }
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);

        rasterize_triangle(t);
    }

    // ===== 超采样抗锯齿：将样本缓冲区合成到帧缓冲区 =====
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int idx = get_index(x, y);
            Eigen::Vector3f avg_color(0, 0, 0);
            float min_depth = std::numeric_limits<float>::infinity();

            for (int sy = 0; sy < 2; sy++)
            {
                for (int sx = 0; sx < 2; sx++)
                {
                    int sample_idx = idx * 4 + sy * 2 + sx;
                    avg_color += sample_buf[sample_idx].color;
                    min_depth = std::min(min_depth, sample_buf[sample_idx].depth);
                }
            }
            // 4 个样本取平均
            frame_buf[idx] = avg_color / 4.0f;
            depth_buf[idx] = min_depth;
        }
    }
}

// 屏幕空间三角形栅格化（含 2x2 超采样抗锯齿）
void rst::rasterizer::rasterize_triangle(const Triangle& t) {
    auto v = t.toVector4();

    // 计算三角形的屏幕空间包围盒
    float min_x = std::max(0.0f, std::min({v[0].x(), v[1].x(), v[2].x()}));
    float min_y = std::max(0.0f, std::min({v[0].y(), v[1].y(), v[2].y()}));
    float max_x = std::min((float)(width - 1), std::max({v[0].x(), v[1].x(), v[2].x()}));
    float max_y = std::min((float)(height - 1), std::max({v[0].y(), v[1].y(), v[2].y()}));

    // 2x2 超采样：每个像素 4 个子样本，偏移量取子像素中心 (0.25, 0.75)
    float offsets[2] = {0.25f, 0.75f};

    for (int y = (int)min_y; y <= (int)max_y; y++)
    {
        for (int x = (int)min_x; x <= (int)max_x; x++)
        {
            int idx = get_index(x, y);

            // 遍历 4 个子样本
            for (int sy = 0; sy < 2; sy++)
            {
                for (int sx = 0; sx < 2; sx++)
                {
                    // 样本在屏幕空间的位置
                    float sample_x = x + offsets[sx];
                    float sample_y = y + offsets[sy];

                    int sample_idx = idx * 4 + sy * 2 + sx;

                    // 判断样本点是否在三角形内
                    if (insideTriangle((int)sample_x, (int)sample_y, t.v))
                    {
                        // 计算重心坐标
                        auto[alpha, beta, gamma] = computeBarycentric2D(sample_x, sample_y, t.v);
                        // 透视正确的深度插值
                        float w_reciprocal = 1.0f / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                        float z_interpolated = alpha * v[0].z() / v[0].w()
                                             + beta * v[1].z() / v[1].w()
                                             + gamma * v[2].z() / v[2].w();
                        z_interpolated *= w_reciprocal;

                        // 每个样本独立深度测试
                        if (z_interpolated < sample_buf[sample_idx].depth)
                        {
                            sample_buf[sample_idx].depth = z_interpolated;
                            sample_buf[sample_idx].color = t.getColor();
                        }
                    }
                    // 样本在三角形外：保持背景色 (0,0,0) 和深度 infinity，不做任何操作
                }
            }
        }
    }
}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
        // 同时清除样本缓冲区的颜色
        for (auto& samples : sample_buf)
        {
            for (auto& s : samples)
            {
                s.color = Eigen::Vector3f{0, 0, 0};
            }
        }
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
        // 同时清除样本缓冲区的深度
        for (auto& samples : sample_buf)
        {
            for (auto& s : samples)
            {
                s.depth = std::numeric_limits<float>::infinity();
            }
        }
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);
    // 初始化超采样缓冲区
    sample_buf.resize(w * h);
}

int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;

}

// clang-format on
