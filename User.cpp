#include "User.h"

using namespace std;

User::User(int new_id)
{
	id = new_id;
}

std::ostream& operator<<(std::ostream& out, const User& user)
{
	return out << user.id;
}
