add_library(hello32 SHARED
    lib.cpp
)
target_link_libraries(hello32 PRIVATE project_options)
set_target_properties(hello32 PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")


add_library(hello64 SHARED
    lib.cpp
)
target_link_libraries(hello64 PRIVATE project_options)
set_target_properties(hello64 PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")