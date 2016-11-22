#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <Windows.h>

#include "CoorSys.h"
#include "LogiPadVehicle.h"
#include "MathTool.h"
#include "RoadDataSet.h"
#include "VehicleKinematics.h"
#include "StateVectorDefinition.h"
#include "QLearningController.h"

using namespace std;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

/* main window object */
GLFWwindow* window;


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


///*光源の属性の設定*/
void initLighting( void ){
	float light0_position[4]={10.0, 5.0, 5.0, 1.0};	//光源の位置設定配列(X,Y,Z,W) Wを0にすると原点に向かう指向性の光となる(らしい)

	glEnable(GL_LIGHTING);	//照明モデルを有効にする
	glEnable(GL_LIGHT0);	//光源の0番目を有効にする(0番目〜7番目まで使用可能．同時に複数の光源を設定できるらしい)
	glEnable(GL_COLOR_MATERIAL);	//物体の質感を有効にする

	//0番目の光源を設定
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

}


//描写物体の色・光の反射具合の設定
void SetLighting(float diffuse[] , float specular[] , float ambient[]){

	glLightfv( GL_LIGHT0 , GL_DIFFUSE , diffuse );/*拡散反射成分の強度*/
	glLightfv( GL_LIGHT0 , GL_SPECULAR , specular );/*鏡面反射成分の強度*/
	glLightfv( GL_LIGHT0 , GL_AMBIENT , ambient );/*環境光成分の強度*/
	glEnable( GL_LIGHT0 );
}


const double FIELD_MESH_SIZE = 1.0;
const double FIELD_SIZE_MIN_X = -500.;
const double FIELD_SIZE_MAX_X = 500.;
const double FIELD_SIZE_MIN_Y = -500.;
const double FIELD_SIZE_MAX_Y = 500.;
//フィールド格子
void ShowField(void){
	int i = 0;
	glLineWidth(1.0);	//線の太さを指定
	glBegin(GL_LINES);
	glColor3f(0.5,0.5,0.5);	//線の色を指定
	for(i = 0 ; FIELD_MESH_SIZE * (double)i + FIELD_SIZE_MIN_X < FIELD_SIZE_MAX_X ; i++){
		glVertex3d( FIELD_MESH_SIZE * (double)i + FIELD_SIZE_MIN_X , FIELD_SIZE_MAX_Y , 0.);
		glVertex3d( FIELD_MESH_SIZE * (double)i + FIELD_SIZE_MIN_X , FIELD_SIZE_MIN_Y , 0.);
	}
	for(i = 0 ; FIELD_MESH_SIZE * (double)i + FIELD_SIZE_MIN_Y < FIELD_SIZE_MAX_Y ; i++){
		glVertex3d( FIELD_SIZE_MAX_X , FIELD_MESH_SIZE * (double)i + FIELD_SIZE_MIN_Y , 0.);
		glVertex3d( FIELD_SIZE_MIN_X , FIELD_MESH_SIZE * (double)i + FIELD_SIZE_MIN_Y , 0.);
	}
	glEnd();
}


const double COODINATE_SYSTEM_MARKER_SIZE = 0.5;
//座標系の表示
void ShowCoordinate(void){
	//グローバル座標系の描写(赤)
	glLineWidth(3.0);	//線の太さを指定
	glBegin(GL_LINES);
	/* X軸(赤) */
	glColor3d(1.,0.,0.);	//線の色を指定
	glVertex3d(0.,0.,0.);
	glVertex3d(COODINATE_SYSTEM_MARKER_SIZE,0.,0.);
	/* Y軸(緑) */
	glColor3d(0.,1.,0.);	//線の色を指定
	glVertex3d(0.,0.,0.);
	glVertex3d(0.,COODINATE_SYSTEM_MARKER_SIZE,0.);
	/* Z軸(青) */
	glColor3d(0.,0.,1.);	//線の色を指定
	glVertex3d(0.,0.,0.);
	glVertex3d(0.,0.,COODINATE_SYSTEM_MARKER_SIZE);
	glEnd();
}


