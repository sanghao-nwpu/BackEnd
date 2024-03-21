add_library(${PROJECT_NAME}_backend
        vertex.cpp
        edge.cpp
        problem.cpp
        )
target_link_libraries(${PROJECT_NAME}_backend ${GLOG_LIBRARIES})