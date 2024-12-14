gray_fur = gr.material({0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
white_fur = gr.material({1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
black_fur = gr.material({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
grass = gr.material({0.1, 0.5, 0.2}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
picnic_blanket_red = gr.material({0.8, 0.2, 0.2}, {0.5, 0.5, 0.5}, 10, 0, 0.0)
brown_candle = gr.material({0.5, 0.2, 0.0}, {0.5, 0.5, 0.5}, 10, 0, 0.0)
leaf_green = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
candle_orange = gr.material({1.0, 0.5, 0.0}, {1.0, 0.5, 0.0}, 10, 0, 0.0)
glass = gr.material({0.9, 1.0, 0.98}, {1.0, 1.0, 1.0}, 125, 0.08, 0.8)
wine = gr.material({0.7, 0.3, 0.3}, {0.5, 0.5, 0.5}, 10, 0, 0.0)


scene = gr.node('scene')

grass_node = gr.cube('grass')
scene:add_child(grass_node)
grass_node:set_material(grass)
grass_node:scale(100.0, 0.1, 100.0)
grass_node:translate(-50.0, -0.5, -50.0)

picnic_blanket = gr.cube('picnic_blanket')
scene:add_child(picnic_blanket)
picnic_blanket:set_material(picnic_blanket_red)
picnic_blanket:scale(5.0, 0.1, 5.0)
picnic_blanket:translate(-2.5, -0.49, -2.5)

function create_tree(name_suffix)
    tree_node = gr.node('tree_node' .. name_suffix)
    tree_node:translate(0.0, 0.0, 0.0)

    trunk = gr.cylinder('trunk' .. name_suffix)
    tree_node:add_child(trunk)
    trunk:set_material(brown_candle)
    trunk:scale(0.4, 3.0, 0.4)
    trunk:translate(0.0, -1.0, 0.0)

    leaves = gr.cone('leaves' .. name_suffix)
    tree_node:add_child(leaves)
    leaves:set_material(leaf_green)
    leaves:translate(0.0, 1.4, 0)
    leaves:scale(1.5, 4.0, 1.5)

    return tree_node
end

tree_1 = create_tree('_1')
scene:add_child(tree_1)
tree_1:translate(-2.0, 0.0, -5.0)

tree_2 = create_tree('_2')
scene:add_child(tree_2)
tree_2:translate(2.0, 0.0, -6.0)

tree_3 = create_tree('_3')
scene:add_child(tree_3)
tree_3:translate(-1.0, 0.0, -15.0)

tree_4 = create_tree('_4')
scene:add_child(tree_4)
tree_4:translate(6.0, 0.0, -10.0)

tree_5 = create_tree('_5')
scene:add_child(tree_5)
tree_5:translate(-5.0, 0.0, -7.0)

rock_1 = gr.mesh('rock_1', './Assets/buckyball.obj')
scene:add_child(rock_1)
rock_1:set_material(stone)
rock_1:translate(3.0, -1.5, -20.0)
rock_1:scale(0.2, 0.2, 0.2)

rock_2 = gr.mesh('rock_2', './Assets/buckyball.obj')
scene:add_child(rock_2)
rock_2:set_material(stone)
rock_2:translate(-12.0, -1.0, -30.0)
rock_2:scale(0.3, 0.3, 0.3)

rock_3 = gr.mesh('rock_3', './Assets/buckyball.obj')
scene:add_child(rock_3)
rock_3:set_material(stone)
rock_3:translate(15.0, -1.0, -50.0)
rock_3:scale(0.3, 0.3, 0.3)

cube_light = gr.sphere('cube_light')
scene:add_child(cube_light)
cube_light:set_material(grass)
cube_light:scale(0.07, 0.07, 0.07)
cube_light:translate(0.0, 0.0, 2.0)
cube_light:set_emission({1.0, 0.5, 0.0}, {1, 0.1, 0}, 100)

light_stand = gr.cylinder('light_stand')
scene:add_child(light_stand)
light_stand:set_material(brown_candle)
light_stand:scale(0.07, 0.4, 0.07)
light_stand:translate(0.0, -0.5, 2.0)

moop = gr.mesh('moop', './Assets/moop.obj')
scene:add_child(moop)
moop:set_material(gray_fur)
moop:scale(0.6, 0.7, 0.7)
moop:translate(-0.7, -0.15, -0.8)
moop:rotate('Y', 50)
moop:set_texture('./Assets/texture_moop.png')
moop:set_normal('./Assets/normal_moop.png')

moop_2 = gr.mesh('moop_2', './Assets/moop.obj')
scene:add_child(moop_2)
moop_2:set_material(gray_fur)
moop_2:scale(0.6, 0.7, 0.7)
moop_2:translate(0.7, -0.15, -0.8)
moop_2:rotate('Y', -50)
moop_2:set_texture('./Assets/texture_moop.png')
moop_2:set_normal('./Assets/normal_moop.png')

glass_parent_1 = gr.difference('glass_parent_1')
scene:add_child(glass_parent_1)
glass_parent_1:translate(0.7, -0.36, 1.0)

glass_bottom_1 = gr.cylinder('glass_bottom_1')
glass_parent_1:add_child(glass_bottom_1)
glass_bottom_1:set_material(glass)
glass_bottom_1:scale(0.15, 0.35, 0.15)

glass_hole_1 = gr.cylinder('glass_hole_1')
glass_parent_1:add_child(glass_hole_1)
glass_hole_1:set_material(glass)
glass_hole_1:scale(0.12, 0.5, 0.12)
glass_hole_1:translate(0.0, 0.05, 0.0)

glass_fill_1 = gr.cylinder('glass_fill_1')
scene:add_child(glass_fill_1)
glass_fill_1:set_material(wine)
glass_fill_1:scale(0.12, 0.2, 0.12)
glass_fill_1:translate(0.7, -0.31, 1.0)

glass_parent_2 = gr.difference('glass_parent_2')
scene:add_child(glass_parent_2)
glass_parent_2:translate(-0.5, -0.36, 1.0)

glass_bottom_2 = gr.cylinder('glass_bottom_2')
glass_parent_2:add_child(glass_bottom_2)
glass_bottom_2:set_material(glass)
glass_bottom_2:scale(0.15, 0.35, 0.15)

glass_hole_2 = gr.cylinder('glass_hole_2')
glass_parent_2:add_child(glass_hole_2)
glass_hole_2:set_material(glass)
glass_hole_2:scale(0.12, 0.5, 0.12)
glass_hole_2:translate(0.0, 0.05, 0.0)

glass_fill_2 = gr.cylinder('glass_fill_2')
scene:add_child(glass_fill_2)
glass_fill_2:set_material(wine)
glass_fill_2:scale(0.12, 0.15, 0.12)
glass_fill_2:translate(-0.5, -0.31, 1.0)


lights = {gr.light({0.0, 100.0, -100.0}, {0.2, 0.2, 0.3}, {1, 0.0, 0.0})}

render_metadata = {
    image_name = './Images/scene_2.png',
    image_width = 512,
    image_height = 512,
    camera_eye = {0, 2, 7},
    camera_view = {0, -0.1, -1},
    camera_up = {0, 1, 0},
    camera_fovy = 50,
    scene_ambient = {0.1, 0.1, 0.1},
    scene_lights = lights,
    enable_supersampling = false,
    thread_count = 8,
    background_image = ""
}

gr.render(scene, render_metadata)
