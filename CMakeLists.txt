cmake_minimum_required(VERSION 3.20)
project(KaiEngine)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# 1. 自动搜索源文件 (现在会递归扫描 src/KaiEngine 和 src/Editor)
# ==============================================================================
file(GLOB_RECURSE SOURCES "src/*.cpp")

# ==============================================================================
# 2. 第三方库子目录 (确保路径与你实际存放位置一致)
# ==============================================================================
add_subdirectory(3rdparty/glad)
add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/glm)
add_subdirectory(3rdparty/spdlog)
add_subdirectory(3rdparty/stb_image)

# ==============================================================================
# 3. 创建可执行程序
# ==============================================================================
add_executable(KaiEngine ${SOURCES})

# ==============================================================================
# 4. 配置包含路径 (重构的核心：让编译器能找到 Core 和 Renderer)
# ==============================================================================
target_include_directories(KaiEngine PRIVATE 
    ${CMAKE_SOURCE_DIR}/src                     # 允许 #include "KaiEngine/Core/Window.h"
    ${CMAKE_SOURCE_DIR}/src/KaiEngine           # 允许 #include "Core/Window.h"
    ${CMAKE_SOURCE_DIR}/src/KaiEngine/Core
    ${CMAKE_SOURCE_DIR}/src/KaiEngine/Renderer
    ${CMAKE_SOURCE_DIR}/src/Editor   
    ${CMAKE_SOURCE_DIR}/src/KaiEngine/Camera           
    
    # 第三方库头文件
    ${CMAKE_SOURCE_DIR}/3rdparty/glad/include
    ${CMAKE_SOURCE_DIR}/3rdparty/glfw/include
    ${CMAKE_SOURCE_DIR}/3rdparty/glm
    ${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include
    ${CMAKE_SOURCE_DIR}/3rdparty/stb_image
)

# ==============================================================================
# 5. 链接库
# ==============================================================================
target_link_libraries(KaiEngine PRIVATE 
    glad 
    glfw 
    glm 
    spdlog 
    stb_image
    opengl32
)

# ==============================================================================
# 6. 宏定义 (如果是 Windows 平台)
# ==============================================================================
if(WIN32)
    target_compile_definitions(KaiEngine PRIVATE KAI_PLATFORM_WINDOWS)
endif()

# ==============================================================================
# 7. 资源拷贝 (防御性写法，确保生成时自动同步 assets 和 shaders)
# ==============================================================================
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/Debug")

# 只有当源文件夹存在时才尝试拷贝
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET KaiEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${OUTPUT_DIR}/assets"
        COMMENT "正在同步 assets 资源..."
    )
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/shaders")
    add_custom_command(TARGET KaiEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_DIR}/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "${OUTPUT_DIR}/shaders"
        COMMENT "正在同步 shaders 资源..."
    )
endif()