cmake_minimum_required (VERSION 3.0.0)
set(CMAKE_CXX_COMPILER_WORKS 1)
project (Narradia)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wfatal-errors -std=c++20")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Bin)

ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --parallel 4 --target all
  COMMENT "Creating the executable in the debug mode.")

ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --parallel 4 --target all
  COMMENT "Creating the executable in the release mode.")

include_directories("Source")

file(GLOB_RECURSE SOURCE_FILES Source/**.cpp)

list(FILTER SOURCE_FILES EXCLUDE REGEX ".*inc.cpp$")

set(PCH_FILES
<iostream>
<memory>
<string>
<vector>
<map>
<set>
<functional>
<numeric>
<filesystem>
<fstream>
<algorithm>
<array>
<deque>
<source_location>
<ranges>

<SDL2/SDL.h>
<SDL2/SDL_image.h>
<SDL2/SDL_ttf.h>
<SDL2/SDL_mixer.h>
<SDL2/SDL_net.h>
<GL/glew.h>
<glm/glm.hpp>
<glm/gtc/matrix_transform.hpp>
<glm/gtc/type_ptr.hpp>
<glm/ext/matrix_projection.hpp>
<glm/gtx/closest_point.hpp>
<assimp/Importer.hpp>
<assimp/scene.h>
<assimp/postprocess.h>

"Source/Core/Core.hpp"
"Source/Matter/Color.hpp"
"Source/Matter/Colors.hpp"
"Source/Matter/GlRectangleF.hpp"
"Source/Matter/Point2.hpp"
"Source/Matter/Point2F.hpp"
"Source/Matter/Point3.hpp"
"Source/Matter/Point3F.hpp"
"Source/Matter/RectangleF.hpp"
"Source/Matter/Size.hpp"
"Source/Matter/SizeF.hpp"
"Source/Matter/Vertex2F.hpp"
"Source/Matter/Vertex3F.hpp"
"Source/Matter/VertexTile.hpp"

"Source/Math/Trigonometry.hpp"
"Source/Math/ExperienceCalculations.hpp"
)

add_executable (Narradia ${SOURCE_FILES})

set_target_properties(Narradia PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(Narradia PUBLIC SDL2 SDL2_image SDL2_ttf SDL2_mixer SDL2_net GL GLEW assimp)

target_precompile_headers(Narradia PRIVATE ${PCH_FILES})
