cmake_minimum_required (VERSION 3.0.0)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
if (WIN32)
  set(CMAKE_C_COMPILER "g++.exe")
  set(CMAKE_CXX_COMPILER "g++.exe")
endif (WIN32)
project (Narradia)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32)
  set(CMAKE_CXX_FLAGS "-Wfatal-errors -std=c++20")
endif (WIN32)

if (UNIX)
  set(CMAKE_CXX_FLAGS "-Wfatal-errors -std=c++20")
endif (UNIX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Bin)

ADD_CUSTOM_TARGET(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --parallel 4 --target all
  COMMENT "Creating the executable in the debug mode.")

ADD_CUSTOM_TARGET(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --parallel 4 --target all
  COMMENT "Creating the executable in the release mode.")



if (WIN32)
  include_directories("Source" "C:/Dependencies")
endif (WIN32)

if (UNIX)
  include_directories("Source" "C:/Dependencies")
endif (UNIX)

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
<GL/glew.h>
<glm/glm.hpp>
<glm/gtc/matrix_transform.hpp>
<glm/gtc/type_ptr.hpp>
<glm/ext/matrix_projection.hpp>
<glm/gtx/closest_point.hpp>
<assimp/Importer.hpp>
<assimp/scene.h>
<assimp/postprocess.h>

"Source/Common/Singleton.hpp"
"Source/Common/Typedefs.hpp"
"Source/Engine/Core/Logger.hpp"
"Source/Engine/Core/Utilities.hpp"
"Source/Matter/Color.hpp"
"Source/Matter/Colors.hpp"
"Source/Matter/Geometry/GlRectangleF.hpp"
"Source/Matter/Geometry/Point2.hpp"
"Source/Matter/Geometry/Point2F.hpp"
"Source/Matter/Geometry/Point3.hpp"
"Source/Matter/Geometry/Point3F.hpp"
"Source/Matter/Geometry/RectangleF.hpp"
"Source/Matter/Geometry/Size.hpp"
"Source/Matter/Geometry/SizeF.hpp"
"Source/Matter/Geometry/Vertex2F.hpp"
"Source/Matter/Geometry/Vertex3F.hpp"
"Source/Matter/Geometry/VertexTile.hpp"

"Source/Math/Trigonometry.hpp"
"Source/Math/ExperienceCalculations.hpp"
)

add_executable (Narradia ${SOURCE_FILES})

set_target_properties(Narradia PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

if (WIN32)
  target_link_libraries(Narradia PUBLIC mingw32 SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer opengl32 glew32 assimp)
endif (WIN32)

if (UNIX)
  target_link_libraries(Narradia PUBLIC SDL2 SDL2_image SDL2_ttf SDL2_mixer GL GLEW assimp)
endif (UNIX)

target_precompile_headers(Narradia PRIVATE ${PCH_FILES})
