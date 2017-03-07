/*
 Doodling in XY with two potentiometer inputs on an Arduino.
 An interaction prototype for a public installation.
 See <rzach.me/blog/http://rzach.me/blog/thesis/research/bus%20stop%20machine/2017/02/21/Low-fi-drawing-prototype.html>
 for context.
 
 Expects serial data from an Arduino in a form like:
 160,293
 170,312
 where these are raw analogRead values from two potentiometers.
 
 2/22/17 * initial code
 3/7/17 * correct rows and cols switcheroo
 * keep dot properly bounded inside the board
 * added autosize
 
 
 Robert Zacharias, rz@rzach.me 
 released by the author to the public domain
 */

import processing.serial.*;

Serial myPort;
int x, y;
boolean trace = false;
boolean dots = true;
boolean mouse = false;
boolean serial = true;
boolean statusText = true;
boolean autosize = true; // set true to define cols and rows, and dots will fill width of screen
int cols = 30; // # columns of dots
int rows = 20; // # rows of dots
int diameter = 30; // pixels of circle diameter (if not using autosize)
int spacing = 35; // pixels between each dot and its neighbor, center to center (if not using autosize)
color[][] colorMatrix = new color[rows][cols]; // dot color container
color lit = color(255, 255, 0);
color unlit = color(49, 49, 49);

void setup() 
{
  size(1200, 800);

  if (serial) {
    //diagnostic to list all ports
    for (int i = 0; i < Serial.list().length; i++) {
      println("Serial.list()[", i, "] = ", Serial.list()[i]);
    }
    String portName = Serial.list()[5]; // may have to change this number later
    myPort = new Serial(this, portName, 9600);
  }

  if (autosize) {
    diameter = int((width*0.75) / cols);
    spacing = int(diameter*1.2);
  }

  fill(255);
  background(0);
  noStroke();

  // preload dots with yellow
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      colorMatrix[i][j] = unlit;
    }
  }
}

void draw()
{
  // if using Arduino data to move dot
  int xpos = int(map(x, 0, 1023, spacing, cols*spacing));
  int ypos = int(map(y, 0, 1023, spacing, rows*spacing));

  if (mouse) { // if using mouse to move dot
    xpos = mouseX;
    ypos = mouseY;
  }

  // toggle by typing 't'
  if (!trace) background(0);

  if (dots) {
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < rows; j++) {
        // if in a dot's zone, change that dot's color
        if ( ((abs(((i+1)*spacing) - xpos)) < (spacing/2)) && ((abs(((j+1)*spacing) - ypos)) < (spacing/2)) ) {
          colorMatrix[j][i] = lit;
        }
        color thisDot = colorMatrix[j][i];
        fill(thisDot);
        ellipse((i+1)*spacing, (j+1)*spacing, diameter, diameter);
      }
    }
  }

  fill(255);
  ellipse(xpos, ypos, 10, 10); // cursor position

  if (statusText) {
    translate(width - 100, 10);
    fill(0);
    rect(0, 0, 100, 100);
    fill(255);
    String status = "cols " + cols + "\nrows " + rows + "\ntrace " + trace +
    "\ndots " + dots + "\nmouse " + mouse + "\nr to reset dots";
    text(status, 5, 10);
  }
}


void keyPressed() {
  if (key == 't') trace = !trace;
  if (key == 'd') dots = !dots;
  if (key == 'm') mouse = !mouse;
  if (key == 's') statusText = !statusText;
  if (key == 'r') {  // reset dot colors
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        colorMatrix[i][j] = unlit;
      }
    }
  }
}


// from http://www.interactiondesign.se/wiki/courses:intro.prototyping.spring.2015.jan19_20_21
// though I had to change the second line to use readStringUntil() to make it actually work
void serialEvent(Serial myPort) {
  String inString = myPort.readStringUntil(10);
  if (inString != null) {
    inString = trim(inString);
    String values [] = split(inString, ',');
    if (values.length>1) {
      x = int(values[0]);
      y = int(values[1]);
    }
  }
}