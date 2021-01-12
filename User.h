#pragma once

#include <iostream>
#include <list>

class User
{
private:
	int id;
	std::list<int> intrested;
public:
	User(int id);
	friend std::ostream& operator<< (std::ostream& out, const User& user);
};

