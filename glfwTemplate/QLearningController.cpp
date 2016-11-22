#include "QLearningController.h"
#include <iostream>
#include <random>

const int NumAction = 5;
//Action: 0 ~ 6
//double Angles[7] = {-45.0 , -30.0 , -15.0 , 0.0 , 15.0 , 30.0 , 45.0 };
double Angles[5] = {-60.0 , -30.0 , 0.0 , 30.0 , 60.0 };

const double InitQvalue = 0.;

QLearningController::QLearningController(void)
{
	Reward = 0.;

	ActionBeforeChanged_1 = 2;
	Action = 2;
}

QLearningController::~QLearningController(void)
{
}

void QLearningController::Reset(void){
	CurState = Qfunc[0].State;
	StateBefore_1 = CurState;
	StateBeforeChanged_1 = CurState;
	ActionBeforeChanged_1 = 2;
	Action = 2;
	Reward = 0.;
}

void QLearningController::SetCurState(std::vector<bool> state){
	CurState = state;
}

void QLearningController::SetReward(double reward){
	Reward = reward;
}

void QLearningController::UpdateQfunc(void){
	static bool FlagInit = true;
	bool FlagChange = false;
	bool FlagLowReward = false;

	/* only first time */
	if(FlagInit == true){
		FlagInit = false;
		StateBefore_1 = CurState;
		StateBeforeChanged_1 = CurState;
		Qstruct buf;
		buf.State = CurState;
		//init Q value
		for(int i = 0 ; i < NumAction ; i++){
			buf.Q.push_back(InitQvalue);
		}
		Qfunc.push_back(buf);
		return;
	}

	bool FlagUpdate = false;

	/* Is state changed ? */
	FlagChange = false;
	if(JudgeSameState(CurState, StateBefore_1) == false){
		FlagUpdate = true;
		FlagChange = true;
		StateBeforeChanged_1 = StateBefore_1;
		ActionBeforeChanged_1 = Action;
	}
	StateBefore_1 = CurState;

	if(Reward < 0.){
		FlagUpdate = true;
		FlagLowReward = true;
		std::cout << "hogehoge\n" << std::endl;
	}

	if(FlagUpdate == true){
		/* New ? */
		bool FlagNewState = true;
		if(FindArrayNumberOfQfunc(Qfunc, CurState) < 0){
			FlagNewState = true;
		}else{
			FlagNewState = false;
		}

		/* if current state is not a member of Qfunc, the state is added to Qfunc */
		if(FlagNewState == true){
			Qstruct buf;
			buf.State = CurState;
			for(int i = 0 ; i < NumAction ; i++){
				buf.Q.push_back(InitQvalue);
			}
			Qfunc.push_back(buf);
		}
		//std::cout << "number of states: " << Qfunc.size() << std::endl;

		/* update Q */
		int NumCurState = FindArrayNumberOfQfunc(Qfunc, CurState);
		int NumStateBeforeChanged_1 = FindArrayNumberOfQfunc(Qfunc, StateBeforeChanged_1);

		std::cout << "-------------------------------------------------------------" << std::endl;

		/*****************************************************************************************/
		int NumState = FindArrayNumberOfQfunc(Qfunc, CurState);
		std::cout << "Q C: ";
		for(int i = 0 ; i < NumAction ; i++){
			std::cout << Qfunc[NumState].Q[i] << " ";
		}
		std::cout << std::endl;
		/*****************************************************************************************/

		/*****************************************************************************************/
		NumState = FindArrayNumberOfQfunc(Qfunc, StateBeforeChanged_1);
		std::cout << "Q B: ";
		for(int i = 0 ; i < NumAction ; i++){
			std::cout << Qfunc[NumState].Q[i] << " ";
		}
		std::cout << std::endl;
		/*****************************************************************************************/

		const double alpha = 0.8;
		const double gamma = 0.8;
		double MaxQvalueInAction = FindMaxQvalueInAction(Qfunc[NumCurState]);
		Qfunc[NumStateBeforeChanged_1].Q[ActionBeforeChanged_1] += alpha * ( Reward + gamma * MaxQvalueInAction - Qfunc[NumStateBeforeChanged_1].Q[ActionBeforeChanged_1] );

		std::cout << "MaxQvalue: " << MaxQvalueInAction << std::endl;

		/*****************************************************************************************/
		NumState = FindArrayNumberOfQfunc(Qfunc, StateBeforeChanged_1);
		std::cout << "Q A: ";
		for(int i = 0 ; i < NumAction ; i++){
			std::cout << Qfunc[NumState].Q[i] << " ";
		}
		std::cout << std::endl;
		/*****************************************************************************************/

		if(FlagLowReward == true){
			//for(int i = 0 ; i < NumAction ; i++){
			//	Qfunc[NumState].Q[i] = -10.;
			//}
			Qfunc[NumState].Q[Action] = Reward;
		}

		//std::cout << "Q: " << Qfunc[NumCurState].Q[Action] << std::endl;
		//std::cout << "Q: " << Qfunc[NumStateBefore_1].Q[ActionBefore_1] << std::endl;
		//std::cout << "Reward: " << Reward << std::endl;

		/* Action */
		//ActionBeforeChanged_1 = Action;
		Action = ActionDecision( Qfunc[NumCurState] );
	}

	//int NumCurState = FindArrayNumberOfQfunc(Qfunc, CurState);
	//std::cout << "Q: ";
	//	for(int i = 0 ; i < NumAction ; i++){
	//		std::cout << Qfunc[NumCurState].Q[i] << " ";
	//	}
	//std::cout << std::endl;

	//int NumState = FindArrayNumberOfQfunc(Qfunc, StateBeforeChanged_1);
	//std::cout << "Q: ";
	//	for(int i = 0 ; i < NumAction ; i++){
	//		std::cout << Qfunc[NumState].Q[i] << " ";
	//	}
	//std::cout << std::endl;

	//std::cout << "cur: " << FindArrayNumberOfQfunc(Qfunc, CurState) << " bef: " << FindArrayNumberOfQfunc(Qfunc, StateBeforeChanged_1) << std::endl;
	//std::cout << "curAct: " << Action << " befAct: " << ActionBeforeChanged_1 << std::endl;
}

