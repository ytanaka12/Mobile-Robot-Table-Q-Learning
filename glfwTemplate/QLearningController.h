#pragma once

#include <vector>
#include <random>

class QLearningController
{
public:
	struct Qstruct{
		std::vector<bool> State;
		std::vector<double> Q;
	};

private:
	double Reward;
	std::vector<bool> StateBeforeChanged_1;
	std::vector<bool> StateBefore_1;
	std::vector<bool> CurState;

	int ActionBeforeChanged_1;
	int Action;

	std::vector<Qstruct> Qfunc;

	std::random_device rnd;
	std::mt19937 mt;

public:
	QLearningController(void);
	~QLearningController(void);

	void Reset(void);

	void SetCurState(std::vector<bool> state);
	void SetReward(double reward);

	void UpdateQfunc(void);

	double GetSteeringAngle(void);

private:
	bool JudgeSameState(std::vector<bool> A, std::vector<bool> B);
	int FindArrayNumberOfQfunc(std::vector<Qstruct> qfunc, std::vector<bool>state);
	double FindMaxQvalueInAction(Qstruct A);
	double FindMinQvalueInAction(Qstruct A);
	int ActionDecision(Qstruct A);
};

