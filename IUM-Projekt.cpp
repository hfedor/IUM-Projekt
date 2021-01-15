// IUM-Projekt.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "DBAccess.h"
#include "Classator.h"

using namespace std;

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

    classator.PreperLearingAFile("learning_fileA2.dab");
    classator.PreperTestAFile("test_fileA2.dab");
    classator.PreperLearingBFile("learning_fileB2.dab");
    classator.PreperTestBFile("test_fileB2.dab");

    //dba.ClearSessionsTable();

    //classator.FindMaxs();

    std::cout << "Hello World!\n";
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
