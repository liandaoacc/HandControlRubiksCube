#include "cubel.h"

#define NOR glNormal3d
#define VER glVertex3d

Cubel::Cubel()
{
	selected=0;
	top=1;
	bottom=2;
	left=3;
	right=4;
	front=5;
	back=6;
	viewChamfer=true;
	chamfer=0.2;
	viewFaces=true;
	a=1-chamfer;
	space=0.9;
}

Cubel::~Cubel()
{

}

void Cubel::reset() // return colors of faces to initial state
{
	top=1;
	bottom=2;
	left=3;
	right=4;
	front=5;
	back=6;
}

void Cubel::rotateup()
{
	int tmp=front;front=left;left=back;back=right;right=tmp;
}
void Cubel::rotatefd()
{
	int tmp=left;left=top;top=right;right=bottom;bottom=tmp;
}
void Cubel::rotatesd()
{
	int tmp=front;front=top;top=back;back=bottom;bottom=tmp;
}
void Cubel::rotateupcw()
{
	int tmp=right;right=back;back=left;left=front;front=tmp;
}
void Cubel::rotatefdcw()
{
	int tmp=bottom;bottom=right;right=top;top=left;left=tmp;
}
void Cubel::rotatesdcw()
{
	int tmp=bottom;bottom=back;back=top;top=front;front=tmp;
}

void Cubel::setColor( int num )
{
	const float colors[8][3]={
		{0.1,0.1,0.1}, 
		{1,1,0},	// top,黄色
		{1,0,0},	// bottom,红色
		{0,1,0},	// left,绿色
		{0,0,1},	// right,蓝色
		{1,0,1},	// front,紫色
		{0,1,1},	// back,青色
		{0.9,0.9,0.9}
	};
	glColor3f(colors[num][0],colors[num][1],colors[num][2]);
}

void Cubel::draw() // drawing
{
	glPushMatrix(); // we will scale our cube, so must save model matrix
	glScaled(space,space,space); // just for fun we make space between cubels

	if(viewChamfer) // draw chamfers
	{
		if(selected) 
			setColor(7); // set color
		else 
			setColor(0);

		glBegin(GL_TRIANGLES); // draw corner triangles
		NOR(1,1,-1);
		VER(a,a,-1);
		VER(1,a,-a);
		VER(a,1,-a);

		NOR(1,1,1);
		VER(a,a,1);
		VER(1,a,a);
		VER(a,1,a);

		NOR(-1,1,-1);
		VER(-a,a,-1);
		VER(-1,a,-a);
		VER(-a,1,-a);

		NOR(1,-1,-1);
		VER(a,-a,-1);
		VER(1,-a,-a);
		VER(a,-1,-a);
		
		NOR(-1,-1,1);
		VER(-a,-a,1);
		VER(-1,-a,a);
		VER(-a,-1,a);

		NOR(1,-1,1);
		VER(a,-a,1);
		VER(1,-a,a);
		VER(a,-1,a);

		NOR(-1,1,1);
		VER(-a,a,1);
		VER(-1,a,a);
		VER(-a,1,a);

		NOR(-1,-1,-1);
		VER(-a,-a,-1);
		VER(-1,-a,-a);
		VER(-a,-1,-a);
		glEnd();
	}

	glBegin(GL_QUADS);
	if(viewChamfer)
	{
		// chamfer ring 1
		NOR(1,0,-1);
		VER(a,a,-1);
		VER(a,-a,-1);
		VER(1,-a,-a);
		VER(1,a,-a);

		NOR(1,0,1);
		VER(a,a,1);
		VER(a,-a,1);
		VER(1,-a,a);
		VER(1,a,a);

		NOR(-1,0,-1);
		VER(-a,a,-1);
		VER(-a,-a,-1);
		VER(-1,-a,-a);
		VER(-1,a,-a);

		NOR(-1,0,1);
		VER(-a,a,1);
		VER(-a,-a,1);
		VER(-1,-a,a);
		VER(-1,a,a);

		// chamfer ring 2
		NOR(0,1,-1);
		VER(a,a,-1);
		VER(-a,a,-1);
		VER(-a,1,-a);
		VER(a,1,-a);

		NOR(0,1,1);
		VER(a,a,1);
		VER(-a,a,1);
		VER(-a,1,a);
		VER(a,1,a);

		NOR(0,-1,-1);
		VER(a,-a,-1);
		VER(-a,-a,-1);
		VER(-a,-1,-a);
		VER(a,-1,-a);

		NOR(0,-1,1);
		VER(a,-a,1);
		VER(-a,-a,1);
		VER(-a,-1,a);
		VER(a,-1,a);

		// chamfer ring 3
		NOR(-1,1,0);
		VER(-1,a,a);
		VER(-1,a,-a);
		VER(-a,1,-a);
		VER(-a,1,a);

		NOR(1,1,0);
		VER(1,a,a);
		VER(1,a,-a);
		VER(a,1,-a);
		VER(a,1,a);

		NOR(-1,-1,0);
		VER(-1,-a,a);
		VER(-1,-a,-a);
		VER(-a,-1,-a);
		VER(-a,-1,a);

		NOR(1,-1,0);
		VER(1,-a,a);
		VER(1,-a,-a);
		VER(a,-1,-a);
		VER(a,-1,a);
	}

	// faces
	if(viewFaces)
	{
		setColor(front);
		NOR(0,0,-1); // front
		VER(a,a,-1);
		VER(a,-a,-1);
		VER(-a,-a,-1);
		VER(-a,a,-1);

		setColor(back);
		NOR(0,0,1); // back
		VER(a,a,1);
		VER(a,-a,1);
		VER(-a,-a,1);
		VER(-a,a,1);

		setColor(right);
		NOR(1,0,0); //right
		VER(1,a,a);
		VER(1,a,-a);
		VER(1,-a,-a);
		VER(1,-a,a);

		setColor(left);
		NOR(-1,0,0); //left
		VER(-1,a,a);
		VER(-1,a,-a);
		VER(-1,-a,-a);
		VER(-1,-a,a);

		setColor(top);
		NOR(0,1,0); //up
		VER(-a,1,-a);
		VER(a,1,-a);
		VER(a,1,a);
		VER(-a,1,a);

		setColor(bottom);
		NOR(0,-1,0); //down
		VER(-a,-1,-a);
		VER(a,-1,-a);
		VER(a,-1,a);
		VER(-a,-1,a);
	}
	glEnd();
	glPopMatrix(); // restore all changes of position

}

QString Cubel::tostring()
{
	QString res;
	if(selected) res="+";
	else res="-";
	res+=QString::number(top);
	res+=QString::number(bottom);
	res+=QString::number(left);
	res+=QString::number(right);
	res+=QString::number(front);
	res+=QString::number(back);
	return res;
}

void Cubel::fromstring(QString s)
{
	selected=s[0]=='+';
	top=s[1].digitValue();
	bottom=s[2].digitValue();
	left=s[3].digitValue();
	right=s[4].digitValue();
	front=s[5].digitValue();
	back=s[6].digitValue();
}