#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <time.h>
#include <fstream>

#include "User.h"
#include "DBAccess.h"

class Classator
{
private:
	DBAccess* dba;
	std::list<User> learningA;
	std::list<User> learningB;
	std::list<User> testA;
	std::list<User> testB;
	int good_possitive = 0;
	int bad_possitive = 0;
	int good_negative = 0;
	int bad_negative = 0;
	double a, b, c; // parameters
	int max_product_visit = 16;
	int max_category_visit = 9;
	int max_all_visit = 554;
	int max_all_boughts = 26;
	int max_category_boughts = 10;
public:
	Classator(double a = 1.0, double b = 1.0, double c = 1.0);
	void CheckModel(std::string test_file, std::string output_file);
	void FindMaxs();
	void GroupUsers(std::vector<int> users, double n);
	bool IsUserInterestedInProduct(int product_id, User user);
	void PreperUserProductTestFile(int product_id, int user_id);
	void PreperLearingAFile(std::string file_name);
	void PreperLearingBFile(std::string file_name);
	void PreperTestAFile(std::string file_name);
	void PreperTestBFile(std::string file_name);
	void PrintLearningGroupA();
	void PrintLearningGroupB();
	void PrintTestGroupA();
	void PrintTestGroupB();
	void SetA(double new_a);
	void SetB(double new_b);
	void SetC(double new_c);
	~Classator();
};

