/*
 * @Author: moxiaosang_vec moxiaosang_vec@163.com
 * @Date: 2024-03-10 14:50:25
 * @LastEditors: moxiaosang_vec moxiaosang_vec@163.com
 * @LastEditTime: 2024-03-11 00:55:54
 * @FilePath: /BackEnd/backend/vertex.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "backend/vertex.h"
// #include "glog/logging.h"
#include <iostream>

namespace vio {
namespace backend {

unsigned long global_vertex_id = 0;

Vertex::Vertex(int num_dimension, int local_dimension) {
    parameters_.resize(num_dimension, 1);
    local_dimension_ = local_dimension > 0 ? local_dimension : num_dimension;
    id_ = global_vertex_id++;
    // LOG(info)
    std::cout << "Vertex construct num_dimension: " << num_dimension
              << " local_dimension: " << local_dimension << " id_: " << id_ << std::endl;
}

Vertex::~Vertex() {}

void Vertex::Plus(const VecX &delta_x)
{
    parameters_ += delta_x;
}


}

}