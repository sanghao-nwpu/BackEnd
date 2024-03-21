
add_executable(testCurveFitting curve_fitting.cpp)
target_link_libraries(testCurveFitting ${PROJECT_NAME}_backend)
target_link_libraries(testCurveFitting ${GLOG_LIBRARIES})