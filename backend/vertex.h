#ifndef _BACKEND_VERTEX_H_
#define _BACKEND_VERTEX_H_

#include <backend/eigen_types.h>

namespace vio {
namespace backend {

/**
 * @brief: 顶点，对应一个parameter block，注意是个向量
 * 变量值以VecX存储，需要在构造时指定维度
 */
class Vertex
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
    /**
     * 构造函数
     * @param num_dimension 顶点自身维度
     * @param local_dimension 本地参数化维度，为-1时认为与本身维度一样
     */
    explicit Vertex(int num_dimension, int local_dimension = -1);

    int Dimension() const { return parameters_.rows(); }
    
    int LocalDimension() const { return local_dimension_; }

    unsigned long Id() const { return id_; }
    
    void SetOrderingId(const unsigned long ordering_id) { ordering_id_ = ordering_id; }
    unsigned long OrderingId() const { return ordering_id_; }
    
    void SetParameters(const VecX& parameters) { parameters_ = parameters; }
    VecX Parameters() const { return parameters_; }
    VecX &Parameters() { return parameters_; }
    
    void SetFixed(bool fixed = true) { fixed_ = fixed; } 
    bool IsFixed() const { return fixed_; }


    // virtual std::string TyeoInfo() const = 0;
    virtual std::string TypeInfo() const = 0;

    // virtual void Plus(const VecX& delta_x);
    void Plus(const VecX& delta_x);

    ~Vertex();

protected:
    VecX parameters_;   // 实际存储的变量值
    int local_dimension_;   // 局部参数化维度
    unsigned long id_;  // 顶点的id，自动生成

    /// ordering id是在problem中排序后的id，用于寻找雅可比对应块
    /// ordering id带有维度信息，例如ordering_id=6则对应Hessian中的第6列
    /// 从零开始
    unsigned long ordering_id_ = 0;

    bool fixed_ = false;    // 是否固定
};



}

}


#endif