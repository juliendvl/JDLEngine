# -----------------------------------------------------------------------------
# JDLEngine - Sandbox Application CMakeLists
# -----------------------------------------------------------------------------
set(APP_NAME sandbox)

add_executable(${APP_NAME} Main.cpp)

include_directories(${ENGINE_DIR}/include)
target_link_libraries(${APP_NAME} ${ENGINE_NAME})
