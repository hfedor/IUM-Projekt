#include "Classator.h"

using namespace std;

Classator::Classator(double a, double b, double c)
{
	dba = new DBAccess("IUM_Projekt.db");
}

void Classator::CheckModel(std::string test_file_name, std::string output_file_name)
{
	std::fstream test_file, output_file;
	test_file.open(test_file_name, std::ios::in);
	output_file.open(output_file_name, std::ios::in);
	if (test_file.good() == true && output_file.good() == true)
	{
		//std::cout << "Uzyskano dostep do pliku!" << std::endl;

		good_possitive = 0;
		bad_possitive = 0;
		good_negative = 0;
		bad_negative = 0;
		
		string line;
		string tmp = "";
		int user_id, product_id;
		bool is_possitive;
		float tmp_float;
		
		while (getline(test_file, line))
		{
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == '#')
				{
					i += 2;
					for (int j = i; j < line.length(); j++)
					{
						if (line[j] == ';')
							break;
						tmp += line[j];
						i++;
					}
					user_id = atoi(tmp.c_str());
					//cout << "user: " << user_id;
					tmp = "";
					i++;
					for (int j = i; j < line.length(); j++)
					{
						tmp += line[j];
						i++;
					}
					product_id = atoi(tmp.c_str());
					//cout << "product: " << product_id << endl;
					tmp = "";

					output_file >> tmp_float;
					is_possitive = tmp_float > 0;

					if (is_possitive)
					{
						if (dba->WasProductBoughtByUser(product_id, user_id))
							good_possitive++;
						else
							bad_possitive++;
					}
					else
					{
						if (dba->WasProductBoughtByUser(product_id, user_id))
							bad_negative++;
						else
							good_negative++;
					}
				}
			}
		}

		cout << "good possitives: " << good_possitive << endl;
		cout << "bad possitives: " << bad_possitive << endl;
		cout << "good neativess: " << good_negative << endl;
		cout << "bad negatives: " << bad_negative << endl;

		int all_possitive = good_possitive + bad_possitive;
		int all_negative = good_negative + bad_negative;
		int all_general = all_possitive + all_negative;
		int all_good = good_possitive + good_negative;

		cout << "good possitives percentage: " << (float)((float)good_possitive / (float)all_possitive) << endl;
		cout << "bad possitives percentage: " << (float)((float)bad_possitive / (float)all_possitive) << endl;
		cout << "good neativess percentage: " << (float)((float)good_negative / (float)all_negative) << endl;
		cout << "bad negatives percentage: " << (float)((float)bad_negative / (float)all_negative) << endl;
		cout << "good general percentage: " << (float)all_good / (float)all_general << endl;
	}
	else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;

}

void Classator::FindMaxs()
{
	vector<int> users = dba->GetUserIDs();

	for (vector<int>::iterator i = users.begin(); i != users.end(); i++)
	{
		vector<int> products = dba->GetProductsVisitedByUser(*i);

		int all_visit = dba->CountAllProductsVisitByUser(*i);
		
		if (all_visit > max_all_visit)
			max_all_visit = all_visit;

		for (vector<int>::iterator j = products.begin(); j != products.end(); j++)
		{
			//int product_visit = dba->CountProductVisitByUser(*j, *i);
			//int category_visit = dba->CountProductsCategoryVisitByUser(*j, *i);

			//if (product_visit > max_product_visit)
				//max_product_visit = product_visit;
			//if (category_visit > max_category_visit)
				//max_category_visit = category_visit;

			int all_boughts = dba->CountAllProductsBoughtedByUser(*j, *i);
			int category_boughts = dba->CountProductsCategoryBoughtedByUser(*j, *i);

			if (all_boughts > max_all_boughts)
				max_all_boughts = all_boughts;
			if (category_boughts > max_category_boughts)
				max_category_boughts = category_boughts;
		}
	}

	cout << "max all visit: " << max_all_visit << endl;
	cout << "max all boughts: " << max_all_boughts << endl;
	cout << "max category boughts: " << max_category_boughts << endl;
}

