#include "LogiPadVehicle.h"
#include <iostream>
#include "MathTool.h"

using namespace std;

const double MaxStrAng = 45. * DEG2RAD;
const double MaxAcceleration = 2.;	//[m/s^2]

LogiPadVehicle::LogiPadVehicle(void){
	SamplingTime = 0.01;
}


LogiPadVehicle::~LogiPadVehicle(void)
{
}

void LogiPadVehicle::SetSamplingTime(double sampling_time){
	SamplingTime = sampling_time;
}

void LogiPadVehicle::SetCurrentState(void){
	/* Get State */
	XINPUT_STATE xinput_state = GetState();

	//Analog signal
	bRT = xinput_state.Gamepad.bRightTrigger;
	bLT = xinput_state.Gamepad.bLeftTrigger;
	sLX = xinput_state.Gamepad.sThumbLX;

	//Digital signal
	if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK){
		bBACK = true;
	}else{
		bBACK = false;
	}

	/* Calculate Velocity */
	if( 0 < bRT && 0 < bLT){
		double acc = MaxAcceleration * (double)bLT / (double)255;
		Velocity -= acc * SamplingTime;
	}else if(0 < bLT){
		double acc = MaxAcceleration * (double)bLT / (double)255;
		Velocity -= acc * SamplingTime;
	}else if(0 < bRT){
		double acc = MaxAcceleration * (double)bRT / (double)255;
		Velocity += acc * SamplingTime;
	}

	/* Calculate Steering Angle */
	if(sLX == 128){
		SteeringAngle = 0.;
	}else if(sLX < 128){
		SteeringAngle = (double)(sLX - 128);
		SteeringAngle = SteeringAngle / (32768 + 128);
		SteeringAngle = MaxStrAng * SteeringAngle;
	}else if(128 < sLX){
		SteeringAngle = (double)(sLX - 128);
		SteeringAngle = SteeringAngle / (32767 - 128);
		SteeringAngle = MaxStrAng * SteeringAngle;
	}else{
		SteeringAngle = 0.;
	}
	SteeringAngle = - SteeringAngle;

	//cout << "bRT: " << (double)bRT << " bLT: " << (double)bLT << " sLX: " << sLX << " bBACK: " << bBACK << endl;
}

double LogiPadVehicle::GetVelocity(void){
	return Velocity;
}

double LogiPadVehicle::GetSteeringAngle(void){
	return SteeringAngle;
}

bool LogiPadVehicle::GetButtonBACK(void){
	return bBACK;
}



