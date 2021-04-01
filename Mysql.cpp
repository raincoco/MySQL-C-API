#include"mysqlc.h"

/*---------------------------------------------------------------
  ---------------------------------------------------------------
  -------------------------- 数据库连接 --------------------------
  ---------------------------------------------------------------
  ---------------------------------------------------------------*/

Mysql_Operation::Mysql_Operation()
{
	m_mysql = mysql_init(NULL);
}

bool Mysql_Operation::Mysql_connect()
{
	mysql_real_connect(m_mysql,		//MYSQL * mysql
		"127.0.0.1",				//const char* host，主机名或IP地址。如果NULL或”localhost”，与本地主机的连接。
		"user",						//const char* user，MySQL登录ID。如果NULL或空字符串，视为当前用户。
		"passwd",					//const char* passwd，用户的密码。如果passwd是NULL，仅会对该用户的（拥有1个空密码字段的）用户表中的条目进行匹配检查。
		"dbname",					//const char* dbname，数据库名称。如果dbname为NULL，连接会将默认的数据库设为该值。 
		0,							//unsigned int port，TCP/IP连接的端口号。
		NULL,						//const char* unix_socket，如果unix_socket不是NULL，该字符串描述了应使用的套接字或命名管道。
		0							//unsigned long client_flag，client_flag的值通常为0。
	);

	if (m_mysql == NULL)
	{
		printf("mysql_real_connect is failed");
		return false;
	}

	if (mysql_query(m_mysql, "set names gbk"))  //语言设置
	{
		return mysql_errno(m_mysql);
	}

	printf("连接成功\n");
	return true;
}

/*---------------------------------------------------------------
  ---------------------------------------------------------------
  -------------------------- 数据库操作 --------------------------
  ---------------------------------------------------------------
  ---------------------------------------------------------------*/

  /* 增删查 */
  /*-------------------------- 插入数据 --------------------------*/
bool Mysql_Operation::Insert(const char* table, const char* info)
{
	string sqlstr = "INSERT INTO ";
	sqlstr += table;
	sqlstr += " values(";
	sqlstr += info;
	sqlstr += ");";
	cout << sqlstr << endl;

	/* mysql_query，执行由“Null终结的字符串”查询指向的SQL查询。成功，返回0；错误，返回非0值。*/
	int ret = mysql_query(m_mysql, sqlstr.c_str());

	if (ret != 0)
	{
		printf("error3\n");
		return false;
	}

	return true;
}

/*-------------------------- 删除数据 --------------------------*/
bool Mysql_Operation::Delete(char* table, char* info)
{
	string sqlstr = "DELETE FROM ";
	sqlstr += table;
	sqlstr += "WHERE(";
	sqlstr += info;
	sqlstr += ");";
	cout << sqlstr << endl;

	int ret = mysql_query(m_mysql, sqlstr.c_str());

	if (ret != 0)
	{
		return false;
	}
	return true;
}

/*-------------------------- 查询数据 --------------------------*/
bool Mysql_Operation::Select(const char* table, int SelectChoose)
{
	string sqlstr;

	sqlstr = "SELECT * FROM ";
	sqlstr += table;

	if (SelectChoose==1)
	{
		cout << sqlstr << endl;
	}
	else if (SelectChoose == 2)
	{
		sqlstr += " WHERE attribute IN('values');"; // 条件查询 
		cout << sqlstr << endl;

	int ret = mysql_query(m_mysql, sqlstr.c_str());

	if (ret != 0)
	{
		return false;
	}

	/* mysql_store_result函数，获取查询结果的一种方式，将从Mysql服务器查询的所有数据都存储到客户端，然后读取。
	   MYSQL_RES 该结构代表返回行的查询结果（SELECT, SHOW, DESCRIBE, EXPLAIN），一般将查询返回的信息称为“结果集”。*/
	MYSQL_RES* res = mysql_store_result(m_mysql);

	if (res == NULL)
	{
		return false;
	}

	int num_fields = mysql_num_fields(res);	//返回结果集的行数

	/* MYSQL_FIELD结构包含关于字段的信息，如字段名、类型和大小。通过重复调用mysql_fetch_field，可为每个字段获得MYSQL_FIELD结构。*/
	MYSQL_FIELD* field;
	MYSQL_ROW rows;	//结果集的下一行

	/* mysql_fetch_field()函数从结果集中取得列信息并作为对象返回。*/
	while (field = mysql_fetch_field(res))
	{
		cout << field->name << ' ';
	}
	cout << endl;

	/* mysql_fetch_row()函数从检索集的下一行，num_fields行数。*/
	while (rows = mysql_fetch_row(res))
	{
		int i = 0;
		for (i = 0; i < num_fields; i++)
		{
			if (rows[i] == NULL)
			{
				cout << "null" << ' ';
				continue;
			}
			cout << rows[i] << ' ';
		}
		cout << endl;
	}

	/* mysql_free_result()函数释放结果集，以防内存泄露。*/
	mysql_free_result(res);
	return true;
}
