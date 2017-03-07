/*
 Doodling in XY with two potentiometer inputs on an Arduino.
 An interaction prototype for a public installation.
 
 Expects serial data from an Arduino in a form like:
 160,293
 170,312
 where these are raw analogRead values from two potentiometers.
 
 Robert Zacharias, rz@rzach.me 2/22/17
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
int rows = 30; // # rows of dots
int cols = 20; // # columns of dots
int spacing = 30; // pixels between each dot and its neighbor
color[][] colorMatrix = new color[rows][cols]; // dot color container
color lit = color(255,255,0);
color unlit = color(49,49,49);

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
  int xpos = int(x*((rows*spacing)/1024.0)); // need to do float math or the division truncates to 0
  int ypos = int(y*((cols*spacing)/1024.0));

  if (mouse) {
    xpos = mouseX;
    ypos = mouseY;
  }

  // toggle by typing 't'
  if (!trace) background(0);

  if (dots) {

    // dot color matrix monitors position
    // draw and fill the dots
    // make the dots!
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        // if in a dot's zone, change that dot's color
        if ( ((abs(((i+1)*spacing) - xpos)) < (spacing/2)) && ((abs(((j+1)*spacing) - ypos)) < (spacing/2)) ) {
          colorMatrix[i][j] = lit;
        }
        color thisDot = colorMatrix[i][j];
        fill(thisDot);
        ellipse((i+1)*spacing, (j+1)*spacing, 25, 25);
      }
    }
  }

  fill(255);
  ellipse(xpos, ypos, 10, 10);

  if (statusText) {
    translate(width - 100, 10);
    fill(0);
    rect(0, 0, 100, 100);
    fill(255);
    String status = "trace " + trace + "\ndots " + dots + "\nmouse " + mouse;
    text(status, 5, 10);
  }
}


void keyPressed() {
  if (key == 't') trace = !trace;
  if (key == 'd') dots = !dots;
  if (key == 'm') mouse = !mouse;
  if (key == 's') statusText = !statusText;
  // reset dot colors
  if (key == 'r') {
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