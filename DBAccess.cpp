#include "DBAccess.h"

using namespace std;

// NODES:
	// CONSTRUCTOR:
DBAccess::DBAccess(string db_file_name)
{
	int rc = sqlite3_open(db_file_name.c_str(), &db);
	
	if( rc )
	{
		string message = "Can't open database: ";
		message += sqlite3_errmsg(db);
		cout << "DBAccess: " + message << endl;
	}
	else
	{
		BuildSessionsTable();
	}
}

// TASKS INFO COPIES:
	// ADD SESSION:
int  DBAccess::AddSessionToDB(int session_id, std::string timestamp, int user_id, int product_id, std::string event_type, std::string category_path)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM sessions WHERE session_id = ? AND timestamp = ? AND user_id = ? AND product_id = ? AND event_type = ? AND category_path = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 

	sqlite3_bind_int(stmt, 1, session_id);
	sqlite3_bind_text(stmt, 2,timestamp.c_str(),-1,0);
	sqlite3_bind_int(stmt, 3, user_id);
	sqlite3_bind_int(stmt, 4, product_id);
	sqlite3_bind_text(stmt, 5, event_type.c_str(), -1, 0);
	sqlite3_bind_text(stmt, 6, category_path.c_str(), -1, 0);

	if( sqlite3_step( stmt ) == SQLITE_ROW ) 
		isInTable = true;

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddSessionToDB (IsAlreadyInTable) error:" << endl;
		return -1;
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
		cout << "AddSessionToDB (IsAlreadyInTable) error:" << endl;
	else if(isInTable)
	{
		cout << "AddSessionToDB (IsAlreadyInTable) error: Recored already in table." << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "INSERT INTO sessions (session_id, timestamp, user_id, product_id, event_type, category_path) VALUES (?,?,?,?,?,?)";

	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt, 1, session_id);
	sqlite3_bind_text(stmt, 2, timestamp.c_str(), -1, 0);
	sqlite3_bind_int(stmt, 3, user_id);
	sqlite3_bind_int(stmt, 4, product_id);
	sqlite3_bind_text(stmt, 5, event_type.c_str(), -1, 0);
	sqlite3_bind_text(stmt, 6, category_path.c_str(), -1, 0);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{;
		sqlite3_free(zErrMsg);
		cout << "AddSessionToDB error:" << endl;
	}
	
	rc = sqlite3_finalize( stmt );
   
	if( rc != SQLITE_OK ){
		cout << "AddSessionToDB error:" << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "SELECT session_id FROM sessions WHERE session_id = " + to_string(session_id) + " AND timestamp = \'" + timestamp + "\' AND user_id = " + to_string(user_id) + " AND product_id = " + to_string(product_id) + " AND event_type = \'" + event_type + "\' AND category_path = \'" + category_path + "\' ;";
	
	int newId;
	
	rc = sqlite3_exec(db, sql.c_str(), GetIdFromDB,&newId, &zErrMsg);
	
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "SQL GetIdFromDB error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	} 
	else
			return newId;
		
   return -1;
}	

bool	DBAccess::BuildSessionsTable()
{	
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;
	
	/* Create SQL statement */
	sql = "CREATE TABLE if not exists [sessions] ([session_Id] INTEGER NOT NULL, [timestamp] TEXT NOT NULL, [user_id] INTEGER NULL, [product_id] INTEGER NOT NULL, [event_type] TEXT NOT NULL, [category_path] TEXT NOT NULL, CONSTRAINT[PK_sessions] PRIMARY KEY([session_Id])); ";
	
	rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

	if( rc != SQLITE_OK ){
		cout << "BuildNounsTable error: CREATE" << endl;
		return false;
	}
	else
		return true;
}

bool DBAccess::ClearSessionsTable()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;

	sql = "DELETE FROM sessions";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "ClearSessionsTable error:" << endl;
		return false;	
	}
	
	sql = "VACUUM";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK ){
		cout << "ClearSessionsTable error:" << endl;
		return false;
	}
	
	return true;
}

