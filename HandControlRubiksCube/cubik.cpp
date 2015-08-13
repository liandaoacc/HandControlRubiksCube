#include "cubik.h"

Cubik::Cubik()
{
	selecth=1;
	selectax=UP;
	selectangle=0;
	animation=0;
	reselectbody();
}

Cubik::~Cubik()
{

}

void Cubik::reselectbody()
{
	int i,j,k;
	if(selectax==SD)
	{
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					body[i][j][k].selected=(i==selecth);
	}
	else if(selectax==UP)
	{
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					body[i][j][k].selected=(j==selecth);
	}
	else if(selectax==FD)
	{
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				for(k=0;k<3;k++)
					body[i][j][k].selected=(k==selecth);
	};
}

void Cubik::drawuplayer(int h)
{
	int i,j;
	glPushMatrix();
	glTranslated(-2,(h-1)*2,-2);

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			body[j][h][i].draw();
			glTranslated(2,0,0);
		}
		glTranslated(-6,0,2);
	}
	glPopMatrix();
}

void Cubik::drawsdlayer(int h)
{
	int i,j;
	glPushMatrix();
	glTranslated((h-1)*2,-2,-2);

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			body[h][i][j].draw();
			glTranslated(0,0,2);
		}
		glTranslated(0,2,-6);
	}
	glPopMatrix();
}

void Cubik::drawfdlayer(int h)
{
	int i,j;
	glPushMatrix();
	glTranslated(-2,-2,(h-1)*2);

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			body[i][j][h].draw();
			glTranslated(0,2,0);
		}
		glTranslated(2,-6,0);
	}
	glPopMatrix();
}

void Cubik::rotateup()
{
	int i,j,h=selecth;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			body[i][h][j].rotateup();

	Cubel tmp=body[2][h][2]; //corner elements
	body[2][h][2]=body[2][h][0];
	body[2][h][0]=body[0][h][0];
	body[0][h][0]=body[0][h][2];
	body[0][h][2]=tmp;

	tmp=body[2][h][1]; // side elements
	body[2][h][1]=body[1][h][0];
	body[1][h][0]=body[0][h][1];
	body[0][h][1]=body[1][h][2];
	body[1][h][2]=tmp;
}

void Cubik::rotatefd()
{
	int i,j,h=selecth;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			body[i][j][h].rotatefd();

	Cubel tmp=body[2][2][h]; //corner elements
	body[2][2][h]=body[2][0][h];
	body[2][0][h]=body[0][0][h];
	body[0][0][h]=body[0][2][h];
	body[0][2][h]=tmp;

	tmp=body[2][1][h]; // side elements
	body[2][1][h]=body[1][0][h];
	body[1][0][h]=body[0][1][h];
	body[0][1][h]=body[1][2][h];
	body[1][2][h]=tmp;
}

void Cubik::rotatesd()
{
	int i,j,h=selecth;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			body[h][i][j].rotatesd();

	Cubel tmp=body[h][2][2]; //corner elements
	body[h][2][2]=body[h][0][2];
	body[h][0][2]=body[h][0][0];
	body[h][0][0]=body[h][2][0];
	body[h][2][0]=tmp;

	tmp=body[h][1][2]; // side elements
	body[h][1][2]=body[h][0][1];
	body[h][0][1]=body[h][1][0];
	body[h][1][0]=body[h][2][1];
	body[h][2][1]=tmp;
}

void Cubik::selax(axis a)
{
	if(animation) return;
	selectax=a;
	reselectbody();
}

void Cubik::selh(int dif)
{
	if(animation) return;
	if (dif	< 0 | dif > 2)
	{
		return;
	}
	selecth = dif;
	reselectbody();
}

void Cubik::resetchanges()
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			for(int k=0;k<3;k++)
				body[i][j][k].reset();
}

void Cubik::draw()
{
	glColor3f(0, 0.1, 0);
	int i;
	switch(selectax)
	{
	case UP:
		for(i=0;i<3;i++)
		{
			if(i==selecth) // we can rotate selected layer
			{
				glPushMatrix();
				glRotated(selectangle,0,1,0);
				drawuplayer(i);
				glPopMatrix();
			}
			else drawuplayer(i);
		}
		break;
	case SD:
		for(i=0;i<3;i++)
		{
			if(i==selecth)
			{
				glPushMatrix();
				glRotated(selectangle,1,0,0);
				drawsdlayer(i);
				glPopMatrix();
			}
			else drawsdlayer(i);
		}
		break;
	case FD:
		for(i=0;i<3;i++)
		{
			if(i==selecth)
			{
				glPushMatrix();
				glRotated(-selectangle,0,0,1);
				drawfdlayer(i);
				glPopMatrix();
			}
			else drawfdlayer(i);
		}
		break;
	}
}

void Cubik::drawscanning()
{
	glPushMatrix();

	glTranslated(0,0,-2);
	drawfdlayer(0); //front

	glPushMatrix(); //top
	glTranslated(0,6,0);
	glRotated(90,1,0,0);
	drawuplayer(0);
	glPopMatrix();

	glPushMatrix(); //bottom
	glTranslated(0,-6,0);
	glRotated(-90,1,0,0);
	drawuplayer(2);
	glPopMatrix();

	glPushMatrix(); //left
	glTranslated(-6,0,0);
	glRotated(-90,0,1,0);
	drawsdlayer(0);
	glPopMatrix();

	glPushMatrix(); //right
	glTranslated(6,0,0);
	glRotated(90,0,1,0);
	drawsdlayer(2);
	glPopMatrix();

	glPushMatrix(); //back
	glTranslated(12,0,0);
	glRotated(180,0,1,0);
	drawfdlayer(2);
	glPopMatrix();

	glPopMatrix();
}

void Cubik::startanimation()
{
	if(animation) return;
	animation=1;
	selectangle=0;
}

void Cubik::animationframe()
{
	// if animation is played;
	if(!animation) 
		return;	
	// Edit here to change speed of rotation
	selectangle-=5;			// visual rotatation layer changing angle
	if(selectangle<=-90)	// if rotation is finished
	{
		animation=0;
		selectangle=0;
		switch(selectax)	// perform logical 90 degrees rotation
		{
		case UP: rotateup();break;
		case FD: rotatefd();break;
		case SD: rotatesd();break;
		}
	}
}

void Cubik::randomize()
{
	int i,x;
	for(int i=0;i<100;i++)
	{
		x=qrand()%3;
		x-=1;
		selh(x);
		x=qrand()%3;
		switch(x)
		{
		case 0: rotateup();break;
		case 1: rotatefd();break;
		case 2: rotatesd();break;
		}
	}
	selh(0);
}

QString Cubik::tostring()
{
	QString res="";
	switch(selectax)
	{
	case UP:res="U"; break;
	case FD:res="F"; break;
	case SD:res="S"; break;
	}
	res+=QString::number(selecth);

	int i,j,k;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
			{
				res+=body[i][j][k].tostring();
			}
			return res;
}

void Cubik::fromstring(QString s)
{
	switch(s[0].toAscii())
	{
	case 'U':selectax=UP; break;
	case 'F':selectax=FD; break;
	case 'S':selectax=SD; break;
	}
	selecth=s[1].digitValue();
	int i,j,k;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
			{
				body[i][j][k].fromstring(s.mid(2+(i*9+j*3+k)*7,7));
			}
}