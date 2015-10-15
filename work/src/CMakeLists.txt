#
# COMP 308
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"comp308.hpp"
	"skeleton.hpp"
	"geometry.hpp"
	"splines.hpp"
	"quat.hpp"
	"shaderLoader.hpp"
	"imageLoader.hpp"
	"texture.hpp"
	"TinyPngOut.hpp"
)


# TODO list your source files (.cpp) here
SET(sources
	"main.cpp"
	"skeleton.cpp"
	"splines.cpp"
	"geometry.cpp"
	"texture.cpp"
	"TinyPngOut.cpp"
)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${COMP308_ASSIGNMENT} ${headers} ${sources})
target_link_libraries(${COMP308_ASSIGNMENT} PRIVATE GLUT::GLUT glew)
target_link_libraries(${COMP308_ASSIGNMENT} PRIVATE stb)