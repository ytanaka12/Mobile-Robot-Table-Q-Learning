#pragma once

#include <vector>
#include "CoorSys.h"

class StateVectorDefinition
{
private:
	std::vector<XY> StatePosOrigin;
	std::vector<XY> StatePos;
	std::vector<bool> State;

	std::vector<XYZ> WhiteLineData;
	std::vector<LineStruct> WhiteLineDataVer2;

	XYA VehiclePose;

public:
	StateVectorDefinition(void);
	~StateVectorDefinition(void);

	void SetVehiclePose(XYA pose);
	void SetWhiteLineData(std::vector<XYZ> data);
	void SetWhiteLineDataVer2(std::vector<LineStruct> data);

	double DistFromWhiteLine(double x, double y);
	void CalcState(void);

	std::vector<XY> GetStatePos(void);
	std::vector<bool> GetState(void);
};

