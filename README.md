"# OpenGLTemplate" 
Make sure it's in x86, It'll likely start in x64, can find this at the top.
Right click the project, go to settings

C++ 
Additional Directories
add this
$(SolutionDir)ThirdParty/GLAD/include
$(SolutionDir)ThirdParty/GLFW/include
$(SolutionDir)ThirdParty/GLM

Go to pre processor definitations

GLFW_INCLUDE_NONE;

then go to the linker
add additional dependancies
$(SolutionDir)ThirdParty/GLFW/lib-vc2019;

then add the library
glfw3.lib