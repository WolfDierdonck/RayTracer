mat1 = gr.material({0.7, 0.7, 0.7}, {0.7, 0.7, 0.7}, 25, 0, 0.0)

scene_root = gr.node('root')

horse = gr.mesh('horse', './Assets/horse.obj')
scene_root:add_child(horse)
horse:set_material(mat1)
horse:set_texture('./Assets/texture_horse.png')
-- horse:set_normal('./Assets/normal_horse.png')
horse:scale(10, 10, 10)
horse:rotate('Y', -40)
horse:rotate('X', 20)
horse:translate(0, -1.8, 0)

white_light = gr.light({0, 3, 6}, {0.9, 0.9, 0.9}, {1, 0, 0})

render_metadata = {
    image_name = './Images/horse_texture.png',
    image_width = 512,
    image_height = 512,
    camera_eye = {0, 0, 6},
    camera_view = {0, 0, -1},
    camera_up = {0, 1, 0},
    camera_fovy = 50,
    scene_ambient = {0.3, 0.3, 0.3},
    scene_lights = {white_light},
    enable_supersampling = false,
    thread_count = 8,
    background_image = ""
}

gr.render(scene_root, render_metadata)