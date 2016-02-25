-- mypuppet.lua
-- A simplified puppet with posable joints, but that
-- looks roughly humanoid.

rootnode = gr.node('root')
rootnode:rotate('y', -20.0)
rootnode:scale( 0.25, 0.25, 0.25 )
rootnode:translate(0.0, 0.0, -2.0)

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

torso = gr.mesh('cube', 'torso')
rootnode:add_child(torso)
torso:set_material(red)
torso:scale(1, 1, 0.5)

shoulder = gr.mesh('cube', 'shoulder')
torso:add_child(shoulder)
shoulder:set_material(red)
shoulder:scale(1.0, 1.0, 1/0.5)
shoulder:scale(2, 0.2, 0.5)
shoulder:translate(0, 0.6, 0)

shoulderDummy = gr.mesh('cube', 'shoulderDummy')
shoulder:add_child(shoulderDummy)
shoulderDummy:set_material(blue)
shoulderDummy:scale(1.0/2, 1.0/0.2, 1.0/0.5)
shoulderDummy:scale(0.2, 0.2, 0.2)
shoulderDummy:translate(0, 0.8, 0)

shoulderjoint = gr.joint('neckjoint', {0, 0, 0}, {-90, 0, 90})
shoulderDummy:add_child(shoulderjoint)

neck = gr.mesh('sphere', 'neck')
shoulderjoint:add_child(neck)
neck:scale(1/0.2, 1/0.2, 1/0.2)
neck:scale(0.1, 0.1, 0.1)
neck:translate(0.0, 1, 0.0)
neck:set_material(blue)

headjoint = gr.joint('headjoint', {-30, 0, 30}, {0, 0, 0})
neck:add_child(headjoint)

head = gr.mesh('cube', 'head')
headjoint:add_child(head)
-- head:scale(1/0.15, 1/0.3, 1/0.15)
head:scale(1/0.1, 1/0.1, 1/0.1)
head:scale(0.4, 0.4, 0.4)
head:translate(0.0, 2.5, 0.0)
head:set_material(white)

leftArmShoulderSphere = gr.mesh('sphere', 'leftArmShoulderSphere')
torso:add_child(leftArmShoulderSphere)
leftArmShoulderSphere:scale(1, 1, 1/0.5)
leftArmShoulderSphere:scale(0.2, 0.2, 0.2)
leftArmShoulderSphere:set_material(blue)
leftArmShoulderSphere:translate(-1, 0.6, 0)

leftArmUpperJoint = gr.joint('leftArmUpperJoint', {-45, 0, 45}, {0, 0, 0})
leftArmShoulderSphere:add_child(leftArmUpperJoint)

leftUpperArm = gr.mesh('cube', 'leftUpperArm')
leftArmUpperJoint:add_child(leftUpperArm)
leftUpperArm:scale(1/0.2, 1/0.2, 1/0.2)
leftUpperArm:scale(0.1, 0.5, 0.1)
leftUpperArm:set_material(green)
leftUpperArm:translate(0, -1.5, 0)

leftArmElbowSphere = gr.mesh('sphere', 'leftArmElbowSphere')
leftUpperArm:add_child(leftArmElbowSphere)
leftArmElbowSphere:scale(1/0.1, 1/0.5, 1/0.1)
leftArmElbowSphere:scale(0.2, 0.2, 0.2)
leftArmElbowSphere:set_material(blue)
leftArmElbowSphere:translate(0, -0.5, 0)

leftArmElbowJoint = gr.joint('leftArmElbowJoint', {-45, 0, 45}, {0, 0, 0})
leftArmElbowSphere:add_child(leftArmElbowJoint)

leftLowerArm = gr.mesh('cube', 'leftLowerArm')
leftArmElbowJoint:add_child(leftLowerArm)
leftLowerArm:scale(1/0.2, 1/0.2, 1/0.2)
leftLowerArm:scale(0.1, 0.5, 0.1)
leftLowerArm:set_material(green)
leftLowerArm:translate(0, -1, 0)

leftArmHandSphere = gr.mesh('sphere', 'leftArmHandSphere')
leftLowerArm:add_child(leftArmHandSphere)
leftArmHandSphere:scale(1/0.1, 1/0.5, 1/0.1)
leftArmHandSphere:scale(0.2, 0.2, 0.2)
leftArmHandSphere:set_material(blue)
leftArmHandSphere:translate(0, -0.8, 0)