void Classator::GroupUsers(vector<int> users, double n)
{
	int test_counter = floor(users.size() * n / 2);
	srand(time(NULL));

	for (int i = test_counter; i > 0; i--)
	{
		int random_user = rand() %users.size();
		int j = 0;
		for (vector<int>::iterator i = users.begin(); i != users.end(); i++)
		{
			if (j == random_user)
			{
				testA.push_back(*i);
				users.erase(i);
				break;
			}
			j++;
		}
	}

	for (int i = test_counter; i > 0; i--)
	{
		int random_user = rand() % users.size();
		int j = 0;
		for (vector<int>::iterator i = users.begin(); i != users.end(); i++)
		{
			if (j == random_user)
			{
				testB.push_back(*i);
				users.erase(i);
				break;
			}
			j++;
		}
	}

	for (int i = floor(users.size()/2); i > 0; i--)
	{
		int random_user = rand() % users.size();
		int j = 0;
		for (vector<int>::iterator i = users.begin(); i != users.end(); i++)
		{
			if (j == random_user)
			{
				learningA.push_back(*i);
				users.erase(i);
				break;
			}
			j++;
		}
	}

	for (vector<int>::iterator i = users.begin(); i != users.end(); i++)
		learningB.push_back(*i);

}

bool Classator::IsUserInterestedInProduct(int product_id, User user)
{
	int porduct_visit = dba->CountProductVisitByUser(product_id, user.GetID());
	int category_visit = dba->CountProductsCategoryVisitByUser(product_id, user.GetID());

	/*
		y = a/(x-b) - c <==> a = (x-b)(y+c)
		b > 0; c > 0; a > 0;

		x - product_visit;
		y - category_vist;
		b - depends on x;
		c - depends on y;
		a - depends on x and y;
	*/

	if (a >= (porduct_visit - b) * (category_visit - c))
		return true;
	else
		return false;
}

void Classator::PreperUserProductTestFile(int product_id, int user_id)
{
	std::fstream file;
	file.open("user_test_file.dab", std::ios::out);
	if (file.good() == true)
	{
		std::cout << "File is good!!" << std::endl;

		bool was_bought = dba->WasProductBoughtByUser(product_id, user_id);

		int product_visit = dba->CountProductVisitByUser(product_id, user_id);
		int category_visit = dba->CountProductsCategoryVisitByUser(product_id, user_id);
		int all_visit = dba->CountAllProductsVisitByUser(user_id);
		int all_boughts = dba->CountAllProductsBoughtedByUser(product_id, user_id);
		int category_boughts = dba->CountProductsCategoryBoughtedByUser(product_id, user_id);

		if (was_bought)
			file << "+1 ";
		else
			file << "-1 ";

		float normalize_product_visit = (float)product_visit / (float)max_product_visit;
		float normalize_category_visit = (float)category_visit / (float)max_category_visit;
		float normalize_all_visit = (float)all_visit / (float)max_all_visit;
		float normalize_all_boughts = (float)all_boughts / (float)max_all_boughts;
		float normalize_category_boughts = (float)category_boughts / (float)max_category_boughts;

		file << "1:" << normalize_product_visit << " "
			<< "2:" << normalize_category_visit << " "
			<< "3:" << normalize_all_visit << " "
			<< "4:" << normalize_all_boughts << " "
			<< "5:" << normalize_category_boughts << " "
			<< "# " << user_id << ";" << product_id << endl;

		file.close();
	}
	else std::cout << "Can't open the file!" << std::endl;
}

