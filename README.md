# IDI-Rick-Morty

![image](https://user-images.githubusercontent.com/129793310/229621937-35d33512-f0d8-44ad-a78a-59e327b14b91.png)


Exercise made with openGL.
The scene contains several models: a security camera, Rick, Morty, a floor (8x8 units) and a wall (8x3).
Pressing various keys interacts with the scene:

Q/E: moves the camera left and right, respectively, 1 degree.

Click and drag: rotates the Euler angles of the camera, giving the effect of rotating the scene.

Left/Right/Up/Down: Turn Morty 45 degrees and move forward or backward (1 unit).

C: Activates a second first person viewport from the camcorder. This camera only captures green colors.

G: Paint Rick the same green as the ground, blending into the camcorder.

A: Restart the scene.

Commands to generate files:

qmake

make

Command to run:

./Exercise-2