leftHandJoint = gr.joint('leftHandJoint', {-45, 0, 45}, {0, 0, 0})
leftArmHandSphere:add_child(leftHandJoint)

leftHand = gr.mesh('cube', 'leftHand')
leftHandJoint:add_child(leftHand)
leftHand:scale(1/0.2, 1/0.2, 1/0.2)
leftHand:scale(0.3, 0.3, 0.1)
leftHand:set_material(green)
leftHand:translate(0, -1, 0)




----------------- right arm ---------------

rightArmShoulderSphere = gr.mesh('sphere', 'rightArmShoulderSphere')
torso:add_child(rightArmShoulderSphere)
rightArmShoulderSphere:scale(1, 1, 1/0.5)
rightArmShoulderSphere:scale(0.2, 0.2, 0.2)
rightArmShoulderSphere:set_material(blue)
rightArmShoulderSphere:translate(1, 0.6, 0)

rightArmUpperJoint = gr.joint('rightArmUpperJoint', {-45, 0, 45}, {0, 0, 0})
rightArmShoulderSphere:add_child(rightArmUpperJoint)

rightUpperArm = gr.mesh('cube', 'rightUpperArm')
rightArmUpperJoint:add_child(rightUpperArm)
rightUpperArm:scale(1/0.2, 1/0.2, 1/0.2)
rightUpperArm:scale(0.1, 0.5, 0.1)
rightUpperArm:set_material(green)
rightUpperArm:translate(0, -1.5, 0)

rightArmElbowSphere = gr.mesh('sphere', 'rightArmElbowSphere')
rightUpperArm:add_child(rightArmElbowSphere)
rightArmElbowSphere:scale(1/0.1, 1/0.5, 1/0.1)
rightArmElbowSphere:scale(0.2, 0.2, 0.2)
rightArmElbowSphere:set_material(blue)
rightArmElbowSphere:translate(0, -0.5, 0)

rightArmElbowJoint = gr.joint('rightArmElbowJoint', {-45, 0, 45}, {0, 0, 0})
rightArmElbowSphere:add_child(rightArmElbowJoint)

rightLowerArm = gr.mesh('cube', 'rightLowerArm')
rightArmElbowJoint:add_child(rightLowerArm)
rightLowerArm:scale(1/0.2, 1/0.2, 1/0.2)
rightLowerArm:scale(0.1, 0.5, 0.1)
rightLowerArm:set_material(green)
rightLowerArm:translate(0, -1, 0)

rightArmHandSphere = gr.mesh('sphere', 'rightArmHandSphere')
rightLowerArm:add_child(rightArmHandSphere)
rightArmHandSphere:scale(1/0.1, 1/0.5, 1/0.1)
rightArmHandSphere:scale(0.2, 0.2, 0.2)
rightArmHandSphere:set_material(blue)
rightArmHandSphere:translate(0, -0.8, 0)

rightHandJoint = gr.joint('rightHandJoint', {-45, 0, 45}, {0, 0, 0})
rightArmHandSphere:add_child(rightHandJoint)

rightHand = gr.mesh('cube', 'rightHand')
rightHandJoint:add_child(rightHand)
rightHand:scale(1/0.2, 1/0.2, 1/0.2)
rightHand:scale(0.3, 0.3, 0.1)
rightHand:set_material(green)
rightHand:translate(0, -1, 0)


------------------------ hip ----------------------

hip = gr.mesh('cube', 'hip')
torso:add_child(hip)
hip:set_material(white)
hip:scale(1.0, 1.0, 1/0.5)
hip:scale(1, 0.2, 1)
hip:translate(0, -0.5, 0)

------------------------ left legs ---------------------

leftHipSphere = gr.mesh('sphere', 'leftHipSphere')
torso:add_child(leftHipSphere)
leftHipSphere:scale(1, 1, 1/0.5)
leftHipSphere:scale(0.2, 0.2, 0.2)
leftHipSphere:set_material(blue)
leftHipSphere:translate(-0.3, -0.65, 0)

leftLegUpperJoint = gr.joint('leftLegUpperJoint', {-45, 0, 45}, {0, 0, 0})
leftHipSphere:add_child(leftLegUpperJoint)

