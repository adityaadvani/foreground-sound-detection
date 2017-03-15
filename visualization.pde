/**  
 * @version: 
 *     $Id: visualization.pde , Version 1.0 05/11/2016 $ 
 *	   $Environment: Processing 3: Processiing Source Code
 * Revisions: 
 *     $Log Initial Version $ 
 */

import processing.serial.*;
import org.gicentre.utils.spatial.*;
import org.gicentre.utils.network.traer.physics.*;
import org.gicentre.utils.gui.*;
import org.gicentre.utils.network.traer.animation.*;
import org.gicentre.utils.geom.*;
import org.gicentre.utils.colour.*;
import org.gicentre.utils.move.*;
import org.gicentre.utils.stat.*;
import org.gicentre.utils.multisketch.*;
import org.gicentre.utils.io.*;
import org.gicentre.utils.*;
import org.gicentre.utils.text.*;
import org.gicentre.utils.network.*;
import org.gicentre.utils.stat.*;    
import java.util.Arrays;

/**
 * Program for visualization and collection of sound sensor data reflecting changes in the frequency domain.
 * The program represents the moving average values of 64 frequency bins extracted from the sound 
 * samplings from Arduino sound sensor.
 * This program works in parallel with the Arduino sensor data collection that senses
 * sound signals by sampling and applying the moving average algorithm.
 * The event detection is handled by Arduino's hardware and doesn't depend on this program.
 * Input to the program is the Arduino's serial output data stream.
 * The library from http://www.gicentre.net/utils/ was used to generate the graphical results.
 
 * @author Aditya Advani
 * @author Raj Korpally
 */

Serial serialInputPort; //Defining serial variable to read the s
PrintWriter output; //Variavle used to perform external write operations
float brightness = 255; //Reading 255 implies white color
BarChart barChart; //BarChart Declaration
float []readings = new float[65]; //Variable to store the frequency bins data
float []readingsSplit = new float[65]; //Variable to store split frequency bins data

//The setup function initializes the environment and is executed only once.

void setup()
{
  size(600, 500); // Size of the output canvas

  //Serial port to read, baud rate should match the Arduino's serial output rate. 
  serialInputPort = new Serial(this, "COM5", 115200); 
  serialInputPort.bufferUntil('\n');

  //Creating data logs files
  output = createWriter("SoundSense.csv");
  output.println("SoundSense");

  //Bar Chart initializiations 
  barChart = new BarChart(this);
  barChart.setData(readings);
  // Axis scaling
  barChart.setMinValue(0);
  barChart.setMaxValue(140);
  barChart.showValueAxis(true);
  barChart.setValueAxisLabel("Amplitude");
  barChart.setCategoryAxisLabel("Frequency");
  barChart.setBarColour(color(220, 80, 80, 200));
  barChart.showCategoryAxis(false);
}

/**
 * The draw function performs the visualization be reading the updated data continuously
 * that is sent from the Arduino. It parses the data into desirable format and extracts
 * the frequency data in a useable format.
 */

void draw()
{
  //background(0, 0, brightness);
  String readData = serialInputPort.readStringUntil('\n');
  String sense = null;

  if (readData!=null) {   
    String[] tempList = split(readData, ',');
    if (tempList.length<=65) {
      int i=0;
      for (String temp : tempList) {
        try {
          readings[i] = Float.parseFloat(temp);
          output.print(readData);
          i++;
        }
        catch(NumberFormatException ex) {
        }
      }
    }
  }

  background(255); // White Background
  barChart.draw(15, 15, width-30, height-30); //Draw visualization


  if (readData!=null)
  {  //Test Print output
    println(readData);
  }
}


void serialEvent(Serial serialInputPort)
{
  //For any change in serial event output you can add addition code here to execute it.
}

// Detects any key input pressed on the visualization canvas.
void keyPressed() {
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  exit(); // Stops the program
}