// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <cstdlib>


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

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}


static bool insideTriangle(float x, float y, const Vector3f* _v) {
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    Vector3f p_point((float)x+0.5,(float)y+0.5, 0);
    Vector3f a_point(_v[0].x(),_v[0].y(), 0);
    Vector3f b_point(_v[1].x(),_v[1].y(), 0);
    Vector3f c_point(_v[2].x(),_v[2].y(), 0);
    Vector3f ap_vector=p_point - a_point;
    Vector3f bp_vector=p_point - b_point;
    Vector3f cp_vector=p_point - c_point;
    Vector3f ab_vector=b_point - a_point;
    Vector3f bc_vector=c_point - b_point;
    Vector3f ca_vector=a_point - c_point;
    Vector3f pab = ap_vector.cross(ab_vector);
    Vector3f pbc = bp_vector.cross(bc_vector);
    Vector3f pca = cp_vector.cross(ca_vector);

    return pab.dot(pbc)>0&&pbc.dot(pca)>0&&pca.dot(pab)>0;

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
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t) {
    auto v = t.toVector4();
    bool MSAA = true;
    // TODO : Find out the bounding box of current triangle.
    // iterate through the pixel and find if the current pixel is inside the triangle
    float x_min = std::min(v.at(0).x(), std::min(v.at(1).x(),v.at(2).x()));
    float y_min = std::min(v.at(0).y(), std::min(v.at(1).y(),v.at(2).y()));
    float x_max = std::max(v.at(0).x(), std::max(v.at(1).x(),v.at(2).x()));
    float y_max = std::max(v.at(0).y(), std::max(v.at(1).y(),v.at(2).y()));

    x_min = static_cast<int>(std::floor(x_min));
    y_min = static_cast<int>(std::floor(y_min));
    x_max = static_cast<int>(std::ceil(x_max));
    y_max = static_cast<int>(std::ceil(y_max));
    // If so, use the following code to get the interpolated z value.
    //auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
    //float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
    //float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
    //z_interpolated *= w_reciprocal;

    // TODO : set the current pixel (use the set_pixel function) to the color of the triangle (use getColor function) if it should be painted.
    if(MSAA)
    {
        for(int i= x_min; i <= x_max;++i)
        {
            for(int j= y_min; j <= y_max; ++j)
            {

                // if use x = i+0.5, the picture will be split from middle in y axis
                float x = i, y=j;
                // std::cout << x << "   " << y <<std::endl;
                float super_sample_result = 0;
                std::vector<std::pair<float,float>> super_sample_points(4);
                super_sample_points[0] = std::pair<float,float>(x + 0.25,y + 0.25);
                super_sample_points[1] = std::pair<float,float>(x + 0.25,y + 0.75);
                super_sample_points[2] = std::pair<float,float>(x + 0.75,y + 0.25);
                super_sample_points[3] = std::pair<float,float>(x + 0.75,y + 0.75);
                for(int k=0;k<4;k++)
                {
                    // std::cout << "super_sample" << std::endl;
                    //std::cout << super_sample_points[k].first << "   " << super_sample_points[k].second <<std::endl;
                    if(insideTriangle(super_sample_points[k].first,super_sample_points[k].second,t.v))
                    {
                        super_sample_result++;
                    }
                }
                               // std::cout << super_sample_result << std::endl;
                if(super_sample_result>0)
                {
                    auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
                    float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                    float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                    z_interpolated *= w_reciprocal;

                    auto ind = get_index(x,y);
                    if(depth_buf[ind] > z_interpolated)
                    {
                        depth_buf[ind] = z_interpolated;
                        // std::cout << super_sample_result << std::endl;
                        set_pixel(Vector3f(x, y, 1),super_sample_result/4.0*t.getColor());
                    }
                }
            }
        }
    }
    else
    {
        for(auto i = x_min; i <= x_max; i++)
        {
            for(auto j = y_min; j<=y_max;j++)
            {
                int x = i + 0.5, y = j + 0.5;
                // std::cout << x << "   "<< y <<std::endl;
                if(insideTriangle(x,y,t.v))
                {
                    auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
                    float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                    float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                    z_interpolated *= w_reciprocal;

                    auto ind = get_index(x,y);
                    if(depth_buf[ind] > z_interpolated)
                    {
                        depth_buf[ind] = z_interpolated;
                        set_pixel(Vector3f(x,y,1),t.getColor());
                    }
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
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);
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