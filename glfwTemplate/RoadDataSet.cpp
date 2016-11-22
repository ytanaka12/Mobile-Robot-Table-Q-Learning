#include "RoadDataSet.h"

/* Rectangle Road course */

const double SizeX = 20.;
const double SizeY = 10.;
const double RoadWidth = 3.;

const double PointInterval = 0.9;	//Interval between data points

std::vector<XYZ> DataPoint2Point(double startX, double startY, double endX, double endY){
	std::vector<XYZ> Ans;

	double dist = sqrt( pow(endX - startX, 2.) + pow(endY - startY, 2.) );
	int point_size = (int)(dist / PointInterval);

	XYZ buf;
	double uVectorX = ( endX - startX ) / dist;
	double uVectorY = ( endY - startY ) / dist;
	for(int i = 0 ; i < point_size ; i++){
		buf.X = PointInterval * uVectorX * (double)i + startX;
		buf.Y = PointInterval * uVectorY * (double)i + startY;
		buf.Z = 0.;
		Ans.push_back(buf);
	}

	return Ans;
}

std::vector<LineStruct> SetWhiteLineDataVer2(void){
	std::vector<LineStruct> Ans;
	LineStruct buf;

	//1
	buf.P1.X = - SizeX / 2.;
	buf.P1.Y = RoadWidth / 2.;
	buf.P2.X = SizeX / 2.;
	buf.P2.Y = RoadWidth / 2.;
	Ans.push_back(buf);

	//
	/*  */
	buf.P1.X = SizeX / 2.;
	buf.P1.Y = RoadWidth / 2.;
	buf.P2.X = SizeX / 2.;
	buf.P2.Y = SizeY + RoadWidth / 2.;
	Ans.push_back(buf);
	/*  */
	buf.P1.X = SizeX / 2.;
	buf.P1.Y = SizeY + RoadWidth / 2.;
	buf.P2.X = - SizeX / 2.;
	buf.P2.Y = SizeY + RoadWidth / 2.;
	Ans.push_back(buf);
	/*  */
	buf.P1.X = - SizeX / 2.;
	buf.P1.Y = SizeY + RoadWidth / 2.;
	buf.P2.X = - SizeX / 2.;
	buf.P2.Y = RoadWidth / 2.;
	Ans.push_back(buf);
	/*  */
	buf.P1.X = - (SizeX / 2. + RoadWidth);
	buf.P1.Y = - RoadWidth / 2.;
	buf.P2.X = SizeX / 2. + RoadWidth;
	buf.P2.Y = - RoadWidth / 2.;
	Ans.push_back(buf);
	/*  */
	buf.P1.X = SizeX / 2. + RoadWidth;
	buf.P1.Y = - RoadWidth / 2.;
	buf.P2.X = SizeX / 2. + RoadWidth;
	buf.P2.Y = SizeY + RoadWidth / 2. + RoadWidth;
	Ans.push_back(buf);
	/*  */
	buf.P1.X = SizeX / 2. + RoadWidth;
	buf.P1.Y = SizeY + RoadWidth / 2. + RoadWidth;
	buf.P2.X = - (SizeX / 2. + RoadWidth);
	buf.P2.Y = SizeY + RoadWidth / 2. + RoadWidth;
	Ans.push_back(buf);
	/*  */
	buf.P1.X = - (SizeX / 2. + RoadWidth);
	buf.P1.Y = SizeY + RoadWidth / 2. + RoadWidth;
	buf.P2.X = - (SizeX / 2. + RoadWidth);
	buf.P2.Y = - RoadWidth / 2.;
	Ans.push_back(buf);

	return Ans;
}

RoadDataSet::RoadDataSet(void){
	double startX;
	double startY;
	double endX;
	double endY;
	std::vector<XYZ> buf_data;

	/*  */
	startX = - SizeX / 2.;
	startY = RoadWidth / 2.;
	endX = SizeX / 2.;
	endY = RoadWidth / 2.;

	buf_data = DataPoint2Point(startX, startY, endX, endY);
	for(int i = 0 ; i < buf_data.size() ; i++){
		WhiteLineData.push_back(buf_data[i]);
	}
	buf_data.clear();

	/*  */
	startX = SizeX / 2.;
	startY = RoadWidth / 2.;
	endX = SizeX / 2.;
	endY = SizeY + RoadWidth / 2.;

	buf_data = DataPoint2Point(startX, startY, endX, endY);
	for(int i = 0 ; i < buf_data.size() ; i++){
		WhiteLineData.push_back(buf_data[i]);
	}
	buf_data.clear();

	/*  */
	startX = SizeX / 2.;
	startY = SizeY + RoadWidth / 2.;
	endX = - SizeX / 2.;
	endY = SizeY + RoadWidth / 2.;

	buf_data = DataPoint2Point(startX, startY, endX, endY);
	for(int i = 0 ; i < buf_data.size() ; i++){
		WhiteLineData.push_back(buf_data[i]);
	}
	buf_data.clear();

	/*  */
	startX = - SizeX / 2.;
	startY = SizeY + RoadWidth / 2.;
	endX = - SizeX / 2.;
	endY = RoadWidth / 2.;

	buf_data = DataPoint2Point(startX, startY, endX, endY);
	for(int i = 0 ; i < buf_data.size() ; i++){
		WhiteLineData.push_back(buf_data[i]);
	}
	buf_data.clear();
	/*------------------------------------------------------------------*/
	/*  */
	startX = - (SizeX / 2. + RoadWidth);
	startY = - RoadWidth / 2.;
	endX = SizeX / 2. + RoadWidth;
	endY = - RoadWidth / 2.;

	buf_data = DataPoint2Point(startX, startY, endX, endY);
	for(int i = 0 ; i < buf_data.size() ; i++){
		WhiteLineData.push_back(buf_data[i]);
	}
	buf_data.clear();

	/*  */
	startX = SizeX / 2. + RoadWidth;
	startY = - RoadWidth / 2.;
	endX = SizeX / 2. + RoadWidth;
	endY = SizeY + RoadWidth / 2. + RoadWidth;

	buf_data = DataPoint2Point(startX, startY, endX, endY);
	for(int i = 0 ; i < buf_data.size() ; i++){
		WhiteLineData.push_back(buf_data[i]);
	}
	buf_data.clear();

	/*  */
	startX = SizeX / 2. + RoadWidth;
	startY = SizeY + RoadWidth / 2. + RoadWidth;
	endX = - (SizeX / 2. + RoadWidth);
	endY = SizeY + RoadWidth / 2. + RoadWidth;

	buf_data = DataPoint2Point(startX, startY, endX, endY);
	for(int i = 0 ; i < buf_data.size() ; i++){
		WhiteLineData.push_back(buf_data[i]);
	}
	buf_data.clear();

	/*  */
	startX = - (SizeX / 2. + RoadWidth);
	startY = SizeY + RoadWidth / 2. + RoadWidth;
	endX = - (SizeX / 2. + RoadWidth);
	endY = - RoadWidth / 2.;

	buf_data = DataPoint2Point(startX, startY, endX, endY);
	for(int i = 0 ; i < buf_data.size() ; i++){
		WhiteLineData.push_back(buf_data[i]);
	}
	buf_data.clear();

	/*------------------------------------------------------------------*/
	WhiteLineDataVer2 = SetWhiteLineDataVer2();
}


RoadDataSet::~RoadDataSet(void)
{
}

std::vector<XYZ> RoadDataSet::GetWhiteLineData(void){
	return WhiteLineData;
}

std::vector<LineStruct> RoadDataSet::GetWhiteLineDataVer2(void){
	return WhiteLineDataVer2;
}
