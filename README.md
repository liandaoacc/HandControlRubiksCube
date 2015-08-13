# HandControlRubiksCube
Detect the fingertips and control 3D Rubiks cube

## Introduction
- Combine image noise processing and skin color segmentation to extract the hand region in HSV color space. 
- Use a mathematical model of fingertip detection, which employs a convex hull and convexity defects to locate the fingertips 
within the hand contour, the process is capable to locate the fingertips in real time. 
- Use the Kalman filter algorithm in order to predict the location of the fingertips. 
- Develop a 3D virtual Rubik's Cube application which allows us to control the Cube with our fingers.

## Require 
- Qt 4.8
- OpenCV 2.4.8
