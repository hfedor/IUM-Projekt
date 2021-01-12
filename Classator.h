#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <time.h> 

#include "User.h"

class Classator
{
private:
	std::list<User> learning;
	std::list<User> test;
public:
	Classator();
	void GroupUsers(std::vector<int> users, double n);
	void PrintLearningGroup();
	void PrintTestGroup();
};

