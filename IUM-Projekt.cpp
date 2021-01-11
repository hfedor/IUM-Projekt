// IUM-Projekt.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "DBAccess.h"

using namespace std;

int main()
{
    DBAccess  dba("IUM_Projekt.db");

    dba.AddSessionToDB(4, "timestamp", 2, 2, "event_type", "category_path");

    cout << "timestamp:" << dba.GetTimestamp(4) << endl;
    cout << "GetCategoryPath" << dba.GetCategoryPath(4) << endl;
    cout << dba.GetEventType(4) << endl;
    cout << dba.GetProductID(4) << endl;
    cout << dba.GetUserID(4) << endl;
    
    //dba.ClearSessionsTable();

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
