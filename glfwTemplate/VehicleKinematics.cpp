#include "VehicleKinematics.h"
#include <math.h>

const double WheelBase = 3.;

VehicleKinematics::VehicleKinematics(void){
	VehiclePose.X = 0.;
	VehiclePose.Y = 0.;
	VehiclePose.A = 0.;

	Velocity = 0.;
	RudderAngle = 0.;

	SamplingTime = 0.01;
}


VehicleKinematics::~VehicleKinematics(void){
}

void VehicleKinematics::ResetPose(void){
	VehiclePose.X = 5.;
	VehiclePose.Y = 0.;
	VehiclePose.A = 0.;
}

void VehicleKinematics::SetSamplingTime(double sampling_time){
	SamplingTime = sampling_time;
}

void VehicleKinematics::SetVelocity(double velocity){
	Velocity = velocity;
}

void VehicleKinematics::SetRudderAngle(double angle){
	RudderAngle = angle;
}

void VehicleKinematics::CalcPose(void){
	double velX = Velocity * cos(VehiclePose.A);
	double velY = Velocity * sin(VehiclePose.A);
	double yawrate = Velocity * tan(RudderAngle) / WheelBase;

	VehiclePose.X += velX * SamplingTime;
	VehiclePose.Y += velY * SamplingTime;
	VehiclePose.A += yawrate * SamplingTime;
}


XYA VehicleKinematics::GetVehiclePose(void){
	return VehiclePose;
}