#ifndef CUBEL_H
#define CUBEL_H

#include <QGLWidget>
#include <QString>
class Cubel
{
public:
	bool selected;// element is selected (white)?

	Cubel();
	~Cubel();

	void reset(); // return colors of faces to initial state

	// Rotation of faces
	void rotateup();	// y ÖáÓÒÊÖÂİĞı 
	void rotatefd();	// z ÖáÓÒÊÖÂİĞı 
	void rotatesd();	// x ÖáÓÒÊÖÂİĞı 
	void rotateupcw();	// y Öá×óÊÖÂİĞı
	void rotatefdcw();	// z Öá×óÊÖÂİĞı
	void rotatesdcw();	// x Öá×óÊÖÂİĞı

	// Draw
	void draw();

	QString tostring();						//something like serialization
	void fromstring(QString s);
private:
	int top,bottom,left,right,front,back;	// colors of faces
	bool viewChamfer;						// chamfers is visible?
	double chamfer;							// size of chamfer
	bool viewFaces;							// faces is visible?
	double a;								// half size of face
	double space;							// scale coefficient of cube
	void setColor(int num);
};

#endif // CUBEL_H
