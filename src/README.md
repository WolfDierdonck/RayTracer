# Project

## Compilation

No changes to the standard compilation are needed. I simply ran `premake4 gmake && make` from the root directory, and then I ran `premake4 gmake && make` from the `A5` directory. Then, you can run the executable with `./A5 ./Scenes/[scene-file-name].lua`.

## Manual

My final image as described in the project description is stored in `A5/screenshot.png` (titled "EIT"). Additionally, I created a second scene based on my A4 scene with the new features, stored in `A5/screenshot_2.png` (titled "Picnic").

I completed all objectives. My objectives for this project were:

1: Both primitives and meshes with UV information can be texture mapped with texture images.
2: Both primitives and meshes with UV information can be normal mapped with images.
3: Cylinder and cone primitives are supported, on top of the existing sphere and box primitives.
4: Phong Shading is implemented on meshes to interpolate vertex normals.
5: Primitives support simple Constructive Solid Geometry with intersection, union, and difference oper- ators. In each path from a leaf to the root node, only one CSG operation needs to be supported.
6: Primitives support complex Constructive Solid Geometry; any number of CSG operations on a path should be supported.
7: Reflective surfaces work correctly and recursion halts after some amount of reflections.
8: The ray tracer is sped up by adding multi-threading support, rendering pixels on separate threads.
9: Cylinder primitives can have emissive materials that act as area lights to illuminate the scene and provide shadows.
10: A nice, novel scene is rendered that demonstrates the ray tracer’s features.
Note that the extra objective I implemented in A4 was supersampling.

## Asset References

References for all the assets are credited in the project report pdf. Additionally, a copy of this can be found in the `sources.txt` file in this directory
