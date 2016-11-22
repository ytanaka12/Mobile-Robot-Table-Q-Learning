
/* definition the Coordinate System Structures */

#ifndef COOR_SYS_H
#define COOR_SYS_H

struct XYZ{
	double X;
	double Y;
	double Z;
};

struct XYA{
	double X;
	double Y;
	double A;
};

struct XY{
	double X;
	double Y;
};

struct LineStruct{
	XYZ P1;
	XYZ P2;
};

#endif