void Classator::PreperLearingAFile(std::string file_name)
{
	std::fstream file;
	file.open(file_name, std::ios::out);
	if (file.good() == true)
	{
		std::cout << "Uzyskano dostep do pliku!" << std::endl;
		
		for (list<User>::iterator i = learningA.begin(); i != learningA.end(); i++)
		{
			vector<int> products = dba->GetProductsVisitedByUser(i->GetID());

			for (vector<int>::iterator j = products.begin(); j != products.end(); j++)
			{
				bool was_bought = dba->WasProductBoughtByUser(*j, i->GetID());

				int product_visit = dba->CountProductVisitByUser(*j, i->GetID());
				int category_visit = dba->CountProductsCategoryVisitByUser(*j, i->GetID());
				int all_visit = dba->CountAllProductsVisitByUser(i->GetID());
				int all_boughts = dba->CountAllProductsBoughtedByUser(*j, i->GetID());
				int category_boughts = dba->CountProductsCategoryBoughtedByUser(*j, i->GetID());
			
				if (was_bought)
					file << "+1 ";
				else
					file << "-1 ";

				float normalize_product_visit = (float) product_visit / (float) max_product_visit;
				float normalize_category_visit = (float) category_visit / (float) max_category_visit;
				float normalize_all_visit = (float)all_visit / (float)max_all_visit;
				float normalize_all_boughts = (float)all_boughts / (float)max_all_boughts;
				float normalize_category_boughts = (float)category_boughts / (float)max_category_boughts;

				file << "1:" << normalize_product_visit << " " 
					 << "2:" << normalize_category_visit << " "
					 << "3:" << normalize_all_visit << " "
					 << "4:" << normalize_all_boughts << " "
					 << "5:" << normalize_category_boughts << " "
					 << "# " << i->GetID() << ";" << *j <<  "\n";
			}
		}

		file << endl;

		file.close();
	}
	else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;
}

void Classator::PreperLearingBFile(std::string file_name)
{
	std::fstream file;
	file.open(file_name, std::ios::out);
	if (file.good() == true)
	{
		std::cout << "Uzyskano dostep do pliku!" << std::endl;

		for (list<User>::iterator i = learningB.begin(); i != learningB.end(); i++)
		{
			vector<int> products = dba->GetProductsVisitedByUser(i->GetID());

			for (vector<int>::iterator j = products.begin(); j != products.end(); j++)
			{
				bool was_bought = dba->WasProductBoughtByUser(*j, i->GetID());

				int product_visit = dba->CountProductVisitByUser(*j, i->GetID());
				int category_visit = dba->CountProductsCategoryVisitByUser(*j, i->GetID());
				int all_visit = dba->CountAllProductsVisitByUser(i->GetID());
				int all_boughts = dba->CountAllProductsBoughtedByUser(*j, i->GetID());
				int category_boughts = dba->CountProductsCategoryBoughtedByUser(*j, i->GetID());

				if (was_bought)
					file << "+1 ";
				else
					file << "-1 ";

				float normalize_product_visit = (float)product_visit / (float)max_product_visit;
				float normalize_category_visit = (float)category_visit / (float)max_category_visit;
				float normalize_all_visit = (float)all_visit / (float)max_all_visit;
				float normalize_all_boughts = (float)all_boughts / (float)max_all_boughts;
				float normalize_category_boughts = (float)category_boughts / (float)max_category_boughts;

				file << "1:" << normalize_product_visit << " "
					<< "2:" << normalize_category_visit << " "
					<< "3:" << normalize_all_visit << " "
					<< "4:" << normalize_all_boughts << " "
					<< "5:" << normalize_category_boughts << " "
					<< "# " << i->GetID() << ";" << *j << "\n";
			}
		}

		file << endl;

		file.close();
	}
	else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;
}

void Classator::PreperTestAFile(std::string file_name)
{
	std::fstream file;
	file.open(file_name, std::ios::out);
	if (file.good() == true)
	{
		std::cout << "Uzyskano dostep do pliku!" << std::endl;

		for (list<User>::iterator i = testA.begin(); i != testA.end(); i++)
		{
			vector<int> products = dba->GetProductsVisitedByUser(i->GetID());

			for (vector<int>::iterator j = products.begin(); j != products.end(); j++)
			{
				bool was_bought = dba->WasProductBoughtByUser(*j, i->GetID());

				int product_visit = dba->CountProductVisitByUser(*j, i->GetID());
				int category_visit = dba->CountProductsCategoryVisitByUser(*j, i->GetID());
				int all_visit = dba->CountAllProductsVisitByUser(i->GetID());
				int all_boughts = dba->CountAllProductsBoughtedByUser(*j, i->GetID());
				int category_boughts = dba->CountProductsCategoryBoughtedByUser(*j, i->GetID());

				float normalize_product_visit = (float)product_visit / (float)max_product_visit;
				float normalize_category_visit = (float)category_visit / (float)max_category_visit;
				float normalize_all_visit = (float)all_visit / (float)max_all_visit;
				float normalize_all_boughts = (float)all_boughts / (float)max_all_boughts;
				float normalize_category_boughts = (float)category_boughts / (float)max_category_boughts;

				file << "0 ";

				file << "1:" << normalize_product_visit << " "
					<< "2:" << normalize_category_visit << " "
					<< "3:" << normalize_all_visit << " "
					<< "4:" << normalize_all_boughts << " "
					<< "5:" << normalize_category_boughts << " "
					<< "# " << i->GetID() << ";" << *j << "\n";
			}
		}

		file << endl;

		file.close();
	}
	else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;
}