//キューブの描写
void ShowCube(void){
	/* マテリアルの光に対する反射具合を設定するパラメータ */
	float diffuse[] = {0.7 , 0.7 , 0.7 , 1.0};/*拡散反射成分の強度[R,G,B,A(Aは透明度)]*/
	float specular[] = {0.5 , 0.5 , 0.5 , 1.0};/*鏡面反射成分の強度[R,G,B,A(Aは透明度)]*/
	float ambient[] = {0.3 , 0.3 , 0.3 , 1.0};/*環境光成分の強度[R,G,B,A(Aは透明度)]*/

	static int tick = 0;
	glRotated((double)tick, 1., 1., 1.);
	tick++;

	/* マテリアル設定 */
	SetLighting(diffuse,specular,ambient);

	/* キューブの各面を描写していく */
	/* 1面 */
	glBegin(GL_POLYGON);
	glNormal3f(0.0 , -1.0 , 0.0);//光が反射する方向を指定
		glVertex3f( 0.   , 0. , 0.   );//1
		glVertex3f( 0.5 , 0. , 0.   );//2
		glVertex3f( 0.5 , 0. , 0.5);//3
		glVertex3f( 0.   , 0. , 0.5);//4
	glEnd();

	/* 2面 */
	glNormal3f(0.0 , 0.0 , -1.0);
	glBegin(GL_POLYGON);
		glVertex3f( 0.   , 0.   , 0.   );//1
		glVertex3f( 0.5 , 0.   , 0.   );//2
		glVertex3f( 0.5 , 0.5 , 0.   );//6
		glVertex3f( 0.   , 0.5 , 0.   );//5
	glEnd();

	/* 3面 */
	glNormal3f(0.0 , 1.0 , 0.0);
	glBegin(GL_POLYGON);
		glVertex3f( 0.   , 0.5 , 0.5 );//8
		glVertex3f( 0.5 , 0.5 , 0.5 );//7
		glVertex3f( 0.5 , 0.5 , 0.   );//6
		glVertex3f( 0.   , 0.5 , 0.   );//5
	glEnd();

	/* 4面 */
	glNormal3f(0.0 , 0.0 , 1.0);
	glBegin(GL_POLYGON);
		glVertex3f( 0.   , 0.5 , 0.5 );//8
		glVertex3f( 0.5 , 0.5 , 0.5 );//7
		glVertex3f( 0.5 , 0. , 0.5);//3
		glVertex3f( 0.   , 0. , 0.5);//4
	glEnd();

	/* 5面 */
	glNormal3f(1.0 , 0.0 , 0.0);
	glBegin(GL_POLYGON);
		glVertex3f( 0.5 , 0. , 0.   );//2
		glVertex3f( 0.5 , 0. , 0.5);//3
		glVertex3f( 0.5 , 0.5 , 0.5 );//7
		glVertex3f( 0.5 , 0.5 , 0.   );//6
	glEnd();

	/* 6面 */
	glNormal3f(-1.0 , 0.0 , 0.0);
	glBegin(GL_POLYGON);
		glVertex3f( 0.   , 0.   , 0.   );//1
		glVertex3f( 0.   , 0. , 0.5);//4
		glVertex3f( 0.   , 0.5 , 0.5 );//8
		glVertex3f( 0.   , 0.5 , 0.   );//5
	glEnd();
}

void ShowRoad(std::vector<XYZ> data){
	glPointSize(2);
	glBegin(GL_POINTS);
	for(int i = 0 ; i < data.size() ; i++){
		glVertex3d(data[i].X , data[i].Y , data[i].Z);
	}
	glEnd();
}

void ShowRoadVer2(std::vector<LineStruct> data){
	glColor3d(0.9, 0.9, 0.9);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for(int i = 0 ; i < data.size() ; i++){
		glVertex3d(data[i].P1.X, data[i].P1.Y , 0.);
		glVertex3d(data[i].P2.X, data[i].P2.Y , 0.);
	}
	glEnd();
}

void ShowVehicle(XYA pose){
	const double width = 1.875;
	const double length = 3.0;

	glPushMatrix();

	glTranslated(pose.X, pose.Y, 0.);
	glRotated(pose.A * RAD2DEG, 0. , 0. , 1.);

	//glColor3d( 0.9, 0.1, 0.1);
	//glPointSize(10);
	//glBegin(GL_POINTS);
	//	glVertex3d(0. , 0. , 0.);
	//glEnd();

	glColor3d(0.9, 0.1, 0.1);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
		//
		glVertex3d(0. , - width / 2. , 0.);
		glVertex3d(length , - width / 2. , 0.);
		//
		glVertex3d(length , - width / 2. , 0.);
		glVertex3d(length ,   width / 2. , 0.);
		//
		glVertex3d(length ,   width / 2. , 0.);
		glVertex3d(0 , width / 2. , 0.);
		//
		glVertex3d(0 ,  width / 2. , 0.);
		glVertex3d(0 , - width / 2. , 0.);
	glEnd();

	glPopMatrix();
}

void ShowStatePos(std::vector<XY> statePos){
	glColor3d(0.1 , 0.9 , 0.1);
	glPointSize(2);
	glBegin(GL_POINTS);
	for(int i = 0 ; i < statePos.size() ; i++){
		glVertex3d(statePos[i].X , statePos[i].Y , 0.);
	}
	glEnd();
}

