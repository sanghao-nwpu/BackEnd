/*
 * @Author: moxiaosang_vec moxiaosang_vec@163.com
 * @Date: 2024-04-15 00:09:01
 * @LastEditors: moxiaosang_vec moxiaosang_vec@163.com
 * @LastEditTime: 2024-04-15 00:13:24
 * @FilePath: /BackEnd/app/curve_fitting_by_ceres.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <ceres/ceres.h>
#include <cmath>

// 二次曲线模型的残差计算
struct QuadraticCostFunction {
    QuadraticCostFunction(double x, double y)
        : x_(x), y_(y) {}

    template <typename T>
    bool operator()(const T* const a, const T* const b, const T* const c, T* residual) const {
        residual[0] = y_ - (a[0] * x_ * x_ + b[0] * x_ + c[0]);
        return true;
    }

private:
    const double x_;
    const double y_;
};

int main() {
    // 实际参数值
    double a_true = 0.3;
    double b_true = -0.8;
    double c_true = 0.5;

    // 生成带噪声的二次曲线数据
    std::vector<double> x_data, y_data;
    for (int i = 0; i < 100; ++i) {
        double x = i / 100.0;
        double y = a_true * x * x + b_true * x + c_true + 0.01 * (rand() / double(RAND_MAX) - 0.5);
        x_data.push_back(x);
        y_data.push_back(y);
    }

    // 初始化 Ceres Solver
    ceres::Problem problem;
    for (int i = 0; i < x_data.size(); ++i) {
        ceres::CostFunction* cost_function =
            new ceres::AutoDiffCostFunction<QuadraticCostFunction, 1, 1, 1, 1>(
                new QuadraticCostFunction(x_data[i], y_data[i])
            );
        problem.AddResidualBlock(cost_function, nullptr, &a_true, &b_true, &c_true);
    }

    // 配置 Solver 选项
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;

    // 运行 Solver
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    // 输出最终的参数估计值
    std::cout << "Final a: " << a_true << ", b: " << b_true << ", c: " << c_true << std::endl;
    std::cout << "Final cost: " << summary.final_cost << std::endl;

    return 0;
}