void Classator::PreperTestBFile(std::string file_name)
{
	std::fstream file;
	file.open(file_name, std::ios::out);
	if (file.good() == true)
	{
		std::cout << "Uzyskano dostep do pliku!" << std::endl;

		for (list<User>::iterator i = testB.begin(); i != testB.end(); i++)
		{
			vector<int> products = dba->GetProductsVisitedByUser(i->GetID());

			for (vector<int>::iterator j = products.begin(); j != products.end(); j++)
			{
				bool was_bought = dba->WasProductBoughtByUser(*j, i->GetID());

				int product_visit = dba->CountProductVisitByUser(*j, i->GetID());
				int category_visit = dba->CountProductsCategoryVisitByUser(*j, i->GetID());
				int all_visit = dba->CountAllProductsVisitByUser(i->GetID());
				int all_boughts = dba->CountAllProductsBoughtedByUser(*j, i->GetID());
				int category_boughts = dba->CountProductsCategoryBoughtedByUser(*j, i->GetID());

				float normalize_product_visit = (float)product_visit / (float)max_product_visit;
				float normalize_category_visit = (float)category_visit / (float)max_category_visit;
				float normalize_all_visit = (float)all_visit / (float)max_all_visit;
				float normalize_all_boughts = (float)all_boughts / (float)max_all_boughts;
				float normalize_category_boughts = (float)category_boughts / (float)max_category_boughts;

				file << "0 ";

				file << "1:" << normalize_product_visit << " "
					<< "2:" << normalize_category_visit << " "
					<< "3:" << normalize_all_visit << " "
					<< "4:" << normalize_all_boughts << " "
					<< "5:" << normalize_category_boughts << " "
					<< "# " << i->GetID() << ";" << *j << "\n";
			}
		}

		file << endl;

		file.close();
	}
	else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;
}

void Classator::PrintLearningGroupA()
{
	cout << "Learning Group" << endl;
	for (list<User>::iterator i = learningA.begin(); i != learningA.end(); i++)
		cout << (*i) << endl;
}

void Classator::PrintLearningGroupB()
{
	cout << "Learning Group" << endl;
	for (list<User>::iterator i = learningB.begin(); i != learningB.end(); i++)
		cout << (*i) << endl;
}

void Classator::PrintTestGroupA()
{
	cout << "Test Group" << endl;
	for (list<User>::iterator i = testA.begin(); i != testA.end(); i++)
		cout << (*i) << endl;
}

void Classator::PrintTestGroupB()
{
	cout << "Test Group" << endl;
	for (list<User>::iterator i = testB.begin(); i != testB.end(); i++)
		cout << (*i) << endl;
}

void Classator::SetA(double new_a)
{
	if (new_a > 0)
		a = new_a;
	else
		cout << "Incorrect 'a' paramiter: a must be > 0!" << endl;
}

void Classator::SetB(double new_b)
{
	if (new_b > 0)
		b = new_b;
	else
		cout << "Incorrect 'b' paramiter: b must be > 0!" << endl;
}

void Classator::SetC(double new_c)
{
	if (new_c > 0)
		c = new_c;
	else
		cout << "Incorrect 'c' paramiter: c must be > 0!" << endl;
}

Classator::~Classator()
{
	delete dba;
}
