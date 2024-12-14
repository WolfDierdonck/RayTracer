mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0, 0.0)

scene_root = gr.node('root')

box_parent = gr.node('box_parent')
scene_root:add_child(box_parent)
box_parent:translate(1, -2.5, 0)

box = gr.cube('box')
box_parent:add_child(box)
box:set_texture("./Assets/texture_stone_tiles.png")
box:set_normal("./Assets/normal_stone_tiles.png")
box:set_material(mat1)
box:rotate('Y', 45)
box:scale(1.7, 1.7, 1.7)

sphere_parent = gr.node('sphere_parent')
scene_root:add_child(sphere_parent)
sphere_parent:translate(2, 2, 0)

sphere = gr.sphere('sphere')
sphere_parent:add_child(sphere)
sphere:set_texture("./Assets/texture_earth.png")
sphere:set_normal("./Assets/normal_earth.png")
sphere:set_material(mat1)
sphere:rotate('Y', 90)
sphere:rotate('Z', 180)
sphere:rotate('X', 45)

cylinder_parent = gr.node('cylinder_parent')
scene_root:add_child(cylinder_parent)
cylinder_parent:translate(-2, 1, 0)

cylinder = gr.cylinder('cylinder')
cylinder_parent:add_child(cylinder)
cylinder:set_texture("./Assets/texture_brick.png")
cylinder:set_normal("./Assets/normal_brick.png")
cylinder:set_material(mat1)
cylinder:rotate('Y', 45)
cylinder:scale(1, 2, 1)

cone_parent = gr.node('cone_parent')
scene_root:add_child(cone_parent)
cone_parent:translate(-2, -0.5, 0)

cone = gr.cone('cone')
cone_parent:add_child(cone)
cone:set_texture("./Assets/texture_brickwall.png")
cone:set_normal("./Assets/normal_brickwall.png")
cone:set_material(mat1)
cone:scale(1, 2, 1)


white_light = gr.light({0, 2.0, 4.5}, {0.9, 0.9, 0.9}, {1, 0, 0})

render_metadata = {
    image_name = './Images/normal_test.png',
    image_width = 512,
    image_height = 512,
    camera_eye = {0, 0, 8},
    camera_view = {0, 0, -1},
    camera_up = {0, 1, 0},
    camera_fovy = 50,
    scene_ambient = {0.3, 0.3, 0.3},
    scene_lights = {white_light},
    enable_supersampling = true,
    thread_count = 8,
    background_image = ""
}

gr.render(scene_root, render_metadata)