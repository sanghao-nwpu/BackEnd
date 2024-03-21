/*
 * @Author: moxiaosang_vec moxiaosang_vec@163.com
 * @Date: 2024-03-12 01:20:53
 * @LastEditors: moxiaosang_vec moxiaosang_vec@163.com
 * @LastEditTime: 2024-03-21 22:24:41
 * @FilePath: /BackEnd/backend/problem.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <glog/logging.h>
#include <iostream>
#include "backend/problem.h"


namespace vio {
namespace backend {

Problem::Problem(ProblemType problem_type)
    : problem_type_(problem_type) { }

/* TODO: Question: Why cannot refrence */
bool Problem::AddVertex(std::shared_ptr<Vertex> vertex)
{
    if (vertexes_.find(vertex->Id()) != vertexes_.end())
    {
        std::cout << "this vertex has been added!\n";
        return false;
    }
    vertexes_.insert(std::pair<unsigned long, std::shared_ptr<Vertex>>(vertex->Id(), vertex));
    return true;
}

bool Problem::AddEdge(std::shared_ptr<Edge> edge)
{
    if (edges_.find(edge->Id()) != edges_.end())
    {
        std::cout << "this edge has been added!\n";
        return false;
    }
    edges_.insert(std::pair<unsigned long, std::shared_ptr<Edge>>(edge->Id(), edge));
    for (auto vertex : edge->Vertexes())
    {
        /* 一个边的约束可能和多个点相关 */
        vertex_id_to_edge_.insert(std::pair<unsigned long, std::shared_ptr<Edge>>(vertex->Id(), edge));
    }
    
    return true;
}



void Problem::MakeHessian()
{
    unsigned long size = 0;

    /* 1. 得到H的维度，并创建 */
    size = ordering_generic_;

    MatXX H(MatXX::Zero(size, size));
    VecX b(VecX::Zero(size));

    /* 2. 遍历每个边，把每个边的雅克比写到H中去 */
    /* Question: Why refrence */
    for (auto &edge : edges_)
    {
        edge.second->ComputeResidual();
        edge.second->ComputeJacobians();
        auto jacobians = edge.second->Jacobians();
        auto vertexes = edge.second->Vertexes();

        for (size_t i = 0; i < vertexes.size(); i++)
        {
            auto v_i = vertexes[i];
            if (v_i->IsFixed())
                continue;
            auto jacobian_i = jacobians[i];
            unsigned long index_i = v_i->OrderingId();
            unsigned long dim_i = v_i->LocalDimension();
            MatXX JtW = jacobian_i.transpose() * edge.second->Information();
            for (size_t j = i; j < vertexes.size(); j++)
            {
                auto v_j = vertexes[j];
                if (v_j->IsFixed())
                    continue;
                auto jacobian_j = jacobians[j];
                unsigned long index_j = v_j->OrderingId();
                unsigned long dim_j = v_j->LocalDimension();
                MatXX hessian = JtW * jacobian_j;

                /* 添加到整体的H中去 */
                /* noalias指不用创建一个临时变量存结果, 这里不能用block<>是因为这是一个动态矩阵 */
                H.block(index_i, index_j, dim_i, dim_j).noalias() += hessian;
                if (j != i)
                {
                    H.block(index_j, index_i, dim_j, dim_i).noalias() += hessian.transpose();
                }
            }
            b.segment(index_i, dim_i).noalias() -= JtW * edge.second->Residual();
        }
    }
    Hessian_ = H;
    b_ = b;
    delta_x_ = VecX::Zero(size);
}

bool Problem::Solve(int iterations)
{
    size_t iter = 0;
    bool stop = false;
    /* 1. count dimension */
    ordering_generic_ = 0;
    for (auto vertex:vertexes_)
    {
        ordering_generic_ += vertex.second->LocalDimension();
    }
    
    /* 2. make hessian */
    MakeHessian();

    /* 3. init */
    current_chi2_ = 0;
    for (auto edge : edges_)
    {
        current_chi2_ += edge.second->Chi2();
    }
    
    while (!stop && iter < (size_t)iterations)
    {
        std::cout << "iter: " << iter << "; current chi2: " << current_chi2_ << "\n";
        LOG(INFO) << "iter: " << iter << "; current chi2: " << current_chi2_;
        LOG(INFO) << "delta_x: " << delta_x_.transpose();
        // std::cout << "delta_x: " << delta_x_.transpose() << "\n";
        // LOG(INFO) << "delta_x: " << delta_x_.transpose();    
        // bool OneStepSuccess = false;
        // size_t false_cnt = 0;
        delta_x_ = Hessian_.inverse() * b_;
        if (delta_x_.squaredNorm() < 1e-4)
        {
            stop = true;
        }

        /* update chi2 */
        current_chi2_ = 0;
        for (auto edge : edges_)
        {
            edge.second->ComputeResidual();
            current_chi2_ += edge.second->Chi2();
        }

        /* when caculate delta_x_, update vertex */
        for (auto vertex : vertexes_)
        {
            unsigned long id = vertex.second->OrderingId();
            unsigned long dim = vertex.second->LocalDimension();
            VecX delta = delta_x_.segment(id, dim);
            vertex.second->Plus(delta);
        }
        iter++;
        MakeHessian();
    }
    return true;
}

} // namespace backend
} // namespace vio




