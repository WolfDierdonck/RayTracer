mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0, 0.0)
mat2 = gr.material({1.0, 0.7, 0.7}, {0.7, 0.5, 0.5}, 25, 0, 0.0)

scene_root = gr.node('root')

-----------------------------------------------
-- CSG Intersection
-----------------------------------------------

csg_intersection = gr.intersection('csg_intersection')
scene_root:add_child(csg_intersection)
csg_intersection:translate(-1.5, 1, 0)

box_parent_1 = gr.node('box_parent_1')
csg_intersection:add_child(box_parent_1)
box_parent_1:translate(-2, -1, 0)

box_1 = gr.cube('box_1')
box_parent_1:add_child(box_1)
box_1:set_material(mat1)
box_1:scale(2, 2, 2)

sphere_parent_1 = gr.node('sphere_parent_1')
csg_intersection:add_child(sphere_parent_1)

sphere_1 = gr.sphere('sphere_1')
sphere_parent_1:add_child(sphere_1)
sphere_1:set_material(mat1)

-----------------------------------------------
-- CSG Union
-----------------------------------------------

csg_union = gr.union('csg_union')
scene_root:add_child(csg_union)
csg_union:translate(0.5, 1, 0)

sphere_parent_2 = gr.node('sphere_parent_2')
csg_union:add_child(sphere_parent_2)

sphere_2 = gr.sphere('sphere_2')
sphere_parent_2:add_child(sphere_2)
sphere_2:set_material(mat1)

sphere_parent_3 = gr.node('sphere_parent_3')
csg_union:add_child(sphere_parent_3)
sphere_parent_3:translate(1, 0, 0)

sphere_3 = gr.sphere('sphere_3')
sphere_parent_3:add_child(sphere_3)
sphere_3:set_material(mat2)

-----------------------------------------------
-- CSG Difference
-----------------------------------------------

csg_difference = gr.difference('csg_difference')
scene_root:add_child(csg_difference)
csg_difference:translate(0, -1.5, 0)
csg_difference:scale(0.8, 0.8, 0.8)

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

white_light = gr.light({0, 0, 5}, {0.9, 0.9, 0.9}, {1, 0, 0})

render_metadata = {
    image_name = './Images/simple_csg.png',
    image_width = 512,
    image_height = 512,
    camera_eye = {0, 0, 6},
    camera_view = {0, 0, -1},
    camera_up = {0, 1, 0},
    camera_fovy = 50,
    scene_ambient = {0.5, 0.5, 0.5},
    scene_lights = {white_light},
    enable_supersampling = false,
    thread_count = 8,
    background_image = ""
}

gr.render(scene_root, render_metadata)