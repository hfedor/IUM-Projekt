#ifndef DBACCESS_H
#define DBACCESS_H

#include <iostream>
#include "sqlite/sqlite3.h" 
#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <random>
#include <chrono>
/*
	Class to get access to database
*/

class DBAccess
{
	private:
		sqlite3 *db;
		
		// USED IN OTHER FUNCTION
		static int IsAlreadyInTable(void *NotUsed, int argc, char **argv, char **azColName); // checking if node is already in database
		static int Callback(void *NotUsed, int argc, char **argv, char **azColName);
		static int GetIdFromDB(void *NotUsed, int argc, char **argv, char **azColName);
		static int GetRecordFromDB(void *record, int argc, char **argv, char **azColName);
		static int PrintFromDBToStream(void *out, int argc, char **argv, char **azColName);
		static int PrintFromDBToFile(void *file, int argc, char **argv, char **azColName);
		int RandID(int table_size);

	public:
		// FUNCTIONS TO CONNECT WITH DATEBASE:
		
		// TASK INFO:
		DBAccess(std::string db_file_name);
		int AddSessionToDB(int session_id, std::string timestamp, int user_id, int product_id, std::string event_type, std::string category_path); // add session with given datas to database
		bool BuildSessionsTable(); // build  sessions table
		bool ClearSessionsTable(); // remove all records from sessions table
		std::string GetTimestamp(int session_id); // get timestamp of session with given ID
		int GetUserID(int session_id); // get user_id of session with given ID
		int GetProductID(int session_id); // get product_id of session with given ID
		std::string GetEventType(int session_id); // get event type of session with given ID
		std::string GetCategoryPath(int session_id); // get category path of session with given ID
		std::string GetSessionsRecord(int session_id, int column);  // get record from given column from node with given ID
		std::vector<int> GetSessionsIndexes(); // get vector of tasks indexes
		std::ostream & GetSessionsInfoFromDB(std::ostream &out); // get Inforamtions about all tasks and put it to given stream
		std::ostream & GetSessionInfoFromDB(int session_id, std::ostream &out); // get Inforamtions about noun with given ID and put it to given stream
		std::string RandomNounFromDB();
		bool RemoveSessionFromDB(int noun_id); // remove noun with given ID from database and return if removing sucesed
		void SetTimestamp(int session_id, std::string timestamp); // set new path to file of noun copy with gicen ID
		void SetUserID(int session_id, int user_id); // set new path to file of noun copy with gicen ID
		void SetProductID(int session_id, int product_id); // set new path to file of noun copy with gicen ID
		void SetEventType(int session_id, std::string event_type); // set new path to file of noun copy with gicen ID
		void SetCategoryPath(int session_id, std::string category_path); // set new path to file of noun copy with gicen ID
		void SetSessionRecord(int noun_id, int columnx, std::string column, std::string new_value); // set new given value of given column in noun with gicen ID
		
		~DBAccess();
};
	
#endif // PMDATEBASEACCESS_HPP