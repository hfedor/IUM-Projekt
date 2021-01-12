#include "Classator.h"

using namespace std;

Classator::Classator()
{
}

void Classator::GroupUsers(vector<int> users, double n)
{
	int test_counter = floor(users.size() * n);
	srand(time(NULL));

	for (int i = test_counter; i > 0; i--)
	{
		int random_user = rand() %users.size();
		int j = 0;
		for (vector<int>::iterator i = users.begin(); i != users.end(); i++)
		{
			if (j == random_user)
			{
				test.push_back(*i);
				users.erase(i);
				test_counter--;
				break;
			}
			j++;
		}
	}

	for (vector<int>::iterator i = users.begin(); i != users.end(); i++)
		learning.push_back(*i);

}

void Classator::PrintLearningGroup()
{
	cout << "Learning Group" << endl;
	for (list<User>::iterator i = learning.begin(); i != learning.end(); i++)
		cout << (*i) << endl;
}

void Classator::PrintTestGroup()
{
	cout << "Test Group" << endl;
	for (list<User>::iterator i = test.begin(); i != test.end(); i++)
		cout << (*i) << endl;
}
