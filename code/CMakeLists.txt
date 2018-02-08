add_subdirectory(Misc)
add_subdirectory(Agents)
add_subdirectory(Simulation)

add_executable(main main.cpp)
target_link_libraries(main Simulation)
