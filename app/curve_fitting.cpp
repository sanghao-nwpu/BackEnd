/*
 * @Author: moxiaosang_vec moxiaosang_vec@163.com
 * @Date: 2024-03-16 19:18:25
 * @LastEditors: moxiaosang_vec moxiaosang_vec@163.com
 * @LastEditTime: 2024-03-21 22:35:39
 * @FilePath: /BackEnd/app/curve_fitting.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <iostream>
#include <random>
#include <memory>
#include <glog/logging.h>

#include "backend/backend.h"

class CurveFitVertex : public vio::backend::Vertex
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    CurveFitVertex() : Vertex(3) {};    /* abc 三个参数 */
    virtual std::string TypeInfo() const { return "abc"; }
};

class CurveFitEdge : public vio::backend::Edge
{
public: 
    double x_, y_;  /* state, and the obs */
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    CurveFitEdge(double x, double y) : Edge(1, 1, std::vector<std::string>{"abc"})
    {
        x_ = x;
        y_ = y;
    }

    virtual void ComputeResidual() override
    {
        /* because only 1 vertex param block */
        Vec3 abc = vertexes_[0]->Parameters();
        residual_(0) = std::exp(abc(0) * x_ * x_ + abc(1) * x_ + abc(2)) - y_;
    }

    virtual void ComputeJacobians() override 
    {
        /* because only 1 vertex param block */
        Vec3 abc = vertexes_[0]->Parameters();
        MatXX jacobians = MatXX::Zero(1, 3);
        jacobians(0, 0) = x_ * x_ * std::exp(abc(0) * x_ * x_ + abc(1) * x_ + abc(2));
        jacobians(0, 1) = x_ * std::exp(abc(0) * x_ * x_ + abc(1) * x_ + abc(2));
        jacobians(0, 2) = std::exp(abc(0) * x_ * x_ + abc(1) * x_ + abc(2));
        jacobians_[0] = jacobians;
    }

    virtual std::string TypeInfo() const { return "CruveFitEdge"; }
};


int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = false;
    FLAGS_log_dir = "./";
    FLAGS_alsologtostderr = false;
    FLAGS_colorlogtostderr = true;
    
    double a = 1.0, b = 2.0, c = 3.0;
    double a_init = 0, b_init = 0, c_init = 9;
    double sigma = 0.1;

    std::default_random_engine generator;
    std::normal_distribution<double> noise(0., sigma);

    std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
    std::cout << "sigma = " << sigma << std::endl;

    /* create problem */
    vio::backend::Problem problem(vio::backend::Problem::ProblemType::GENERIC_PROBLEM);
    // std::shared_ptr<CurveFitVertex> vertex_ptr = std::make_shared<CurveFitVertex>();
    std::shared_ptr<CurveFitVertex> vertex_ptr(new CurveFitVertex());
    /* set vertex param */

    vertex_ptr->SetParameters(Vec3(a_init, b_init, c_init));
    problem.AddVertex(vertex_ptr);

    for (int i = 0; i < 100; i++)
    {
        double x = i / 100.0;
        double n = noise(generator);
        double y = std::exp(a * x * x + b * x + c) + n;

        /* add edge */
        // std::shared_ptr<CurveFitEdge> edge_ptr = std::make_shared<CurveFitEdge>(x, y);
        std::shared_ptr<CurveFitEdge> edge_ptr(new CurveFitEdge(x, y));
        std::vector<std::shared_ptr<vio::backend::Vertex>> vertex_ptrs;
        vertex_ptrs.push_back(vertex_ptr);
        edge_ptr->SetVertexes(vertex_ptrs);
        problem.AddEdge(edge_ptr);
    }

    problem.Solve(50);
    std::cout << "after optimization: " << std::endl;
    std::cout << "a = " << vertex_ptr->Parameters()(0)
              << ", b = " << vertex_ptr->Parameters()(1)
              << ", c = " << vertex_ptr->Parameters()(2) << std::endl;
    google::ShutdownGoogleLogging();
    return 0;
}
