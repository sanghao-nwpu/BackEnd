/*
 * @Author: moxiaosang_vec moxiaosang_vec@163.com
 * @Date: 2024-03-11 01:00:46
 * @LastEditors: moxiaosang_vec moxiaosang_vec@163.com
 * @LastEditTime: 2024-03-17 17:59:31
 * @FilePath: /BackEnd/backend/problem.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef _BACKEND_PROBLEM_H_
#define _BACKEND_PROBLEM_H_

#include <map>
#include <unordered_map>
#include <memory>

#include "backend/eigen_types.h"
#include "backend/vertex.h"
#include "backend/edge.h"

namespace vio {
namespace backend {

class Problem
{
public:
    enum class ProblemType {
        SLAM_PROBLEM, 
        GENERIC_PROBLEM
    };
    typedef std::map<unsigned long, std::shared_ptr<Vertex>> HashVertex;
    typedef std::unordered_map<unsigned long, std::shared_ptr<Edge>> HashEdge;
    typedef std::unordered_multimap<unsigned long, std::shared_ptr<Edge>> HashVertexIdToEdge;

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

    Problem(ProblemType problem_type);

    bool AddVertex(std::shared_ptr<Vertex> vertex);

    bool AddEdge(std::shared_ptr<Edge> edge);

    bool Solve(int iterations);

private:

    void MakeHessian();
   
    void UpdateStates();

    void RollBackStates();


    /* DATA */
    double current_chi2_;
    double stop_threshold_;
    ProblemType problem_type_;
    
    MatXX Hessian_;
    VecX  b_;
    VecX  delta_x_;
    // MatXX Hessian_prior_;
    // VecX  b_prior_;
    // VecX  error_prior_;

    HashVertex vertexes_;
    HashEdge   edges_;
    HashVertexIdToEdge vertex_id_to_edge_;
    
    unsigned long ordering_generic_ = 0;
    
};

} // namespace backend

} // namespace vio

#endif