leftUpperLeg = gr.mesh('cube', 'leftUpperLeg')
leftLegUpperJoint:add_child(leftUpperLeg)
leftUpperLeg:scale(1/0.2, 1/0.2, 1/0.2)
leftUpperLeg:scale(0.1, 0.5, 0.1)
leftUpperLeg:set_material(green)
leftUpperLeg:translate(0, -1.5, 0)

leftLegElbowSphere = gr.mesh('sphere', 'leftLegElbowSphere')
leftUpperLeg:add_child(leftLegElbowSphere)
leftLegElbowSphere:scale(1/0.1, 1/0.5, 1/0.1)
leftLegElbowSphere:scale(0.2, 0.2, 0.2)
leftLegElbowSphere:set_material(blue)
leftLegElbowSphere:translate(0, -0.5, 0)

leftLegElbowJoint = gr.joint('leftLegElbowJoint', {-45, 0, 45}, {0, 0, 0})
leftLegElbowSphere:add_child(leftLegElbowJoint)

leftLowerLeg = gr.mesh('cube', 'leftLowerLeg')
leftLegElbowJoint:add_child(leftLowerLeg)
leftLowerLeg:scale(1/0.2, 1/0.2, 1/0.2)
leftLowerLeg:scale(0.1, 0.5, 0.1)
leftLowerLeg:set_material(green)
leftLowerLeg:translate(0, -1, 0)

leftFootSphere = gr.mesh('sphere', 'leftFootSphere')
leftLowerLeg:add_child(leftFootSphere)
leftFootSphere:scale(1/0.1, 1/0.5, 1/0.1)
leftFootSphere:scale(0.2, 0.2, 0.2)
leftFootSphere:set_material(blue)
leftFootSphere:translate(0, -0.8, 0)


leftFootJoint = gr.joint('leftFootJoint', {-45, 0, 45}, {0, 0, 0})
leftFootSphere:add_child(leftFootJoint)

leftFoot = gr.mesh('cube', 'leftFoot')
leftFootJoint:add_child(leftFoot)
leftFoot:scale(1/0.2, 1/0.2, 1/0.2)
leftFoot:scale(0.3, 0.3, 0.1)
leftFoot:set_material(green)
leftFoot:translate(0, -1, 0)



------------------------ right legs ---------------------
rightHipSphere = gr.mesh('sphere', 'rightHipSphere')
torso:add_child(rightHipSphere)
rightHipSphere:scale(1, 1, 1/0.5)
rightHipSphere:scale(0.2, 0.2, 0.2)
rightHipSphere:set_material(blue)
rightHipSphere:translate(0.3, -0.65, 0)

rightLegUpperJoint = gr.joint('rightLegUpperJoint', {-45, 0, 45}, {0, 0, 0})
rightHipSphere:add_child(rightLegUpperJoint)

rightUpperLeg = gr.mesh('cube', 'rightUpperLeg')
rightLegUpperJoint:add_child(rightUpperLeg)
rightUpperLeg:scale(1/0.2, 1/0.2, 1/0.2)
rightUpperLeg:scale(0.1, 0.5, 0.1)
rightUpperLeg:set_material(green)
rightUpperLeg:translate(0, -1.5, 0)

rightLegElbowSphere = gr.mesh('sphere', 'rightLegElbowSphere')
rightUpperLeg:add_child(rightLegElbowSphere)
rightLegElbowSphere:scale(1/0.1, 1/0.5, 1/0.1)
rightLegElbowSphere:scale(0.2, 0.2, 0.2)
rightLegElbowSphere:set_material(blue)
rightLegElbowSphere:translate(0, -0.5, 0)

rightLegElbowJoint = gr.joint('rightLegElbowJoint', {-45, 0, 45}, {0, 0, 0})
rightLegElbowSphere:add_child(rightLegElbowJoint)

rightLowerLeg = gr.mesh('cube', 'rightLowerLeg')
rightLegElbowJoint:add_child(rightLowerLeg)
rightLowerLeg:scale(1/0.2, 1/0.2, 1/0.2)
rightLowerLeg:scale(0.1, 0.5, 0.1)
rightLowerLeg:set_material(green)
rightLowerLeg:translate(0, -1, 0)

