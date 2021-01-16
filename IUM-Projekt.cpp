// IUM-Projekt.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "DBAccess.h"
#include "Classator.h"
#include <string>

using namespace std;

/*
int main()
{
    DBAccess  dba("IUM_Projekt.db");

    //dba.AddSessionToDB(3, "timestamp", 7, 22, "event_type", "category_path");

    //cout << "timestamp:" << dba.GetTimestamp(4) << endl;
    //cout << "GetCategoryPath" << dba.GetCategoryPath(4) << endl;
    //cout << dba.GetEventType(4) << endl;
    //cout << dba.GetProductID(4) << endl;
    //cout << dba.GetUserID(4) << endl;
    
    vector<int> users = dba.GetUserIDs();

    //cout << "Users:" << endl;
    //for (vector<int>::iterator i = users.begin(); i != users.end(); i++)
        //cout << (*i) << endl;

    Classator classator;
    classator.GroupUsers(users, 0.25);
    cout << "Learning A:" << endl;
    classator.PrintLearningGroupA();
    cout << "Learning B:" << endl;
    classator.PrintLearningGroupB();
    cout << "Test A:" << endl;
    classator.PrintTestGroupA();
    cout << "Test B:" << endl;
    classator.PrintTestGroupB();

    cout << "Product vists count:" << dba.CountProductVisitByUser(1315,102) << endl;
    cout << "Category visits count:" << dba.CountProductsCategoryVisitByUser(1315, 102) << endl;
    cout << "All visits count:" << dba.CountAllProductsVisitByUser(102) << endl;
    cout << "Category boughts count:" << dba.CountProductsCategoryBoughtedByUser(1318, 146) << endl;
    cout << "All boughts count:" << dba.CountAllProductsBoughtedByUser(1277, 102) << endl;

    cout << "Was product: 1315 bought by user: 102?: " << dba.WasProductBoughtByUser(1315, 102) << endl;
    cout << "Was product: 1293 bought by user: 102?: " << dba.WasProductBoughtByUser(1293, 102) << endl;

    cout << "Products visited by user: 102:" << endl;
    vector<int> products = dba.GetProductsVisitedByUser(102);
    //for (vector<int>::iterator i = products.begin(); i != products.end(); i++)
        //cout << (*i) << endl;

    //classator.PreperLearingAFile("learning_fileA3.dab");
    //classator.PreperTestAFile("test_fileA3.dab");
    //classator.PreperLearingBFile("learning_fileB3.dab");
    //classator.PreperTestBFile("test_fileB3.dab");

    classator.CheckModel("test_fileA2.dab","output_fileA2.txt");

    //dba.ClearSessionsTable();

    //classator.FindMaxs();

    std::cout << "Hello World!\n";
}

*/

int main(int argc, char* argv[])
{
    DBAccess  dba("IUM_Projekt.db");
    if (argc < 2)
    {
        perror("Not enough parameters.");
        exit(0);
    }

    string mode = argv[1];
    if (mode == "1")                                                                //TRYB PREDYKCJI UZYTKOWNIKA
    {
        cout << argc << endl;
        if (argc != 5)
        {
            perror("Bad number of parameters for this mode (4 needed).");
            exit(0);
        }

        int product = atoi(argv[3]);
        int user = atoi(argv[4]);

        Classator c;
        c.PreperUserProductTestFile(product, user);

        fstream output_file;
        
        string model = argv[2];
        if (model == "1")
        {
            system("svm_classify.exe user_test_file.dab model_fileA.dab output_fileA.dab");
            output_file.open("output_fileA.dab", std::ios::in);
        }
        else if (model == "2")
        {
            system("svm_classify.exe user_test_file.dab model_fileB.dab output_fileB.dab");
            output_file.open("output_fileB.dab", std::ios::in);
        }

        if (output_file.good() == true)
        {
            float tmp_float;
            output_file >> tmp_float;
            cout << "predistion: " << tmp_float << endl;
            if(tmp_float > 0)
                cout << "User: " << user << " is intrested in product: " << product << "." << endl;
            else
                cout << "User: " << user << " is NOT intrested in product: " << product << "." << endl;
        }
        else std::cout << "Can't open output file!" << std::endl;
    }
    else if (mode =="2")                                                   // TRYB TESTOW AB
    {
        if (argc != 3)
        {
            perror("Not enough parameters for this mode.");
            exit(0);
        }
        string testData;
        testData = argv[2];

        ifstream testDataFile;

        string cmd;

        Classator c;

        cmd = "svm_classify.exe " + testData + " model_fileA.dab output_fileA.dab";

        system(cmd.c_str());

        cout << "\nModel A results:" << endl;

        c.CheckModel(testData, "output_fileA.dab");

        cmd = "svm_classify.exe " + testData + " model_fileB.dab output_fileB.dab";

        system(cmd.c_str());

        cout << "\nModel B results:" << endl;

        c.CheckModel(testData, "output_fileB.dab");
    }
    else
    {
        perror("Wrong 1st parameter");
        exit(0);
    }
    return 0;
}


// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
