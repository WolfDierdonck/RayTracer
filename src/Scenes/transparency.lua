gray_fur = gr.material({0.5, 0.5, 0.5}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
white_fur = gr.material({1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
black_fur = gr.material({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
grass = gr.material({0.1, 0.5, 0.2}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
picnic_blanket_red = gr.material({0.8, 0.2, 0.2}, {0.5, 0.5, 0.5}, 10, 0, 0.0)
brown_candle = gr.material({0.5, 0.2, 0.0}, {0.5, 0.5, 0.5}, 10, 0, 0.0)
leaf_green = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
stone = gr.material({0.8, 0.7, 0.7}, {0.0, 0.0, 0.0}, 0, 0, 0.0)
glass = gr.material({0.9, 1.0, 0.98}, {1.0, 1.0, 1.0}, 125, 0.08, 0.84)

function create_seal(name_suffix)
    seal_node = gr.node('seal_node' .. name_suffix)
    seal_node:translate(0.0, 0.0, 0.0)

    body = gr.sphere('seal_body' .. name_suffix)
    seal_node:add_child(body)
    body:set_material(gray_fur)
    body:scale(0.5, 0.5, 0.8)

    head = gr.sphere('seal_head' .. name_suffix)
    seal_node:add_child(head)
    head:set_material(gray_fur)
    head:scale(0.5, 0.6, 0.5)
    head:translate(0.0, 0.0, 0.5)

    left_eye = gr.sphere('seal_left_eye' .. name_suffix)
    seal_node:add_child(left_eye)
    left_eye:set_material(black_fur)
    left_eye:scale(0.05, 0.05, 0.05)
    left_eye:translate(0.15, 0.30, 0.87)

    right_eye = gr.sphere('seal_right_eye' .. name_suffix)
    seal_node:add_child(right_eye)
    right_eye:set_material(black_fur)
    right_eye:scale(0.05, 0.05, 0.05)
    right_eye:translate(-0.15, 0.30, 0.87)

    nose = gr.sphere('seal_nose' .. name_suffix)
    seal_node:add_child(nose)
    nose:set_material(white_fur)
    nose:scale(0.2, 0.2, 0.15)
    nose:translate(0.0, 0.0, 1.0)

    nose_front = gr.sphere('seal_nose_front' .. name_suffix)
    seal_node:add_child(nose_front)
    nose_front:set_material(black_fur)
    nose_front:scale(0.03, 0.03, 0.03)
    nose_front:translate(0.0, 0.1, 1.15)

    left_hand = gr.sphere('seal_left_hand' .. name_suffix)
    seal_node:add_child(left_hand)
    left_hand:set_material(gray_fur)
    left_hand:scale(0.2, 0.1, 0.4)
    left_hand:translate(0.5, -0.2, 0.0)

    right_hand = gr.sphere('seal_right_hand' .. name_suffix)
    seal_node:add_child(right_hand)
    right_hand:set_material(gray_fur)
    right_hand:scale(0.2, 0.1, 0.4)
    right_hand:translate(-0.5, -0.2, 0.0)

    left_leg = gr.sphere('seal_left_leg' .. name_suffix)
    seal_node:add_child(left_leg)
    left_leg:set_material(gray_fur)
    left_leg:scale(0.2, 0.2, 0.4)
    left_leg:rotate('Z', 30)
    left_leg:translate(0.3, -0.2, -0.6)

    right_leg = gr.sphere('seal_right_leg' .. name_suffix)
    seal_node:add_child(right_leg)
    right_leg:set_material(gray_fur)
    right_leg:scale(0.2, 0.2, 0.4)
    right_leg:rotate('Z', -30)
    right_leg:translate(-0.3, -0.2, -0.6)

   return seal_node
end

scene = gr.node('scene')

seal_1_parent = gr.node('seal_1_parent')
seal_1_parent:translate(-1.3, 0.0, 0.0)
scene:add_child(seal_1_parent)

seal_1 = create_seal('_1')
seal_1_parent:add_child(seal_1)
seal_1:rotate('Y', 60)

seal_2_parent = gr.node('seal_2_parent')
seal_2_parent:translate(1.3, 0.0, 0.0)
scene:add_child(seal_2_parent)

seal_2 = create_seal('_2')
seal_2_parent:add_child(seal_2)
seal_2:rotate('Y', -60)

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

    leaves = gr.sphere('leaves' .. name_suffix)
    tree_node:add_child(leaves)
    leaves:set_material(leaf_green)
    leaves:translate(0.0, 2.0, 0)
    leaves:scale(1.5, 1.5, 1.5)

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

glass_panel = gr.cube('glass_panel')
scene:add_child(glass_panel)
glass_panel:set_material(glass)
glass_panel:scale(5, 5, 0.1)
glass_panel:translate(-2.5, -1.9, -2.5)


lights = {gr.light({0.0, 6.0, -5.5}, {1.0, 0.5, 0.0}, {1, 0.2, 0.0005})}

render_metadata = {
    image_name = './Images/transparency.png',
    image_width = 512,
    image_height = 512,
    camera_eye = {0, 2, 7},
    camera_view = {0, -0.1, -1},
    camera_up = {0, 1, 0},
    camera_fovy = 50,
    scene_ambient = {0.4, 0.4, 0.6},
    scene_lights = lights,
    enable_supersampling = false,
    thread_count = 8,
    background_image = ""
}

gr.render(scene, render_metadata)
