target_sources(${APP_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainComponent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Live2DViewComponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Live2DViewComponent.h
)

add_subdirectory(LAppBase)