std::string DBAccess::GetTimestamp(int id)
{
	return GetSessionsRecord(id,1);
}

int DBAccess::GetUserID(int id)
{
	return atoi(GetSessionsRecord(id,2).c_str());
}

int DBAccess::GetProductID(int id)
{
	return atoi(GetSessionsRecord(id, 3).c_str());
}

std::string DBAccess::GetEventType(int id)
{
	return GetSessionsRecord(id, 4);
}

std::string DBAccess::GetCategoryPath(int id)
{
	return GetSessionsRecord(id, 5);
}

string DBAccess::GetSessionsRecord(int id, int column)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	string result;
	sqlite3_stmt* stmt = 0;

	sql = "SELECT * FROM sessions WHERE session_id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, column);
		if(tmp == NULL)
			result = "0";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetSessionsRecord (IsAlreadyInTable) error:" << endl;
		return "0";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK ){
		cout << "GetSessionsRecord error:" << endl;
		return "0";
	}
	else 
		return result;
}

std::vector<int> DBAccess::GetSessionsIndexes()
{
	char* zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;
	vector<int> ids;

	sql = "SELECT session_id FROM sessions";

	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

	sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

	while (sqlite3_step(stmt) == SQLITE_ROW)
		ids.push_back(sqlite3_column_int(stmt, 0));

	sqlite3_step(stmt);
	sqlite3_clear_bindings(stmt);
	sqlite3_reset(stmt);

	rc = sqlite3_exec(db, "END TRANSACTION", 0, 0, &zErrMsg);   //  End the transaction.

	if (rc != SQLITE_OK)
	{
		sqlite3_free(zErrMsg);
		cout << "GetSessionsIndexes error:" << endl;
		return ids;
	}

	rc = sqlite3_finalize(stmt);

	if (rc != SQLITE_OK)
		cout << "GetSessionsIndexes error:" << endl;
	else
		return ids;
}

std::ostream & DBAccess::GetSessionsInfoFromDB(std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT session_id, timestamp, user_id, product_id, event_type, category_path FROM sessions;";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetSessionsInfoFromDB error:" << endl;
    }
	return out;
}

std::ostream & DBAccess::GetSessionInfoFromDB(int id, std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT session_id, timestamp, user_id, product_id, event_type, category_path WHERE session_id = " + to_string(id) + ";";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetSessionInfoFromDB error:" << endl;
	}
	
	return out;
}

string DBAccess::RandomNounFromDB()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	int table_size;
	sqlite3_stmt* stmt = 0;
	string result;
	
	// Create SQL statement
	sql = "SELECT COUNT(*) FROM sessions";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
		table_size = sqlite3_column_int(stmt, 0);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomNounFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomNounFromDB : Get rable size" << endl;
		return "";
	}
	
	int x = RandID(table_size);
	
	// Create SQL statement
	sql = "SELECT noun FROM nouns WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 

	sqlite3_bind_int(stmt,1,x);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, 0);
		if(tmp == NULL)
			result = "";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomNounFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomNounFromDB error: Get record by index." << endl;
		return "";
	}
	
	return result;
}

	// REMOVE TASKS:
bool DBAccess::RemoveSessionFromDB(int id)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM sessions WHERE session_id = ?";
	
	rc = sqlite3_exec(db, sql.c_str(), IsAlreadyInTable,&isInTable, &zErrMsg);
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RemoveSessionFromDB (IsAlreadyInTable) error:" << endl;
		return false;
    }
	
	// Create SQL statement
	sql = "DELETE FROM sessions WHERE session_id = '" + to_string(id) + "';";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RemoveSessionFromDB (IsAlreadyInTable) error:"  << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
	  
	if( rc != SQLITE_OK ){
		cout << "RemoveSessionFromDB error:" << endl;
		return false;
	}
	else
		return true;
}

	// SET VALUES:
