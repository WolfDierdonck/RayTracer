## RayTracer

A ray tracer I made for a school project, winning the `Gold` prize in the course. It supports a large set of features to realistically render indoor scenes such as: texture/normal mapping, reflections, transparency, and emissive materials. Additionally, Constructive Solid Geometry (CSG) is supported to easily create complex models (note that meshes are also supported). The ray tracer also supports multithreading to speed up the rendering.

I created the following scenes that were rendered using this ray tracer:
![scene](https://github.com/user-attachments/assets/070a6514-a769-4b01-87f7-5f4359f950d4)

![scene_2](https://github.com/user-attachments/assets/291fadd5-615b-45a5-8433-f0aca16ace24)

### Building

This project uses `premake4` to manage build steps. First, generate the root level Makefile using `premake4 gmake`. This will generate a new Makefile, which you can use to build the dependencies by running `make` (again in the root directory).

Then, you can build the raytracer specific code in the `src` directory. Again, first run `premake4 gmake` in this directory to generate a Makefile. Then, run `make` in the `src` directory to build the project.

Note: On MacOS, these steps will only work when running the shell as x86. You can do this by first running `arch -x86_64 /bin/zsh` (replaced with the path to your shell).

### Running

The project can be run inside the `src` directory by running `./Raytracer ./Scenes/[input-scene].lua`. Any of the sample scenes in the `src/Scene` directory can be used, which create a png in the `src/Image` directory (of the same name as the scene). New scenes can also easily be created using a hierarchical Lua language.