void ShowState(std::vector<XY> statePos, std::vector<bool>state){
	glColor3d(0.1 , 0.9 , 0.1);
	glPointSize(10);
	glBegin(GL_POINTS);
	for(int i = 0 ; i < statePos.size() ; i++){
		if(state[i] == true){
			glColor3d(0.9 , 0.1 , 0.1);
			glVertex3d(statePos[i].X , statePos[i].Y , 0.);
		}else{
			glColor3d(0.1 , 0.9 , 0.1);
			glVertex3d(statePos[i].X , statePos[i].Y , 0.);
		}
	}
	glEnd();
}

double CalcReward(double dist_from_line){
	double reward = 0.;
	const double hani = 0.5;

	if(dist_from_line < hani){
		reward = -10.;
	}else{
		reward = 0.;
	}

	return reward;
}

const int window_size = 80;
const int window_width = 16 * window_size;
const int window_height = 9 * window_size;
const char title[100] = "glfw template";
const int windowPosX = 100;
const int windowPosY = 200;

/* Initialize main window */
void InitWindow(void){
	const double aspect = (double)window_width / (double)window_height;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowPos(window, windowPosX, windowPosY);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	glViewport(0, 0, window_width, window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0 , aspect , 1.0 , 500.0);	//透視投影の設定
	//光源の設定
	initLighting();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* Terminate main window */
void TerminateWindow(void){
    printf("See you...\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/* main function */
int main(void)
{
	const double SamplingTime = 0.02;

	InitWindow();

	/* Initialization Logicool Game Pad */
	LogiPadVehicle logiCtrl;
	logiCtrl.SetSamplingTime(SamplingTime);

	/* Init Raod data set */
	RoadDataSet rData;

	/* Init Vehicle kinematics */
	VehicleKinematics vKine;
	vKine.SetSamplingTime(SamplingTime);

	/* init State vector */
	StateVectorDefinition sVector;

	QLearningController QLC;

	/* main loop */
    while (!glfwWindowShouldClose(window))
    {
		/* Get Controller output */
		logiCtrl.SetCurrentState();
		//double vel = logiCtrl.GetVelocity();
		//double strAng = logiCtrl.GetSteeringAngle();
		//if(1. < vel){
		//	vel = 1.;
		//}else if(vel < -1.){
		//	vel = -1.;
		//}else{
		//	vel = 0.;
		//}
		//cout << "vel: " << vel << " strAng: " << strAng << endl;
		if( logiCtrl.GetButtonBACK() ){
			break;
		}

		/* calc vehicle pose */
		const double vel = 2.;	//const
		static double strAng = 0;
		vKine.SetVelocity(vel);
		vKine.SetRudderAngle(strAng);
		vKine.CalcPose();
		XYA VehiclePose = vKine.GetVehiclePose();

		/* calc state vector */
		sVector.SetWhiteLineData(rData.GetWhiteLineData());
		sVector.SetWhiteLineDataVer2( rData.GetWhiteLineDataVer2() );
		sVector.SetVehiclePose( VehiclePose );
		sVector.CalcState();
		std::vector<bool> State = sVector.GetState();

		/* Calc Reward */
		double reward = CalcReward( sVector.DistFromWhiteLine(VehiclePose.X, VehiclePose.Y) );
		//cout << "reward: " << reward << endl;

		/* Update Q function */
		QLC.SetCurState(sVector.GetState());
		QLC.SetReward(reward);
		QLC.UpdateQfunc();
		strAng = QLC.GetSteeringAngle();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		glEnable(GL_DEPTH_TEST);
		/*---------------------------------------------------------*/

		/* definition of the camera pose */
		const double wheelbase = 1.;
		double frontX = wheelbase * cos(VehiclePose.A) + VehiclePose.X;
		double frontY = wheelbase * sin(VehiclePose.A) + VehiclePose.Y;
		gluLookAt(VehiclePose.X , VehiclePose.Y , 10. , frontX , frontY , 0 , frontX - VehiclePose.X , frontY - VehiclePose.Y ,1);

		ShowField();

		ShowCoordinate();

		glColor3d(0.9 , 0.9 , 0.9);
		ShowRoadVer2(rData.GetWhiteLineDataVer2());

		glColor3d(0.9, 0.9, 0.9);
		ShowVehicle(VehiclePose);

		//ShowStatePos(sVector.GetStatePos());
		ShowState(sVector.GetStatePos(), sVector.GetState());

		glColor3d(0.9, 0.1, 0.1);
		ShowCube();

		/*---------------------------------------------------------*/
		glDisable(GL_DEPTH_TEST);
		glPopMatrix();
		glfwSwapBuffers(window);
		glfwPollEvents();

		if(reward < 0){
			vKine.ResetPose();
			QLC.Reset();
		}
    }

    TerminateWindow();
}



