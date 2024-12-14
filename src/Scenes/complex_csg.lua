mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0, 0.0)
mat2 = gr.material({1.0, 0.7, 0.7}, {0.7, 0.5, 0.5}, 25, 0, 0.0)

scene_root = gr.node('root')

-- We will have a sphere center at (0, 0, 0)
-- this sphere will then only have the back half (intersect with cube from z=-1 to z=0)
-- then, a cylinder in the middle will be subtracted from the sphere

-----------------------------------------------
-- Outer
-----------------------------------------------

outer = gr.difference('outer')
scene_root:add_child(outer)
outer:translate(0, -1, 0)
outer:rotate('y', 60)

inner = gr.intersection('inner')
outer:add_child(inner)

sphere = gr.sphere('sphere')
inner:add_child(sphere)
sphere:set_material(mat2)

cube = gr.cube('cube')
inner:add_child(cube)
cube:translate(-0.5, -0.5, -1)
cube:scale(2, 2, 1)
cube:set_material(mat2)

cylinder = gr.sphere('cylinder')
outer:add_child(cylinder)
cylinder:scale(0.5, 0.5, 0.5)
cylinder:set_material(mat2)


-----------------------------------------------
-- CSG Intersection
-----------------------------------------------

csg_intersection = gr.intersection('csg_intersection')
scene_root:add_child(csg_intersection)
csg_intersection:translate(0, 1, 0)

box_outer = gr.cube('box_outer')
csg_intersection:add_child(box_outer)
box_outer:translate(0, 0, -1)
box_outer:set_material(mat1)

-----------------------------------------------
-- CSG Difference
-----------------------------------------------

csg_difference = gr.difference('csg_difference')
csg_intersection:add_child(csg_difference)
-- csg_difference:scale(0.8, 0.8, 0.8)

box_parent_2 = gr.node('box_parent_2')
csg_difference:add_child(box_parent_2)
box_parent_2:translate(-1, -1, -1)

box_2 = gr.cube('box_2')
box_parent_2:add_child(box_2)
box_2:set_material(mat2)
box_2:scale(2.0, 2.0, 2.0)

sphere_parent_4 = gr.node('sphere_parent_4')
csg_difference:add_child(sphere_parent_4)

sphere_4 = gr.sphere('sphere_4')
sphere_parent_4:add_child(sphere_4)
sphere_4:set_material(mat2)
sphere_4:scale(1.4, 1.4, 1.4)

csg_intersection = gr.intersection('csg_intersection')


white_light = gr.light({5, 0, 0}, {0.7, 0.7, 0.7}, {1, 0, 0})
white_light_2 = gr.light({-5, 0, 0}, {0.7, 0.7, 0.7}, {1, 0, 0})

render_metadata = {
    image_name = './Images/complex_csg.png',
    image_width = 512,
    image_height = 512,
    camera_eye = {0, 2, 6},
    camera_view = {0, -0.3, -1},
    camera_up = {0, 1, 0},
    camera_fovy = 50,
    scene_ambient = {0.5, 0.5, 0.5},
    scene_lights = {white_light, white_light_2},
    enable_supersampling = false,
    thread_count = 8,
    background_image = ""
}

gr.render(scene_root, render_metadata)