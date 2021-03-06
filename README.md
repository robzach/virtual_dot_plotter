# Two physical knobs to drive one drawing on a computer screen

simple Arduino + Processing system to drive an Etch-a-Sketch like screen interaction from two potentiometers

Use two potentiometers (i.e. volume knobs) to operate an Etch-a-sketch like simple drawing program on a computer screen.

The pictures drawn look a bit like this beautiful lemon:

![lemon](https://raw.githubusercontent.com/robzach/virtual_dot_plotter/master/dots_image.png)

The Arduino schematic is below. Note that the red dotted line between A0 and A1 is an error; there shouldn't be any wire connecting them.

![Arduino schematic](https://github.com/robzach/virtual_dot_plotter/blob/master/x%20y%20plotter%20arduino%20to%20processing_schem.png)

And the less-useful breadboard image is here:

![Arduino breadboard](https://github.com/robzach/virtual_dot_plotter/blob/master/x%20y%20plotter%20arduino%20to%20processing_bb.png)

(The capacitors help smooth out rapid movements of the potentiometers, making the interface more fluid.)
