add_executable(helloworld32 
    helloworld.cpp
)
target_link_libraries(helloworld32 PRIVATE project_options)
set_target_properties(helloworld32 PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")


add_executable(helloworld64 
    helloworld.cpp
)
target_link_libraries(helloworld64 PRIVATE project_options)
set_target_properties(helloworld64 PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")