double QLearningController::GetSteeringAngle(void){
	return Angles[Action];
}


bool QLearningController::JudgeSameState(std::vector<bool> A, std::vector<bool> B){
	bool FlagSameState = true;
	for(int i = 0 ; i < A.size() ; i++){
		if(A[i] != B[i]){
			FlagSameState = false;
			i = A.size() + 1;
		}
	}

	return FlagSameState;
}

int QLearningController::FindArrayNumberOfQfunc(std::vector<Qstruct> qfunc, std::vector<bool>state){
	for(int i = 0 ; i < qfunc.size() ; i++){
		if(JudgeSameState(state, qfunc[i].State) == true){
			return i;
		}
	}

	return -1;
}

double QLearningController::FindMaxQvalueInAction(Qstruct A){
	double Max = -10000000000000000000000000000000000000000.;
	for(int i = 0 ; i < NumAction ; i++){
		if(Max < A.Q[i]){
			Max = A.Q[i];
		}
	}
	return Max;
}

double QLearningController::FindMinQvalueInAction(Qstruct A){
	double Min = 10000000000000000000000000000000000.;
	for(int i = 0 ; i < NumAction ; i++){
		if(A.Q[i] < Min){
			Min = A.Q[i];
		}
	}
	return Min;
}

int QLearningController::ActionDecision(Qstruct A){
	int action = 3;
	double Max = FindMaxQvalueInAction(A);
	double Min = FindMinQvalueInAction(A);

	mt.seed(rnd());
	std::uniform_int_distribution<> rand7(0, NumAction - 1);	//random: 0 ~ 6
	std::uniform_real_distribution<> real_rand1(0. , 1.);	//random: 0.0 ~ 1.0

	/* if different between Max and Min is small, Action is decided as random */
	if(fabs(Max - Min) < 0.1){
		action = rand7(mt);
	}else{
		double q[NumAction];
		double NCoef = 0.;
		for(int i = 0 ; i < NumAction ; i++){
			q[i] = exp( A.Q[i] - Min + fabs(Max - Min) * 0.01);
			NCoef += q[i];
		}
		for(int i = 0 ; i < NumAction ; i++){
			q[i] = q[i] / NCoef;
		}
		/* Darts */
		//double dart = real_rand1(mt);
		//double lower = 0.;
		//double upper = 0.;
		//for(int i = 0 ; i < NumAction ; i++){
		//	lower = upper;
		//	upper += q[i];
		//	if( lower <= dart && dart < upper ){
		//		action = i;
		//	}
		//}
		/* Greedy */
		double MaxQ = -1000000000000000000000000000000000000000000.;
		for(int i = 0 ; i < NumAction ; i++){
			if(MaxQ < A.Q[i]){
				MaxQ = A.Q[i];
				action = i;
			}
		}
	}

	return action;
}