void DBAccess::SetTimestamp(int id, std::string timestamp)
{
	SetSessionRecord(id, 1, "timestamp", timestamp);
}

void DBAccess::SetUserID(int id, int user_id)
{
	SetSessionRecord(id, 1, "user_id", to_string(user_id));
}

void DBAccess::SetProductID(int id, int product_id)
{
	SetSessionRecord(id, 1, "product_id", to_string(product_id));
}

void DBAccess::SetEventType(int id, std::string event_type)
{
	SetSessionRecord(id, 1, "event_type", event_type);
}

void DBAccess::SetCategoryPath(int id, std::string category_path)
{
	SetSessionRecord(id, 1, "category_path", category_path);
}

void DBAccess::SetSessionRecord(int id, int columnx, std::string column, std::string new_value)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM sessions WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char *tmp = sqlite3_column_text( stmt, columnx );
		string tmp2 = std::string(reinterpret_cast<const char*>(tmp));
    }
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddNounToDB (IsAlreadyInTable) error:" << endl;
		
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK ){
		cout << "SetSessionRecord (IsAlreadyInTable) error:" << endl;
		return;
	}
	else if(isInTable){
		cout << "SetSessionRecord (IsAlreadyInTable) error: Recored already has given value" << endl;
		return;
	}
	sql = "UPDATE session SET " + column + " = ?  WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_text(stmt,1,new_value.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,id);
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "SetSessionRecord (IsAlreadyInTable) error:" << endl;
		return;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "SetNounRecord error:" << endl;
}

// USED IN OTHER FUNCTION:
int DBAccess::IsAlreadyInTable(void* isInTable, int argc, char **argv, char **azColName)
{	
	bool* tmp = (bool*) isInTable;
	
	if(argc >= 0)
		*tmp = true;
	else 
		*tmp = false;
	return 0;
}

int DBAccess::Callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int DBAccess::GetIdFromDB(void *id, int argc, char **argv, char **azColName)
{
	int* tmp;
	tmp = (int*) id;
	*tmp = atoi(argv	[0]);
	return 0;
}

int DBAccess::PrintFromDBToFile(void *file, int argc, char **argv, char **azColName) 
{
	int i;
	fstream* tmp;
	tmp = (fstream*) file;
	
	for(i = 0; i<argc; i++)
		*tmp << azColName[i] << " = "  << (argv[i] ? argv[i] : "NULL") << "\n";
	
	*tmp << endl;
	
	return 0;
}

int DBAccess::PrintFromDBToStream(void *out, int argc, char **argv, char **azColName) 
{
	int i;
	ostream* tmp;
	tmp = (ostream*) out;
	
	for(i = 0; i<argc; i++)
		*tmp << azColName[i] << " = "  << (argv[i] ? argv[i] : "NULL") << "\n";
	
	*tmp << endl;
	
	return 0;
}


	// GET VALUES FROM RECORDS:
int DBAccess::GetRecordFromDB(void *record, int argc, char **argv, char **azColName) 
{
	string* tmp;
	tmp = (string*) record;
	if(argv[0] == NULL)
		*tmp = "NULL";
	else
		*tmp = argv[0];
	return 0;
}	

DBAccess::~DBAccess()
{
	sqlite3_close(db);
}

int DBAccess::RandID(int table_size)
{
	int x;
	float y;
	std::random_device rd;
    std::default_random_engine e2(rd()) ;
    std::uniform_real_distribution<> dist(0, 1);
	
	y = dist(e2);
	//cout << "ts = " << table_size << endl;
	//cout << "y = " << y << endl;
	//cout << "ts^(1/6) = " << pow((double)y,((float)1.0)/(float(7.0))) << endl;
	
	x = (int) table_size*(pow((double)y,((float)1.0)/(float(7.0))) );
	//cout << "x = " << x << endl;
	
	return x;
}