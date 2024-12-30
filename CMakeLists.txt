cmake_minimum_required(VERSION 3.1)
project(hypertetris)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${CMAKE_CURRENT_LIST_DIR} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
add_executable(hypertetris
               Main.C
               Shared.C
               FourD.C
               Hyper.C
               Asserts.C
               Board.C
               GenPiece.C
               CubePiece.C
               DList.C
               LPiece.C
               StraightPiece.C
               TPiece.C
               SPiece.C
               CornerPiece.C
               TwistyPiece.C
               ZPiece.C
               ThreeLinePiece.C
               TriPiece.C
               TwoPiece.C
               SimplePiece.C
               Rotor.C)
target_link_libraries(hypertetris ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})
if (APPLE)
    target_link_libraries(hypertetris ${GLUT_cocoa_LIBRARY})
endif()
install(TARGETS hypertetris)

