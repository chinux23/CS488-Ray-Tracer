#!/bin/bash
for ((i=1;i<=100;i+=1))
do
    printf -v j "%05d\n" $i
    printf -v filename "Assets/animation_%03d.lua" $i

    cp "Assets/animation.lua" $filename

    stepsize=0.1
    # float point calculation
    t=$(echo "(30-$stepsize*$i)" | bc)

    echo $t

    printf -v render "gr.render(scene, 'animation%03d.png', 1024, 1024,
	  {0, -3, %05.3f}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {white_spherical_light})" $i $t

	echo $render >> $filename

	 ./A4 $filename

	 convert -delay 10 animation*.png animation.gif
done