rightFootSphere = gr.mesh('sphere', 'rightFootSphere')
rightLowerLeg:add_child(rightFootSphere)
rightFootSphere:scale(1/0.1, 1/0.5, 1/0.1)
rightFootSphere:scale(0.2, 0.2, 0.2)
rightFootSphere:set_material(blue)
rightFootSphere:translate(0, -0.8, 0)


rightFootJoint = gr.joint('rightFootJoint', {-45, 0, 45}, {0, 0, 0})
rightFootSphere:add_child(rightFootJoint)

rightFoot = gr.mesh('cube', 'rightFoot')
rightFootJoint:add_child(rightFoot)
rightFoot:scale(1/0.2, 1/0.2, 1/0.2)
rightFoot:scale(0.3, 0.3, 0.1)
rightFoot:set_material(green)
rightFoot:translate(0, -1, 0)



ears = gr.mesh('sphere', 'ears')
head:add_child(ears)
ears:scale(1.2, 0.08, 0.08)
ears:set_material(red)
ears:set_material(blue)

leftEye = gr.mesh('cube', 'leftEye')
head:add_child(leftEye)
leftEye:scale(0.2, 0.1, 0.1)
leftEye:translate(-0.2, 0.2, 0.5)
leftEye:set_material(blue)

rightEye = gr.mesh('cube', 'rightEye')
head:add_child(rightEye)
rightEye:scale(0.2, 0.1, 0.1)
rightEye:translate(0.2, 0.2, 0.5)
rightEye:set_material(blue)

-- leftShoulder = gr.mesh('sphere', 'leftShoulder')
-- torso:add_child(leftShoulder)
-- leftShoulder:scale(1/0.5,1.0,1/0.5);
-- leftShoulder:scale(0.2, 0.2, 0.2)
-- leftShoulder:translate(-0.4, 0.35, 0.0)
-- leftShoulder:set_material(blue)

-- leftShoulderJoint = gr.joint('leftShoulderJoint', {-30, 0, 30}, {0, 0, 0})
-- leftShoulder:add_child(leftShoulderJoint)

-- leftArm = gr.mesh('cube', 'leftArm')
-- leftShoulderJoint:add_child(leftArm)
-- leftArm:scale(1.0/0.2, 1.0/0.2, 1.0/0.2)
-- leftArm:scale(0.1, 0.6, 0.1)
-- -- leftArm:rotate('z', 50)
-- leftArm:translate(-0.6, -1, 0.0)
-- leftArm:set_material(red)

-- rightShoulder = gr.mesh('sphere', 'rightShoulder')
-- torso:add_child(rightShoulder)
-- rightShoulder:scale(1/0.5,1.0,1/0.5);
-- rightShoulder:scale(0.2, 0.2, 0.2)
-- rightShoulder:translate(0.4, 0.35, 0.0)
-- rightShoulder:set_material(blue)

-- rightArm = gr.mesh('cube', 'rightArm')
-- torso:add_child(rightArm)
-- rightArm:scale(1/0.5,1.0,1/0.5);
-- rightArm:scale(0.4, 0.1, 0.1)
-- rightArm:rotate('z', -50);
-- rightArm:translate(0.8, 0.0, 0.0)
-- rightArm:set_material(red)

-- leftHip = gr.mesh('sphere', 'leftHip')
-- torso:add_child(leftHip)
-- leftHip:scale(1/0.5,1.0,1/0.5);
-- leftHip:scale(0.21, 0.21, 0.21)
-- leftHip:translate(-0.38, -0.5, 0.0)
-- leftHip:set_material(blue)

-- rightHip = gr.mesh('sphere', 'rightHip')
-- torso:add_child(rightHip)
-- rightHip:scale(1/0.5,1.0,1/0.5);
-- rightHip:scale(0.21, 0.21, 0.21)
-- rightHip:translate(0.38, -0.5, 0.0)
-- rightHip:set_material(blue)

-- leftLeg = gr.mesh('cube', 'leftLeg')
-- leftHip:add_child(leftLeg)
-- leftLeg:scale(0.5,4,0.5)
-- leftLeg:translate(0,-2.8,0)
-- leftLeg:set_material(red)

-- rightLeg = gr.mesh('cube', 'rightLeg')
-- rightHip:add_child(rightLeg)
-- rightLeg:scale(0.5,4,0.5)
-- rightLeg:translate(0,-2.8,0)
-- rightLeg:set_material(red)

return rootnode
