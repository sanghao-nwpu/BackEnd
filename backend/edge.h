/*
 * @Author: moxiaosang_vec moxiaosang_vec@163.com
 * @Date: 2024-03-10 15:11:06
 * @LastEditors: moxiaosang_vec moxiaosang_vec@163.com
 * @LastEditTime: 2024-03-17 17:57:39
 * @FilePath: /BackEnd/backend/edge.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _BACKEND_EDGE_H_
#define _BACKEND_EDGE_H_


#include <memory>
#include <iostream>

#include "backend/vertex.h"
#include "backend/eigen_types.h"

namespace vio {
namespace backend {

class Vertex;

/**
 * @description: 边，一个边对应一个雅克比矩阵，
 * @return {*}
 */
class Edge {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    /**
     * 构造函数，会自动化配雅可比的空间
     * @param residual_dimension 残差维度
     * @param num_vertexes 顶点数量，一个顶点对应一个雅克比。注意顶点是向量。
     * @param vertexes_types 顶点类型名称，可以不给，不给的话check中不会检查
     * @note 实际上这些数量均和定义区分有关，把顶点向量拆成一个个数量，那顶点数量就变成对应向量维度了，理解分块的思想
     */
    explicit Edge(int residual_dimention, int num_vertexes,
                  const std::vector<std::string> &vertexes_types = std::vector<std::string>());
    virtual ~Edge();

    unsigned long Id() const { return id_; }

    void AddVertex(std::shared_ptr<Vertex> &vertex) { vertexes_.emplace_back(vertex); }

    void SetVertexes(std::vector<std::shared_ptr<Vertex>>& vertexes) { vertexes_ = vertexes; }

    std::shared_ptr<Vertex> GetVertex(const unsigned long i) const { return vertexes_[i]; }

    std::vector<std::shared_ptr<Vertex>> GetVertexes() const { return vertexes_; }

    size_t NumVertexes() const { return vertexes_.size(); }

    bool CheckValid();

    double Chi2();

    VecX Residual() const { return residual_; }

    /* Question: Why vector */
    std::vector<MatXX> Jacobians() const { return jacobians_; }

    void SetInformation(const MatXX& information) { information_ = information; }

    MatXX Information() const { return information_; }

    void SetObservation(const VecX& observation) { observation_ = observation; }
    
    VecX Observation() const { return observation_; }

    int OrderingId() const { return ordering_id_; }

    void SetOrderingId(int id) { ordering_id_ = id; };

    std::vector<std::shared_ptr<Vertex>> Vertexes() { return vertexes_; }

    virtual std::string TypeInfo() const = 0;

    virtual void ComputeResidual() = 0;

    virtual void ComputeJacobians() = 0;

protected: 
    unsigned long id_;
    int ordering_id_;
    std::vector<std::string> vertexes_types_;        /* for debug */
    std::vector<std::shared_ptr<Vertex>> vertexes_; /* the edge related verteies */
    VecX residual_;                 /* 残差, Question: 为什么是Vector，一个边不是对应一个残差？多维观测也解释不通 */
    std::vector<MatXX> jacobians_;  /* 雅可比，每个雅可比维度是 residual x vertex[i] */
    MatXX information_;             /* */
    VecX observation_;
};

} // namespace backend 

} // namespace vio 


#endif