## RayTracer

A raytracer I made for a school project.

### Building

This project uses `premake4` to manage build steps. First, generate the root level Makefile using `premake4 gmake`. This will generate a new Makefile, which you can use to build the dependencies by running `make` (again in the root directory).

Then, you can build the raytracer specific code in the `src` directory. Again, first run `premake4 gmake` in this directory to generate a Makefile. Then, run `make` in the `src` directory to build the project.

Note: On MacOS, these steps will only work when running the shell as x86. You can do this by first running `arch -x86_64 /bin/zsh` (replaced with the path to your shell).

### Running

