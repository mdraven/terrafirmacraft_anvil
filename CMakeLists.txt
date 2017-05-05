cmake_minimum_required(VERSION 3.5)

project(tfc_anvil)

#### Check --------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/3rdparty")

set(FLTK_SKIP_OPENGL true)
set(FLTK_SKIP_FORMS true)
set(FLTK_SKIP_FLUID true)
find_package(FLTK REQUIRED)
include_directories(${FLTK_INCLUDE_DIR})

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

#### Configure ----------------------------

function(add_sanitize TARGET)
  target_compile_options(${TARGET} PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address>
    $<$<CONFIG:Debug>:-fsanitize=leak>
    $<$<CONFIG:Debug>:-fsanitize=undefined>
    $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
    )
  target_link_libraries(${TARGET}
    $<$<CONFIG:Debug>:-fsanitize=address>
    $<$<CONFIG:Debug>:-fsanitize=undefined>
    )
endfunction()

set(SOURCE_DIR "src")

include_directories(${SOURCE_DIR})

set(EXEC_GENERATOR "anvil_chains_generator")

set(SOURCES_GENERATOR
  ${SOURCE_DIR}/Technique.cpp
  ${SOURCE_DIR}/anvil_chains_generator.cpp
  ${SOURCE_DIR}/increment.cpp
  )

add_executable(${EXEC_GENERATOR} ${SOURCES_GENERATOR})
set_property(TARGET ${EXEC_GENERATOR} PROPERTY CXX_STANDARD 14)
target_link_libraries(${EXEC_GENERATOR})
add_sanitize(${EXEC_GENERATOR})


set(EXEC_ANVIL "tfc_anvil")

set(SOURCES_ANVIL
  ${SOURCE_DIR}/AnvilChainsGetter.cpp
  ${SOURCE_DIR}/Config.cpp
  ${SOURCE_DIR}/MainWindow.cpp
  ${SOURCE_DIR}/Rule.cpp
  ${SOURCE_DIR}/ScanImage.cpp
  ${SOURCE_DIR}/Technique.cpp
  ${SOURCE_DIR}/anvil_chains.cpp
  ${SOURCE_DIR}/get_last_file.cpp
  ${SOURCE_DIR}/increment.cpp
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/png.cpp
  )

add_executable(${EXEC_ANVIL} ${SOURCES_ANVIL})
set_property(TARGET ${EXEC_ANVIL} PROPERTY CXX_STANDARD 14)
target_link_libraries(
  ${EXEC_ANVIL}
  ${FLTK_LIBRARIES}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY})
add_sanitize(${EXEC_ANVIL})
