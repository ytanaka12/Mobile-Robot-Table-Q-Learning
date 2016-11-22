#pragma once

#include <vector>
#include "CoorSys.h"

class RoadDataSet
{
private:
	std::vector<XYZ> WhiteLineData;
	std::vector<LineStruct> WhiteLineDataVer2;

public:
	RoadDataSet(void);
	~RoadDataSet(void);

	std::vector<XYZ> GetWhiteLineData(void);
	std::vector<LineStruct> GetWhiteLineDataVer2(void);
};

