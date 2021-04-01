#include"mysqlc.h"

/*---------------------------------------------------------------
  ---------------------------------------------------------------
  -------------------------- ���ݿ����� --------------------------
  ---------------------------------------------------------------
  ---------------------------------------------------------------*/

Mysql_Operation::Mysql_Operation()
{
	m_mysql = mysql_init(NULL);
}

bool Mysql_Operation::Mysql_connect()
{
	mysql_real_connect(m_mysql,		//MYSQL * mysql
		"127.0.0.1",				//const char* host����������IP��ַ�����NULL��localhost�����뱾�����������ӡ�
		"user",						//const char* user��MySQL��¼ID�����NULL����ַ�������Ϊ��ǰ�û���
		"passwd",					//const char* passwd���û������롣���passwd��NULL������Ը��û��ģ�ӵ��1���������ֶεģ��û����е���Ŀ����ƥ���顣
		"dbname",					//const char* dbname�����ݿ����ơ����dbnameΪNULL�����ӻὫĬ�ϵ����ݿ���Ϊ��ֵ�� 
		0,							//unsigned int port��TCP/IP���ӵĶ˿ںš�
		NULL,						//const char* unix_socket�����unix_socket����NULL�����ַ���������Ӧʹ�õ��׽��ֻ������ܵ���
		0							//unsigned long client_flag��client_flag��ֵͨ��Ϊ0��
	);

	if (m_mysql == NULL)
	{
		printf("mysql_real_connect is failed");
		return false;
	}

	if (mysql_query(m_mysql, "set names gbk"))  //��������
	{
		return mysql_errno(m_mysql);
	}

	printf("���ӳɹ�\n");
	return true;
}

/*---------------------------------------------------------------
  ---------------------------------------------------------------
  -------------------------- ���ݿ���� --------------------------
  ---------------------------------------------------------------
  ---------------------------------------------------------------*/

  /* ��ɾ�� */
  /*-------------------------- �������� --------------------------*/
bool Mysql_Operation::Insert(const char* table, const char* info)
{
	string sqlstr = "INSERT INTO ";
	sqlstr += table;
	sqlstr += " values(";
	sqlstr += info;
	sqlstr += ");";
	cout << sqlstr << endl;

	/* mysql_query��ִ���ɡ�Null�ս���ַ�������ѯָ���SQL��ѯ���ɹ�������0�����󣬷��ط�0ֵ��*/
	int ret = mysql_query(m_mysql, sqlstr.c_str());

	if (ret != 0)
	{
		printf("error3\n");
		return false;
	}

	return true;
}

/*-------------------------- ɾ������ --------------------------*/
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

/*-------------------------- ��ѯ���� --------------------------*/
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
		sqlstr += " WHERE attribute IN('values');"; // ������ѯ 
		cout << sqlstr << endl;

	int ret = mysql_query(m_mysql, sqlstr.c_str());

	if (ret != 0)
	{
		return false;
	}

	/* mysql_store_result��������ȡ��ѯ�����һ�ַ�ʽ������Mysql��������ѯ���������ݶ��洢���ͻ��ˣ�Ȼ���ȡ��
	   MYSQL_RES �ýṹ�������еĲ�ѯ�����SELECT, SHOW, DESCRIBE, EXPLAIN����һ�㽫��ѯ���ص���Ϣ��Ϊ�����������*/
	MYSQL_RES* res = mysql_store_result(m_mysql);

	if (res == NULL)
	{
		return false;
	}

	int num_fields = mysql_num_fields(res);	//���ؽ����������

	/* MYSQL_FIELD�ṹ���������ֶε���Ϣ�����ֶ��������ͺʹ�С��ͨ���ظ�����mysql_fetch_field����Ϊÿ���ֶλ��MYSQL_FIELD�ṹ��*/
	MYSQL_FIELD* field;
	MYSQL_ROW rows;	//���������һ��

	/* mysql_fetch_field()�����ӽ������ȡ������Ϣ����Ϊ���󷵻ء�*/
	while (field = mysql_fetch_field(res))
	{
		cout << field->name << ' ';
	}
	cout << endl;

	/* mysql_fetch_row()�����Ӽ���������һ�У�num_fields������*/
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

	/* mysql_free_result()�����ͷŽ�������Է��ڴ�й¶��*/
	mysql_free_result(res);
	return true;
}
