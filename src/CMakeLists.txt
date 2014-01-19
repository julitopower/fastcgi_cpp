message(STATUS ${FCGI_LIBRARIES})
add_executable(app main.cpp)
target_link_libraries(app ${FCGI_LIBRARIES})
