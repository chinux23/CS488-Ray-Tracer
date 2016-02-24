-- a3mark.lua
-- A very simple scene creating a trivial hierarchical puppet.
-- We'll use this scene as part of testing your assignment.
-- See a3mark.png for an example of what this file produces.

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

torso = gr.mesh('cube','torso')
rootnode:add_child(torso)
torso:set_material(white)

left_shoulder = gr.mesh('sphere', 'left_shoulder')
torso:add_child(left_shoulder)
left_shoulder:scale(0.2, 0.2, 0.2)
left_shoulder:translate(-0.5, 0.3, 0.3)
left_shoulder:set_material(blue)

right_shoulder = gr.mesh('sphere', 'right_shoulder')
torso:add_child(right_shoulder)
right_shoulder:scale(0.2, 0.2, 0.2)
right_shoulder:translate(0.5, 0.3, 0.3)
right_shoulder:set_material(blue)

leftShoulderJoint = gr.joint('leftShoulderJoint', {-30, 0, 30}, {0, 0, 0})
left_shoulder:add_child(leftShoulderJoint)

rightShoulderJoint = gr.joint('rightShoulderJoint', {-30, 0, 30}, {0, 0, 0})
right_shoulder:add_child(rightShoulderJoint)

right_arm = gr.mesh('cube','right_arm')
rightShoulderJoint:add_child(right_arm)
right_arm:translate(1.0, -0.5, 0.0)
right_arm:scale(1, 5, 1)
right_arm:set_material(red)

left_arm = gr.mesh('cube','left_arm')
leftShoulderJoint:add_child(left_arm)
left_arm:translate(-1.0, -0.5, 0.0)
left_arm:scale(1, 5, 1)
left_arm:set_material(green)

-- s2 = gr.mesh('sphere','s2')
-- j1:add_child(s2)
-- s2:scale(1.0, 1.0, 1.0)
-- -- s2:rotate('z', 90.0)
-- s2:translate(0.0, -1.0, 0.0)
-- s2:set_material(blue)

-- s3 = gr.mesh('sphere','s3')
-- s2:add_child(s3)
-- s3:scale(1.0, 0.1, 2.0)
-- s3:translate(0.0, -2.0, 2.0)
-- s3:set_material(green)

rootnode:translate(0.0, 0.0, -10.0)

return rootnode
