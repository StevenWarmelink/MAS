add_library(Agents SHARED
            Agent.cpp ActiveAgent.cpp)
target_link_libraries(Agents Misc)
