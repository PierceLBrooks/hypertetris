cmake_minimum_required(VERSION 3.13)
project(hypertetris LANGUAGES CXX)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
set(SRCS )
list(APPEND SRCS
               Main.CC
               Shared.CC
               FourD.CC
               Hyper.CC
               Asserts.CC
               Board.CC
               GenPiece.CC
               CubePiece.CC
               DList.CC
               LPiece.CC
               StraightPiece.CC
               TPiece.CC
               SPiece.CC
               CornerPiece.CC
               TwistyPiece.CC
               ZPiece.CC
               ThreeLinePiece.CC
               TriPiece.CC
               TwoPiece.CC
               SimplePiece.CC
               Rotor.CC)
foreach(SRC IN LISTS SRCS)
    set_source_files_properties(${SRC} PROPERTIES LANGUAGE CXX)
endforeach()
add_executable(hypertetris ${SRCS})
set_target_properties(hypertetris PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(hypertetris PRIVATE cxx_std_11)
if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
    target_include_directories(hypertetris PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIR}/GL)
    target_link_libraries(hypertetris ${OPENGL_LIBRARIES} GLUT::GLUT)
    #add_custom_command(TARGET hypertetris POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:GLUT::GLUT> ${CMAKE_INSTALL_PREFIX}/bin/)
else()
    target_include_directories(hypertetris PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
    target_link_libraries(hypertetris ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})
    if (APPLE)
        target_link_libraries(hypertetris ${GLUT_cocoa_LIBRARY})
    endif()
endif()
install(TARGETS hypertetris)
