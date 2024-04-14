
add_executable(testCurveFitting curve_fitting.cpp)
target_link_libraries(testCurveFitting ${PROJECT_NAME}_backend)
target_link_libraries(testCurveFitting ${GLOG_LIBRARIES})


add_executable(testCurveFittingByCeres curve_fitting_by_ceres.cpp)
target_link_libraries(testCurveFittingByCeres ${PROJECT_NAME}_backend)
target_link_libraries(testCurveFittingByCeres ${CERES_LIBRARIES})
target_link_libraries(testCurveFittingByCeres ${GLOG_LIBRARIES})
