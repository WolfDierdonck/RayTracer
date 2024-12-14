green = gr.material({0.6, 1.0, 0.6}, {0.5, 0.7, 0.5}, 25, 0, 0.0)
red = gr.material({1.0, 0.6, 0.6}, {0.7, 0.5, 0.5}, 25, 0, 0.0)
blue = gr.material({0.6, 0.6, 1.0}, {0.5, 0.5, 0.7}, 25, 0, 0.0)
white = gr.material({1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
white_light = gr.material({1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, 0, 0, 0.0)

scene_root = gr.node('root')

floor = gr.cube('floor')
scene_root:add_child(floor)
floor:scale(20.0, 0.2, 20.0)
floor:translate(-10.0, -0.2, -10.0)
floor:set_material(green)

left_wall = gr.cube('left_wall')
scene_root:add_child(left_wall)
left_wall:set_material(red)
left_wall:scale(0.1, 10.0, 30.0)
left_wall:translate(-3.0, -1.0, -10.0)

back_wall = gr.cube('back_wall')
scene_root:add_child(back_wall)
back_wall:set_material(blue)
back_wall:scale(30.0, 10.0, 0.1)
back_wall:translate(-10.0, -1.0, -10.0)

right_wall = gr.cube('right_wall')
scene_root:add_child(right_wall)
right_wall:set_material(red)
right_wall:scale(0.1, 10.0, 30.0)
right_wall:translate(3.0, -1.0, -10.0)

cylinder = gr.cylinder('cylinder')
scene_root:add_child(cylinder)
cylinder:set_material(white_light)
cylinder:set_emission({0.9, 0.9, 0.9}, {1, 0.15, 0}, 100) 
cylinder:translate(0.0, 1.0, -5.0)

sphere_1 = gr.sphere('sphere_1')
scene_root:add_child(sphere_1)
sphere_1:set_material(white)
sphere_1:scale(0.5, 0.5, 0.5)
sphere_1:translate(-2.0, 0.5, 0.0)

sphere_2 = gr.sphere('sphere_2')
scene_root:add_child(sphere_2)
sphere_2:set_material(white)
sphere_2:scale(0.5, 0.5, 0.5)
sphere_2:translate(2.0, 5, -5.0)

-- horse = gr.mesh('horse', './Assets/horse.obj')
-- scene_root:add_child(horse)
-- horse:set_material(white)
-- horse:set_texture('./Assets/texture_horse.png')
-- horse:set_normal('./Assets/normal_horse.png')
-- horse:scale(6, 6, 6)
-- horse:translate(-2.0, 3.0, -7.0)

render_metadata = {
    image_name = './Images/emission.png',
    image_width = 512,
    image_height = 512,
    camera_eye = {0, 6, 10},
    camera_view = {0, -0.4, -1},
    camera_up = {0, 1, 0},
    camera_fovy = 50,
    scene_ambient = {0.0, 0.0, 0.0},
    scene_lights = {gr.light({0, 10, -5}, {0.1, 0.1, 0.1}, {1, 0, 0})},
    enable_supersampling = false,
    thread_count = 8,
    background_image = ""
}

gr.render(scene_root, render_metadata)