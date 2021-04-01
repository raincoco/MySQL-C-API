#include<stdio.h>
#include<string.h>
#include<string>
#include <iostream>
#include <mysql.h>
#include <sql.h>

class Mysql_Operation
{
public:
	MYSQL* m_mysql;

	Mysql_Operation();
	///~Mysql_Operation();

	bool Mysql_connect();
	bool Insert(const char* table, const char* info);
	bool Delete(char* table, char* info);
	bool Update(char* table, char* info);
	bool Select(const char* table, int SelectChoose);
};