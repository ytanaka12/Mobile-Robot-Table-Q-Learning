#include "StateVectorDefinition.h"
#include <math.h>
#include <iostream>

const double Interval = 0.6;	//Interval between states [m]

const double LengthX = 2.;	//[m]
const double LengthY = 2.;	//[m]

double CalcDist(double ax, double ay, double bx, double by){
	return sqrt( pow(ax - bx, 2.) + pow(ay - by, 2.) );
}

void Rotation2D(XY input, XY &output, double rAng){
	output.X = input.X * cos(rAng) - input.Y * sin(rAng);
	output.Y = input.X * sin(rAng) + input.Y * cos(rAng);
}

StateVectorDefinition::StateVectorDefinition(void){
	VehiclePose.X = 0.;
	VehiclePose.Y = 0.;
	VehiclePose.A = 0.;

	/* Set state position */
	const int numX = (int)(LengthX / Interval) + 1;
	const int numY = (int)(LengthY / Interval) + 1;
	for(int i = 0 ; i < numX ; i++){
		for(int j = 0 ; j < numY ; j++){
			XY buf;
			buf.X = Interval * (double)i;
			buf.Y = Interval * (double)j;
			StatePosOrigin.push_back(buf);
			StatePos.push_back(buf);
		}
	}

	/* center in Y axis */
	for(int i = 0 ; i < StatePos.size() ; i++){
		StatePos[i].Y += - LengthY / 2.;
		StatePosOrigin[i].Y += - LengthY / 2.;
	}

	/* Init State */
	for(int i = 0 ; i < StatePos.size() ; i++){
		State.push_back(false);
	}
}


StateVectorDefinition::~StateVectorDefinition(void)
{
}


void StateVectorDefinition::SetVehiclePose(XYA pose){
	VehiclePose = pose;
}

void StateVectorDefinition::SetWhiteLineData(std::vector<XYZ> data){
	for(int i = 0 ; i < data.size() ; i++){
		WhiteLineData.push_back(data[i]);
	}
}

void StateVectorDefinition::SetWhiteLineDataVer2(std::vector<LineStruct> data){
	WhiteLineDataVer2 = data;
}

double StateVectorDefinition::DistFromWhiteLine(double x, double y){
	double Min = 10000000000000000000000000000000.;

	for(int j = 0 ; j < WhiteLineDataVer2.size() ; j++){
		XYZ A = WhiteLineDataVer2[j].P1;
		XYZ B = WhiteLineDataVer2[j].P2;
		XY C;
		C.X = x;
		C.Y = y;
		double AB = CalcDist(A.X, A.Y, B.X, B.Y);
		double BC = CalcDist(B.X, B.Y, C.X, C.Y);
		double AC = CalcDist(A.X, A.Y, C.X, C.Y);
		if( AC < AB && BC < AB ){
			double l = - ( BC*BC - AB*AB - AC*AC ) / ( 2. * AB );
			double dist = sqrt( AC*AC - l * l );
			if(dist < Min){
				Min = dist;
			}
		}
	}

	return Min;
}

void StateVectorDefinition::CalcState(void){
	/* rotation */
	for(int i = 0 ; i < StatePos.size() ; i++){
		XY buf;
		Rotation2D(StatePosOrigin[i], buf, VehiclePose.A);
		StatePos[i] = buf;
	}

	/* translation */
	for(int i = 0 ; i < StatePos.size() ; i++){
		StatePos[i].X += VehiclePose.X;
		StatePos[i].Y += VehiclePose.Y;
	}

	/* condition decision */
	//for(int i = 0 ; i < StatePos.size() ; i++){
	//	State[i] = false;
	//	for(int j = 0 ; j < WhiteLineData.size() ; j++){
	//		double dist = sqrt( pow(StatePos[i].X - WhiteLineData[j].X, 2.) + pow(StatePos[i].Y - WhiteLineData[j].Y, 2.) );
	//		if(dist < Interval){
	//			State[i] = true;
	//			j = WhiteLineData.size() + 1;
	//		}
	//	}
	//}

	//std::cout << "size: " << WhiteLineDataVer2.size() << std::endl;

	/* condition decision ver 2 */
	for(int i = 0 ; i < StatePos.size() ; i++){
		State[i] = false;
		double dist = DistFromWhiteLine(StatePos[i].X, StatePos[i].Y);
		if(dist < Interval / 2. + 0.1){
		//if(dist < Interval / 3.){
			State[i] = true;
		}
		//for(int j = 0 ; j < WhiteLineDataVer2.size() ; j++){
		//	XYZ A = WhiteLineDataVer2[j].P1;
		//	XYZ B = WhiteLineDataVer2[j].P2;
		//	XY C = StatePos[i];
		//	double AB = CalcDist(A.X, A.Y, B.X, B.Y);
		//	double BC = CalcDist(B.X, B.Y, C.X, C.Y);
		//	double AC = CalcDist(A.X, A.Y, C.X, C.Y);
		//	if( AC < AB && BC < AB ){
		//		double l = - ( BC*BC - AB*AB - AC*AC ) / ( 2. * AB );
		//		double dist = sqrt( AC*AC - l * l );
		//		if(dist < Interval){
		//			State[i] = true;
		//			j = WhiteLineDataVer2.size() + 1;
		//		}
		//	}
		//}
	}
}

std::vector<XY> StateVectorDefinition::GetStatePos(void){
	return StatePos;
}
std::vector<bool> StateVectorDefinition::GetState(void){
	return State;
}





