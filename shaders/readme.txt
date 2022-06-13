I've inserted my shaders directly as C-strings.
You must not do that - rather you must provide
vertex and fragment shader source files that
must be compiled and linked into a shader program
by your application.
These shader source files must be located in
$(SolutionDir)shaders