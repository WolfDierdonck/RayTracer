white = gr.material({0.9, 0.9, 0.9}, {0.9, 0.9, 0.9}, 10, 0, 0.0)
floor_dark_grey = gr.material({0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, 10, 0.15, 0.0)
left_wall_color = gr.material({0.5, 0.7, 0.9}, {0.0, 0.0, 0.0}, 10, 0, 0.0)
clear_glass = gr.material({0.9, 1.0, 0.98}, {1.0, 1.0, 1.0}, 125, 0.08, 0.84)
opaque_glass = gr.material({1.0, 1.0, 1.0}, {0.9, 0.5, 0.9}, 125, 0.08, 0.20)
orange = gr.material({1.0, 0.5, 0.0}, {0.9, 0.9, 0.9}, 10, 0, 0.0)
couch_green = gr.material({0.0, 0.5, 0.0}, {0.9, 0.9, 0.9}, 10, 0, 0.0)
couch_blue = gr.material({0.0, 0.0, 0.5}, {0.9, 0.9, 0.9}, 10, 0, 0.0)
whiteboard_mat = gr.material({0.9, 0.9, 0.9}, {0.9, 0.9, 0.9}, 5, 0.05, 0.0)
grey_metal = gr.material({0.5, 0.5, 0.5}, {0.9, 0.9, 0.9}, 10, 0, 0.0)
cabinet_black = gr.material({0.1, 0.1, 0.1}, {0.9, 0.9, 0.9}, 10, 0, 0.0)
fridge_silver = gr.material({0.6, 0.6, 0.6}, {0.9, 0.9, 0.9}, 10, 0, 0.0)
chair_blue = gr.material({0.0, 0.0, 0.5}, {0.9, 0.9, 0.9}, 10, 0, 0.0)
gray_fur = gr.material({0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}, 0, 0, 0.0)

scene = gr.node('scene')
ROOM_HEIGHT = 4.0
ROOM_HALF_WIDTH = 3.5
ROOM_DEPTH = 12.0

-----------------------------------------------
-- Walls (not including the back wall)
-----------------------------------------------

floor = gr.cube('floor')
scene:add_child(floor)
floor:set_material(floor_dark_grey)
floor:scale(ROOM_HALF_WIDTH * 2, 0.1, ROOM_DEPTH)
floor:translate(-ROOM_HALF_WIDTH, -0.1, -ROOM_DEPTH)
floor:set_texture("./Assets/texture_stone_tiles.png")
floor:set_normal("./Assets/normal_stone_tiles.png")

left_wall = gr.cube('left_wall')
scene:add_child(left_wall)
left_wall:set_material(left_wall_color)
left_wall:scale(0.1, ROOM_HEIGHT, ROOM_DEPTH)
left_wall:translate(-ROOM_HALF_WIDTH - 0.1, 0.0, -ROOM_DEPTH)
left_wall:set_normal("./Assets/normal_wall.png")

right_wall = gr.cube('right_wall')
scene:add_child(right_wall)
right_wall:set_material(white)
right_wall:scale(0.1, ROOM_HEIGHT, ROOM_DEPTH)
right_wall:translate(ROOM_HALF_WIDTH, 0.0, -ROOM_DEPTH)
right_wall:set_normal("./Assets/normal_wall.png")

front_wall = gr.cube('front_wall')
scene:add_child(front_wall)
front_wall:set_material(white)
front_wall:scale(ROOM_HALF_WIDTH * 2, ROOM_HEIGHT, 0.1)
front_wall:translate(-ROOM_HALF_WIDTH, 0.0, -0.1)
front_wall:set_normal("./Assets/normal_wall.png")

-----------------------------------------------
-- Back Wall
-----------------------------------------------

back_wall_parent = gr.node('back_wall_parent')
scene:add_child(back_wall_parent)
back_wall_parent:translate(0, 0, -ROOM_DEPTH)

back_wall_big_hole_parent = gr.difference('back_wall_big_hole_parent')
back_wall_parent:add_child(back_wall_big_hole_parent)

back_wall = gr.cube('back_wall')
back_wall_big_hole_parent:add_child(back_wall)
back_wall:set_material(white)
back_wall:scale(ROOM_HALF_WIDTH * 2, ROOM_HEIGHT, 0.1)
back_wall:translate(-ROOM_HALF_WIDTH, 0.0, -0.1)
back_wall:set_normal("./Assets/normal_wall.png")

back_wall_holes = gr.node('back_wall_holes')
back_wall_big_hole_parent:add_child(back_wall_holes)

back_wall_big_hole = gr.cube('back_wall_hole')
back_wall_holes:add_child(back_wall_big_hole)
back_wall_big_hole:set_material(white)
back_wall_big_hole:scale(ROOM_HALF_WIDTH + 0.5, ROOM_HEIGHT / 2.3, 0.3)
back_wall_big_hole:translate(-1, ROOM_HEIGHT / 2.1, -0.2)

back_wall_small_hole_1 = gr.cube('back_wall_small_hole_1')
back_wall_holes:add_child(back_wall_small_hole_1)
back_wall_small_hole_1:set_material(white)
back_wall_small_hole_1:scale(ROOM_HALF_WIDTH + 0.5, ROOM_HEIGHT / 4.6, 0.3)
back_wall_small_hole_1:translate(-1, ROOM_HEIGHT / 4.3, -0.2)

back_wall_big_window = gr.cube('back_wall_window')
back_wall_parent:add_child(back_wall_big_window)
back_wall_big_window:set_material(clear_glass)
back_wall_big_window:scale(ROOM_HALF_WIDTH + 0.5, ROOM_HEIGHT / 2.3, 0.1)
back_wall_big_window:translate(-1, ROOM_HEIGHT / 2.1, -0.1)

back_wall_small_window = gr.cube('back_wall_small_window')
back_wall_parent:add_child(back_wall_small_window)
back_wall_small_window:set_material(opaque_glass)
back_wall_small_window:scale(ROOM_HALF_WIDTH + 0.5, ROOM_HEIGHT / 4.6, 0.1)
back_wall_small_window:translate(-1, ROOM_HEIGHT / 4.3, -0.1)

-----------------------------------------------
-- Ceiling Lights
-----------------------------------------------
ceiling = gr.cube('ceiling')
scene:add_child(ceiling)
ceiling:set_material(left_wall_color)
ceiling:scale(ROOM_HALF_WIDTH * 3, 0.1, ROOM_DEPTH * 1.5)
ceiling:translate(-ROOM_HALF_WIDTH * 1.5, ROOM_HEIGHT, -ROOM_DEPTH)
ceiling:set_texture("./Assets/texture_scene_ceiling.png")
ceiling:set_normal("./Assets/normal_scene_ceiling.png")

light_1_parent = gr.node('light_1_parent')
scene:add_child(light_1_parent)
light_1_parent:translate(-0.975, ROOM_HEIGHT - 0.001, -6.95)

light_1 = gr.cube('light_1')
light_1_parent:add_child(light_1)
light_1:set_material(floor_dark_grey)
light_1:set_emission({1.0, 0.75, 0.45}, {1, 0.15, 0}, 100)
light_1:scale(1.95, 0.001, 1.13)

-----------------------------------------------
-- Objects
-----------------------------------------------

function create_couch(name_suffix, color) 
    couch = gr.node('couch' .. name_suffix)

    couch_back = gr.cube('couch_back' .. name_suffix)
    couch:add_child(couch_back)
    couch_back:set_material(couch_green)
    couch_back:scale(1.7, 1.3, 0.4)
    couch_back:rotate('x', -15)
    couch_back:set_texture("./Assets/texture_couch_" .. color .. ".png")
    couch_back:set_normal("./Assets/normal_couch.png")

    couch_seat = gr.cube('couch_seat' .. name_suffix)
    couch:add_child(couch_seat)
    couch_seat:set_material(couch_green)
    couch_seat:scale(1.7, 0.6, 1.0)
    couch_seat:set_texture("./Assets/texture_couch_" .. color .. ".png")
    couch_seat:set_normal("./Assets/normal_couch.png")

    return couch
end

couch_1 = create_couch('_1', 'green')
scene:add_child(couch_1)
couch_1:translate(0.9, 0, -10)

couch_2 = create_couch('_2', 'green')
scene:add_child(couch_2)
couch_2:translate(-0.9, 0, -10)

whiteboard_1 = gr.cube('whiteboard_1')
scene:add_child(whiteboard_1)
whiteboard_1:set_material(whiteboard_mat)
whiteboard_1:scale(0.1, 1.5, 3.0)
whiteboard_1:translate(ROOM_HALF_WIDTH - 0.1, 1.5, -6)
whiteboard_1:set_texture("./Assets/texture_whiteboard.png")

whiteboard_2 = gr.cube('whiteboard_2')
scene:add_child(whiteboard_2)
whiteboard_2:set_material(whiteboard_mat)
whiteboard_2:scale(0.1, 1.5, 3.0)
whiteboard_2:translate(ROOM_HALF_WIDTH - 0.1, 1.5, -10)
whiteboard_2:set_texture("./Assets/texture_whiteboard.png")

couch_3_parent = gr.node('couch_3_parent')
scene:add_child(couch_3_parent)
couch_3_parent:translate(-ROOM_HALF_WIDTH + 0.2, 0, -2)

couch_3 = create_couch('_3', 'blue')
couch_3_parent:add_child(couch_3)
couch_3:rotate('y', 90)

couch_4_parent = gr.node('couch_4_parent')
scene:add_child(couch_4_parent)
couch_4_parent:translate(-ROOM_HALF_WIDTH + 0.2, 0, -3.7)

couch_4 = create_couch('_4', 'blue')
couch_4_parent:add_child(couch_4)
couch_4:rotate('y', 90)

couch_5_parent = gr.node('couch_5_parent')
scene:add_child(couch_5_parent)
couch_5_parent:translate(-ROOM_HALF_WIDTH + 0.2, 0, -5.5)

couch_5 = create_couch('_5', 'green')
couch_5_parent:add_child(couch_5)
couch_5:rotate('y', 90)

table_parent = gr.union('table_parent')
scene:add_child(table_parent)
table_parent:translate(-1.3, 0, -4)

table_leg = gr.cylinder('table_leg')
table_parent:add_child(table_leg)
table_leg:set_material(grey_metal)
table_leg:scale(0.1, 1.1, 0.1)

table_top = gr.cylinder('table_top')
table_parent:add_child(table_top)
table_top:set_material(white)
table_top:scale(1.1, 0.1, 1.1)
table_top:translate(0, 1.1, 0)

fridge = gr.cube('fridge')
scene:add_child(fridge)
fridge:set_material(fridge_silver)
fridge:scale(1.3, 2.5, 1.3)
fridge:translate(-ROOM_HALF_WIDTH, 0, -9.5)
fridge:set_normal("./Assets/normal_wall.png")

handle_parent = gr.intersection('handle_parent')
scene:add_child(handle_parent)
handle_parent:translate(-ROOM_HALF_WIDTH+0.88, 1.6, -8.5)

handle_intermediate = gr.difference('handle_intermediate')
handle_parent:add_child(handle_intermediate)

handle_outer = gr.sphere('handle_outer')
handle_intermediate:add_child(handle_outer)
handle_outer:set_material(fridge_silver)
handle_outer:scale(0.5, 0.5, 0.5)

handle_inner = gr.sphere('handle_inner')
handle_intermediate:add_child(handle_inner)
handle_inner:set_material(fridge_silver)
handle_inner:scale(0.45, 0.45, 0.45)

handle_other = gr.cube('handle_other')
handle_parent:add_child(handle_other)
handle_other:set_material(fridge_silver)
handle_other:scale(0.1, 1.0, 0.1)
handle_other:translate(0.4, -0.5, -0.05)

table_parent_2 = gr.union('table_parent_2')
scene:add_child(table_parent_2)
table_parent_2:translate(0.7, 0, -8.3)

table_leg_2 = gr.cylinder('table_leg_2')
table_parent_2:add_child(table_leg_2)
table_leg_2:set_material(grey_metal)
table_leg_2:scale(0.05, 0.9, 0.05)

table_top_2 = gr.cube('table_top_2')
table_parent_2:add_child(table_top_2)
table_top_2:set_material(white)
table_top_2:scale(1.1, 0.06, 0.6)
table_top_2:translate(-0.55, 0.9, -0.3)

-----------------------------------------------
-- Chairs
-----------------------------------------------

function create_chair(name_suffix)
    chair = gr.node('chair' .. name_suffix)

    chair_leg_1 = gr.cylinder('chair_leg_1' .. name_suffix)
    chair:add_child(chair_leg_1)
    chair_leg_1:set_material(grey_metal)
    chair_leg_1:scale(0.02, 0.5, 0.02)
    chair_leg_1:translate(0.3, 0, 0.2)

    chair_leg_2 = gr.cylinder('chair_leg_2' .. name_suffix)
    chair:add_child(chair_leg_2)
    chair_leg_2:set_material(grey_metal)
    chair_leg_2:scale(0.02, 0.5, 0.02)
    chair_leg_2:translate(0.3, 0, -0.2)

    chair_leg_3 = gr.cylinder('chair_leg_3' .. name_suffix)
    chair:add_child(chair_leg_3)
    chair_leg_3:set_material(grey_metal)
    chair_leg_3:scale(0.02, 0.5, 0.02)
    chair_leg_3:translate(-0.3, 0, 0.2)

    chair_leg_4 = gr.cylinder('chair_leg_4' .. name_suffix)
    chair:add_child(chair_leg_4)
    chair_leg_4:set_material(grey_metal)
    chair_leg_4:scale(0.02, 0.5, 0.02)
    chair_leg_4:translate(-0.3, 0, -0.2)

    chair_seat = gr.cube('chair_seat' .. name_suffix)
    chair:add_child(chair_seat)
    chair_seat:set_material(chair_blue)
    chair_seat:scale(0.8, 0.1, 0.5)
    chair_seat:translate(-0.4, 0.5, -0.25)
    chair_seat:set_texture("./Assets/texture_couch_blue.png")
    chair_seat:set_normal("./Assets/normal_couch.png")

    chair_back_parent = gr.difference('chair_back_parent' .. name_suffix)
    chair:add_child(chair_back_parent)

    chair_back = gr.cube('chair_back' .. name_suffix)
    chair_back_parent:add_child(chair_back)
    chair_back:set_material(white)
    chair_back:scale(0.8, 0.75, 0.05)
    chair_back:translate(-0.4, 0.5, -0.3)

    chair_back_hole = gr.cube('chair_back_hole' .. name_suffix)
    chair_back_parent:add_child(chair_back_hole)
    chair_back_hole:set_material(white)
    chair_back_hole:scale(0.6, 0.2, 0.3)
    chair_back_hole:translate(-0.3, 0.7, -0.4)

    chair_arm_1 = gr.cube('chair_arm_1' .. name_suffix)
    chair:add_child(chair_arm_1)
    chair_arm_1:set_material(white)
    chair_arm_1:scale(0.07, 0.03, 0.4)
    chair_arm_1:translate(-0.45, 0.9, -0.3)

    chair_arm_2 = gr.cube('chair_arm_2' .. name_suffix)
    chair:add_child(chair_arm_2)
    chair_arm_2:set_material(white)
    chair_arm_2:scale(0.07, 0.03, 0.4)
    chair_arm_2:translate(0.38, 0.9, -0.3)


    return chair
end

chair_1_parent = gr.node('chair_1_parent')
scene:add_child(chair_1_parent)
chair_1_parent:translate(-1, 0, -5.5)

chair_1 = create_chair('_1')
chair_1_parent:add_child(chair_1)
chair_1:rotate('y', -30)

chair_2_parent = gr.node('chair_2_parent')
scene:add_child(chair_2_parent)
chair_2_parent:translate(0.0, 0, -4)

chair_2 = create_chair('_2')
chair_2_parent:add_child(chair_2)
chair_2:rotate('y', -90)

kitchen = gr.cube('kitchen')
scene:add_child(kitchen)
kitchen:set_material(fridge_silver)
kitchen:scale(1.0, 1.5, 2.5)
kitchen:translate(-ROOM_HALF_WIDTH, 0, -ROOM_DEPTH)

moop_parent = gr.node('moop_parent')
scene:add_child(moop_parent)
moop_parent:translate(-1, 1.3, -4)

moop = gr.mesh('moop', './Assets/moop.obj')
moop_parent:add_child(moop)
moop:set_material(gray_fur)
moop:scale(0.3 / 1.3, 0.35 / 1.3, 0.35 / 1.3)
moop:rotate('Y', 120)
moop:set_texture('./Assets/texture_moop.png')
moop:set_normal('./Assets/normal_moop.png')

moop_parent_2 = gr.node('moop_parent_2')
scene:add_child(moop_parent_2)
moop_parent_2:translate(2, 0.8, -6)

moop_2 = gr.mesh('moop_2', './Assets/moop.obj')
moop_parent_2:add_child(moop_2)
moop_2:set_material(gray_fur)
moop_2:scale(0.3 / 1.3, 0.35 / 1.3, 0.35 / 1.3)
moop_2:rotate('Y', -35)
moop_2:set_texture('./Assets/texture_moop.png')
moop_2:set_normal('./Assets/normal_moop.png')

ottoman_parent = gr.node('ottoman_parent')
scene:add_child(ottoman_parent)
ottoman_parent:translate(2, 0, -6)

ottoman = gr.cylinder('ottoman')
ottoman_parent:add_child(ottoman)
ottoman:set_material(couch_blue)
ottoman:set_texture("./Assets/texture_couch_blue.png")
ottoman:set_normal("./Assets/normal_couch.png")
ottoman:scale(0.5, 0.7, 0.5)


lights = {gr.light({0.0, ROOM_HEIGHT, -ROOM_DEPTH * 1.5}, {0.2, 0.2, 0.2}, {1, 0.0, 0.0})}

render_metadata = {
    image_name = './Images/scene.png',
    image_width = 1000,
    image_height = 600,
    camera_eye = {0, 2.0, -0.1},
    camera_view = {0, 0.0, -1},
    camera_up = {0, 1, 0},
    camera_fovy = 60,
    scene_ambient = {0.2, 0.2, 0.2},
    scene_lights = lights,
    enable_supersampling = true,
    thread_count = 8,
    background_image = "./Assets/background_scene.png"
}

gr.render(scene, render_metadata)
