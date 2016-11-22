#pragma once

#include "CoorSys.h"

class VehicleKinematics
{
public:

private:
	XYA VehiclePose;
	double Velocity;
	double RudderAngle;

	double SamplingTime;

public:
	VehicleKinematics(void);
	~VehicleKinematics(void);

	void ResetPose(void);

	void SetSamplingTime(double sampling_time);
	void SetVelocity(double velocity);
	void SetRudderAngle(double angle);

	void CalcPose(void);

	XYA GetVehiclePose(void);
};

