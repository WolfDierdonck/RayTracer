mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0, 0.0)

scene_root = gr.node('root')

cylinder = gr.cylinder('cylinder')
scene_root:add_child(cylinder)
cylinder:translate(-2, -0.5, 0)
cylinder:set_material(mat1)
cylinder:scale(1, 2, 1)

cone_parent = gr.node('cone_parent')
scene_root:add_child(cone_parent)
cone_parent:rotate('x', -50)

cone = gr.cone('cone')
cone_parent:add_child(cone)
cone:set_material(mat1)
cone:scale(1, 2, 1)
cone:translate(1, 1, 0)

white_light = gr.light({0, 0.0, 5}, {0.7, 0.7, 0.7}, {1, 0, 0})

render_metadata = {
    image_name = './Images/primitive_test.png',
    image_width = 512,
    image_height = 512,
    camera_eye = {0, 4, 10},
    camera_view = {0, -0.2, -1},
    camera_up = {0, 1, 0},
    camera_fovy = 50,
    scene_ambient = {0.5, 0.5, 0.5},
    scene_lights = {white_light},
    enable_supersampling = false,
    thread_count = 8,
    background_image = ""
}

gr.render(scene_root, render_metadata)