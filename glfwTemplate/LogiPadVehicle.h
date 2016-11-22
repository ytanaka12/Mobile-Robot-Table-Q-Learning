#pragma once
#include "xboxcontroller.h"

class LogiPadVehicle :
	public CXBOXController
{
private:
	/* Signal State */
	unsigned int bRT;
	unsigned int bLT;
	bool bBACK;
	short sLX;

	double SamplingTime;

	/* Control Value */
	double Velocity;
	double SteeringAngle;

public:
	LogiPadVehicle(void);
	~LogiPadVehicle(void);

	void SetSamplingTime(double sampling_time);
	void SetCurrentState(void);

	double GetVelocity(void);
	double GetSteeringAngle(void);
	bool GetButtonBACK(void);
};

