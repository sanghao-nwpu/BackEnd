/*
 * @Author: moxiaosang_vec moxiaosang_vec@163.com
 * @Date: 2024-03-10 23:54:32
 * @LastEditors: moxiaosang_vec moxiaosang_vec@163.com
 * @LastEditTime: 2024-03-17 17:58:06
 * @FilePath: /BackEnd/backend/edge.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "backend/edge.h"

namespace vio {
namespace backend {

unsigned long global_edge_id = 0;

Edge::Edge(int residual_dimention, int num_vertexes,
           const std::vector<std::string> &vertexes_types)
{
    residual_.resize(residual_dimention, 1);
    if (!vertexes_types.empty())
    {
        vertexes_types_ = vertexes_types;
    }

    jacobians_.resize(num_vertexes);    /* Why, I think should be residual_dimention */
    id_ = global_edge_id++;
    MatXX information(residual_dimention, residual_dimention);
    information.setIdentity();
    SetInformation(information);
}

Edge::~Edge() { }

double Edge::Chi2()
{
    return residual_.transpose() * information_ * residual_;
}

bool Edge::CheckValid()
{
    if (!vertexes_types_.empty())
    {
        // check type info
        for (size_t i = 0; i < vertexes_.size(); ++i)
        {
            if (vertexes_types_[i] != vertexes_[i]->TypeInfo())
            {
                std::cout << "Vertex type does not match, should be "
                          << vertexes_types_[i] << ", but set to "
                          << vertexes_[i]->TypeInfo() << std::endl;
                return false;
            }
        }
    }
    return true;
}

} // namespace backend

} // namespace vio 