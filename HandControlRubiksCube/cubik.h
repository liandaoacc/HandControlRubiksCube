#ifndef CUBIK_H
#define CUBIK_H

#include "cubel.h"

enum axis {UP,SD,FD};	// axes of rotation - up, side , forward

class Cubik
{
public:
	Cubik();
	~Cubik();
	
	void selax(axis a);		// change selected axis
	void selh(int dif);		// change selected layer
	void resetchanges();	// assembles cubic into start position
	void draw();			// draws cube layers along selected axis
	void drawscanning();	// draws scanning of text
	void startanimation();	// sets initial animation values
	void animationframe();	// change state of cubic for next frame
	void randomize();
	QString tostring();
	void fromstring(QString s);
private:
	Cubel body[3][3][3];	// elements of cube
	int selecth;			// selected layer
	axis selectax;			// selected axis of rotation
	double selectangle;		// angle of selected layer (for animation)
	int animation;			// while animation is on, we can't make any another animation (rotation)

	void reselectbody();	// when we change axis or layer cubels must change colors of chamfers
	void drawuplayer(int h);// we must have functions for drawing one selected layer normal to every axis
	void drawsdlayer(int h);
	void drawfdlayer(int h);
	void rotateup();		// rotation of selected layer around vertical axis (y)
	void rotatefd();		// rotation of selected layer around fordward axis (z)
	void rotatesd();		// rotation of selected layer around side axis (x)
};

#endif // CUBIK_H
