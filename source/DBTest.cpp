// #include <stdio.h>
// #include <stdlib.h>
// #ifndef __linux__
// #include <windows.h>
// #endif
// #include <string>
// #include <iostream>
// #include <fstream>
// #include <direct.h>
// #include <regex>
// #ifndef __linux__
// #include <boost\thread.hpp>
// #else
// #include <boost/thread.hpp>
// #include"Util/StringUtil.h"
// #endif
// #include "DBLayer.h"
// #include "Log.h"
// #include "BeeBase/Log/Log.h"
// #include "BeeBase/Util/StringUtil.h"
// #include "BeeBase/Util/FileUtil.h"
// #pragma warning( disable : 4996 )
//
// #ifndef __linux__
// //#import "msado15.dll" no_namespace rename("EOF", "adoEOF") rename("BOF", "adoBOF")
//
// using namespace std;
// using namespace db_layer;
//
// int dblayer_conn_number = 0;
//
// //std::string SQLServer_TCHAR2STRING(const TCHAR* str_res)
// //{
// //	std::string str_dst{ "" };
// //	try
// //	{
// //		int iLen = WideCharToMultiByte(CP_ACP, 0, str_res, -1, NULL, 0, NULL, NULL);
// //		char* chRtn = new char[iLen * sizeof(char)];
// //		WideCharToMultiByte(CP_ACP, 0, str_res, -1, chRtn, iLen, NULL, NULL);
// //		str_dst = chRtn;
// //		delete[] chRtn;
// //		chRtn = nullptr;
// //	}
// //	catch (std::exception e)
// //	{
// //		std::string trans_err = e.what();
// //	}
// //
// //	return str_dst;
// //}
//
// shared_ptr<db_layer::CConnect> ApplyConn(const string& plugin_instance)
// {
// 	ULONGLONG start_time = GetTickCount64();
// 	db_layer::CConnect* conn = db_layer::DBLayer_ApplyConn(plugin_instance);
// 	if (conn == nullptr)
// 	{
// 		ULONGLONG end_time = GetTickCount64();
// 		printf("Time Out: %d \n", static_cast<int>(end_time - start_time));
// 		assert(0);
// 		return shared_ptr<db_layer::CConnect>();
// 	}
//
// 	dblayer_conn_number++;
// 	printf("Connect: %d \n", dblayer_conn_number);
//
// 	auto delete_func = [&](db_layer::CConnect* ptr_conn)
// 	{
// 		if (ptr_conn != nullptr)
// 		{
// 			db_layer::DBLayer_FreeConn(ptr_conn);
// 			dblayer_conn_number--;
// 			printf("Connect:%d \n", dblayer_conn_number);
// 		}
// 	};
//
// 	return shared_ptr<db_layer::CConnect>(conn, delete_func);
// }
//
// bool ExecuteQ(const string& sql, std::function<void(db_layer::CConnect&)> func, const string& owner_name)
// {
// 	shared_ptr<db_layer::CConnect> ptr_connect = ApplyConn(owner_name);
// 	if (!ptr_connect)
// 	{
// 		assert(0); // 没有申请到连接
// 		printf("Apply Conn Failed.");
// 		return false;
// 	}
//
// 	if (ptr_connect->ExecuteQuery(sql))
// 	{
// 		func(*ptr_connect);
// 	}
// 	else
// 	{
// 		string err_info("");
// 		int err_code;
// 		ptr_connect->GetLastError(err_code, err_info);
// 		assert(0);
// 		printf("Execute Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 		return false;
// 	}
//
// 	return true;
// }
//
// bool ExecuteH(const string& sql, const string& owner_name)
// {
// 	shared_ptr<db_layer::CConnect> ptr_connect = ApplyConn(owner_name);
// 	if (!ptr_connect)
// 	{
// 		assert(0); // 没有申请到连接
// 		printf("Apply Conn Failed.");
// 		return false;
// 	}
//
// 	if (!ptr_connect->Execute(sql))
// 	{
// 		string err_info("");
// 		int err_code;
// 		ptr_connect->GetLastError(err_code, err_info);
// 		assert(0);
// 		printf("Execute Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 		return false;
// 	}
//
// 	return true;
// }
//
// void DBLayerConnectMode_PostgreSQL()
// {
// 	DBParam param;
// 	param.db_type = DBType::PostgreSQL;
// 	param.ip = "10.19.181.104";
// 	param.port = 5432;
// 	param.user = "postgres";
// 	param.passwd = "ZjhhYTE0ZGEyMzAxZTIwMWU4MTdmNWI4NjY3YTM2YmI0MGM4Y2E0OWRhNjliMzQ3MGE3NGQwZjRlYzE5NDk2MQ==";
// 	param.db_name = "cms";
// 	param.min_conn = 1;
// 	param.max_conn = 1;
//
// 	// 初始化数据库接入层
// 	db_layer::DBLayer_Init(param);
//
// 	db_layer::Module_Register(param, "Plugin_A");
//
// 	bool k_conn = db_layer::DBLayer_Valid("Plugin_A");
//
// 	db_layer::CConnect* conn_A = db_layer::DBLayer_ApplyConn("Plugin_A");
// 	// bool count_ret = conn_A->ExecuteQueryUDT("select count(1) from platform.db_version;", 3);
// 	bool count_ret = conn_A->ExecuteQueryUDT("select count(1) from vehiclebiz.vehicle_data_log;");
//
// 	for (; count_ret && !conn_A->IsEOF(); conn_A->Next())
// 	{
// 		int count = conn_A->GetInt("count");
// 		cout << count << endl;
// 	}
//
// 	db_layer::DBLayer_FreeConn(conn_A);
//
// 	db_layer::CConnect* conn_B = db_layer::DBLayer_ApplyConn("Plugin_A");
// 	bool ver_ret = conn_B->ExecuteQuery("select version from platform.db_version;");
//
// 	for (; ver_ret && !conn_B->IsEOF(); conn_B->Next())
// 	{
// 		std::string vers = conn_A->GetString("version");
// 		cout << vers << endl;
// 	}
//
// 	db_layer::DBLayer_FreeConn(conn_B);
//
// 	param.min_conn = 5;
// 	db_layer::Module_Register(param, "Plugin_B");
//
// 	k_conn = db_layer::DBLayer_Valid("Plugin_B");
//
// 	param.max_conn = 20;
// 	db_layer::Isomerism_Register(param, "Plugin_C");
//
// 	k_conn = db_layer::DBLayer_Valid("Plugin_C");
//
// 	param.ip = "10.18.64.103";
//
// 	db_layer::DBLayer_ResetParam(param);
//
// 	auto func = [&](db_layer::CConnect& rs)
// 	{
// 		for (; !rs.IsEOF(); rs.Next())
// 		{
// 			int id = rs.GetInt("id");
// 			string info = rs.GetString("info");
// 			string crt_time = rs.GetString("crt_time");
//
// 			cout << id << ", " << info << ", " << crt_time << endl;
// 		}
// 	};
//
// 	db_layer::CConnect* ptr_connect = db_layer::DBLayer_ApplyConn("Plugin_A");
// 	ptr_connect->Begin();
// 	if (!ptr_connect->Execute("insert into t2 (info, crt_time) values (md5(random()::text), now());"))
// 	{
// 		string err_info("");
// 		int err_code;
// 		ptr_connect->GetLastError(err_code, err_info);
// 		assert(0);
// 		printf("Execute Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 		ptr_connect->Rollback();
// 	}
//
// 	db_layer::Isomerism_ResetParam(param, "Plugin_C");
//
// 	if (ptr_connect->ExecuteQuery("select * from t1 order by random() limit 5;"))
// 	{
// 		func(*ptr_connect);
// 	}
// 	else
// 	{
// 		string err_info("");
// 		int err_code;
// 		ptr_connect->GetLastError(err_code, err_info);
// 		assert(0);
// 		printf("ExecuteQuery Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 		ptr_connect->Rollback();
// 	}
//
// 	ptr_connect->Commit();
//
// 	db_layer::DBLayer_FreeConn(ptr_connect);
//
// 	ExecuteQ("select * from t1 order by random() limit 5;", func, "Plugin_A");
//
// 	ExecuteH("insert into t1 (info, crt_time) values (md5(random()::text), now());", "Plugin_A");
//
// 	ExecuteQ("select * from t1 order by random() limit 5;", func, "Plugin_B");
//
// 	ExecuteQ("select * from t1 order by random() limit 5;", func, "Plugin_C");
//
// 	// db_layer::DBLayer_Remove();
//
// 	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
//
// 	return;
// }
//
// void DBLayerExecuteThread_PostgreSQL()
// {
// 	auto func = [&](db_layer::CConnect& rs)
// 	{
// 		for (; !rs.IsEOF(); rs.Next())
// 		{
// 			int id = rs.GetInt("id");
// 			string info = rs.GetString("info");
// 			string crt_time = rs.GetString("crt_time");
//
// 			cout << id << ", " << info << ", " << crt_time << endl;
// 		}
// 	};
//
// 	int k = 10;
// 	while (k--)
// 	{
// 		switch (rand() % 3)
// 		{
// 		case 0:
// 			ExecuteQ("select * from t1 order by random() limit 5;", func, "Plugin_A");
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(500));
// 			ExecuteQ("select * from t2 order by random() limit 5;", func, "Plugin_B");
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(500));
// 			ExecuteH("insert into t3 (info, crt_time) values (md5(random()::text), now());", "Plugin_C");
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(500));
//
// 			break;
//
// 		case 1:
// 			ExecuteQ("select * from t3 order by random() limit 5;", func, "Plugin_A");
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
// 			ExecuteQ("select * from t1 order by random() limit 5;", func, "Plugin_B");
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
// 			ExecuteH("insert into t2 (info, crt_time) values (md5(random()::text), now());", "Plugin_C");
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
//
// 			break;
//
// 		case 2:
// 			ExecuteQ("select * from t2 order by random() limit 5;", func, "Plugin_A");
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(1500));
// 			ExecuteQ("select * from t3 order by random() limit 5;", func, "Plugin_B");
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(1500));
// 			ExecuteH("insert into t1 (info, crt_time) values (md5(random()::text), now());", "Plugin_C");
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(1500));
//
// 			break;
// 		}
// 	}
// }
//
// void DBLayerConnectThread_PostgreSQL()
// {
// 	DBParam param;
// 	param.db_type = DBType::PostgreSQL;
// 	param.ip = "127.0.0.1";
// 	param.port = 5432;
// 	param.user = "postgres";
// 	param.passwd = "Abc12345";
// 	param.db_name = "postgres";
// 	param.min_conn = 5;
// 	param.max_conn = 10;
//
// 	auto func = [&](db_layer::CConnect& rs)
// 	{
// 		for (; !rs.IsEOF(); rs.Next())
// 		{
// 			int id = rs.GetInt("id");
// 			string info = rs.GetString("info");
// 			string crt_time = rs.GetString("crt_time");
//
// 			cout << id << ", " << info << ", " << crt_time << endl;
// 		}
// 	};
//
// 	cout << "Start TEST:" << endl;
// 	IVMS_LOG_INFO("DBLayer", "[DBLayerConnectThread_PostgreSQL] Start TEST.");
//
// 	if (DBLayer_Init(param))
// 	{
// 		cout << "create dblayer : RW" << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] create dblayer : RW");
// 	}
//
// 	boost::this_thread::sleep(boost::posix_time::milliseconds(500));
//
// 	db_layer::Module_Register(param, "XXX");
//
// 	CConnect* ptr_conn_1_1 = DBLayer_ApplyConn("XXX");
// 	if (ptr_conn_1_1 == nullptr)
// 	{
// 		cout << "dblayer-RW: ptr_conn is NULL..." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : RW, ptr_conn is NULL...");
// 		return;
// 	}
//
// 	boost::this_thread::sleep(boost::posix_time::milliseconds(100));
//
// 	db_layer::Module_Register(param, "YYY");
//
// 	CConnect* ptr_conn_1_2 = DBLayer_ApplyConn("YYY");
// 	if (ptr_conn_1_2 == nullptr)
// 	{
// 		cout << "dblayer-RW: ptr_conn is NULL..." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : RW, ptr_conn is NULL...");
// 		return;
// 	}
//
// 	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
//
// 	DBLayer_FreeConn(ptr_conn_1_1);
//
// 	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
//
// 	DBLayer_FreeConn(ptr_conn_1_2);
//
// 	if (!DBLayer_Remove())
// 	{
// 		cout << "dblayer-RW: remove ptr_dblayer failed..." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : RW, remove ptr_dblayer failed...");
// 	}
// 	else
// 	{
// 		cout << "dblayer-RW: ptr_dblayer has been removed." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : RW, ptr_dblayer has been removed.");
// 	}
//
// 	return;
// }
//
// void DBLayerConnectMode_DB()
// {
// 	DBParam param;
// 	param.db_type = DBType::SQLServer;
// 	// param.db_type = DBType::MySQL;
// 	param.ip = "10.18.70.104";
// 	param.port = 1433;
// 	// param.port = 3306;
// 	param.user = "sa";
// 	// param.user = "root";
// 	param.passwd = "Abc12345";
// 	param.db_name = "vsm";
// 	param.min_conn = 5;
// 	param.max_conn = 10;
//
// 	int layer_id = 0;
// 	db_layer::DBLayer_Init(param, layer_id);
//
// 	db_layer::CConnect* x_conn = db_layer::DBLayer_ApplyConn(layer_id);
//
// 	if (!x_conn->Execute("INSERT INTO hik_info(hik_name_1, hik_time_1) VALUES ('xxx', CURRENT_TIMESTAMP);"))
// 	// if (!x_conn->Execute("INSERT INTO person(id, name, age, ctime) VALUES (4, 'xxx', 18, now());"))
// 	{
// 		int err_code;
// 		std::string err_info;
// 		x_conn->GetLastError(err_code, err_info);
// 		cout << "x_conn->Execute Failed." << endl;
// 		IVMS_LOG_ERROR("DBLayer", "[DBLayerConnectMode_DB] Error Info: %s.", err_info.c_str());
// 	}
//
// 	if (x_conn->ExecuteQuery("SELECT * FROM hik_info;"))
// 	// if (x_conn->ExecuteQuery("SELECT * FROM person;"))
// 	{
// 		for (; !x_conn->IsEOF(); x_conn->Next())
// 		{
// 			int id = x_conn->GetInt("id");
// 			string hik_name = x_conn->GetString("hik_name");
// 			string hik_time = x_conn->GetString("hik_time");
//
// 			/*int id = x_conn->GetInt("id");
// 			string name = x_conn->GetString("name");
// 			int age = x_conn->GetInt("age");
// 			string ctime = x_conn->GetString("ctime");*/
// 		}
// 	}
// 	else
// 	{
// 		int err_code;
// 		std::string err_info;
// 		x_conn->GetLastError(err_code, err_info);
// 		cout << "x_conn->Execute Failed." << endl;
// 		IVMS_LOG_ERROR("DBLayer", "[DBLayerConnectMode_DB] Error Info: %s.", err_info.c_str());
// 	}
//
// 	db_layer::DBLayer_Remove(layer_id);
//
// 	Sleep(5000);
//
// 	db_layer::DBLayer_Init(param, layer_id);
//
// 	db_layer::CConnect* y_conn = db_layer::DBLayer_ApplyConn(layer_id);
//
// 	// if (y_conn->ExecuteQuery("SELECT * FROM hik_info;"))
// 	if (y_conn->ExecuteQuery("SELECT * FROM person;"))
// 	{
// 		for (; !y_conn->IsEOF(); y_conn->Next())
// 		{
// 			/*int id = y_conn->GetInt("id");
// 			string hik_name = y_conn->GetString("hik_name");
// 			string hik_time = y_conn->GetString("hik_time");*/
//
// 			int id = y_conn->GetInt("id");
// 			string name = y_conn->GetString("name");
// 			int age = y_conn->GetInt("age");
// 			string ctime = y_conn->GetString("ctime");
// 		}
// 	}
// 	else
// 	{
// 		int err_code;
// 		std::string err_info;
// 		y_conn->GetLastError(err_code, err_info);
// 		cout << "x_conn->Execute Failed." << endl;
// 		IVMS_LOG_ERROR("DBLayer", "[DBLayerConnectMode_DB] Error Info: %s.", err_info.c_str());
// 	}
//
// 	db_layer::DBLayer_Remove(layer_id);
//
// 	return;
// }
//
// void DBLayerConnectMode_SQLServerDB()
// {
// 	DBParam param;
// 	param.db_type = DBType::SQLServer;
// 	// param.db_type = DBType::MySQL;
// 	param.ip = "127.0.0.1";
// 	param.port = 1433;
// 	// param.port = 3306;
// 	param.user = "sa";
// 	// param.user = "root";
// 	param.passwd = "Abc12345";
// 	param.db_name = "student";
// 	param.min_conn = 5;
// 	param.max_conn = 10;
//
// 	int layer_id = 0;
// 	if (!db_layer::DBLayer_Init(param, layer_id))
// 	{
// 		std::cout << "init failed!" << std::endl;
// 		return ;
// 	}
//
// 	db_layer::CConnect* x_conn = db_layer::DBLayer_ApplyConn(layer_id);
//
//
// 	if (x_conn->ExecuteQuery("select * from dbo.Student;"))
// 	{
// 		for (; !x_conn->IsEOF(); x_conn->Next())
// 		{
// 			int id = x_conn->GetInt("Sno");
// 			string name = x_conn->GetString("Sname");
// 			string sex = x_conn->GetString("Ssex");
// 			auto age = x_conn->GetInt("Sage");
// 			auto time = x_conn->GetDateTime("btime");
// 			tm * tm_ = nullptr;
// 			tm_ = localtime(&time);
// 			std::stringstream ss;
// 			ss << tm_->tm_year + 1900 << "-" << tm_->tm_mon + 1 << "-" << tm_->tm_mday << " "
// 				<< tm_->tm_hour << ":" << tm_->tm_min << ":" << tm_->tm_sec
// 				<< std::endl;
// 			std::cout << ss.str();
// 			/*int id = x_conn->GetInt("id");
// 			string name = x_conn->GetString("name");
// 			int age = x_conn->GetInt("age");
// 			string ctime = x_conn->GetString("ctime");*/
// 		}
// 	}
// 	else
// 	{
// 		int err_code;
// 		std::string err_info;
// 		x_conn->GetLastError(err_code, err_info);
// 		cout << "x_conn->Execute Failed." << endl;
// 		IVMS_LOG_ERROR("DBLayer", "[DBLayerConnectMode_DB] Error Info: %s.", err_info.c_str());
// 	}
//
// 	db_layer::DBLayer_Remove(layer_id);
//
// 	return;
// }
//
// bool DirectConnectMode_Oracle()
// {
// 	DBParam param;
// 	param.db_type = DBType::SQLServer;
// 	param.ip = "10.19.181.104";
// 	param.port = 1433;
// 	param.user = "sa";
// 	param.passwd = "Abc12345";
// 	param.db_name = "vsm";
// 	param.min_conn = 5;
// 	param.max_conn = 10;
//
// 	bool ret = Isomerism_Register(param, "xxx");
//
// 	Sleep(3000);
//
// 	CConnect* ptr_conn = DBLayer_ApplyConn("xxx");
//
// 	// ret = ptr_conn->Execute("INSERT INTO \"ADMIN\".\"db\" ( \"a\" ,\"b\" ,\"c\" ,\"d\" ,\"g\" ,\"h\" ,\"j\" ,\"l\" ,\"m\" ,\"n\" ,\"o\" ,\"p\" ,\"k\" ,\"e\" ,\"f\" ,\"i\" ,\"q\" ,\"r\" ,\"s\"  ) \
// 	VALUES ( '3926176880' ,'2021-12-14T12:17:25' ,'2021-12-14' ,'12:17:25' ,'10.18.64.201' ,'E33392121' ,'本亚 杨' ,'0592377872' ,'' ,'' ,'' ,'' ,'All Persons' ,'' ,'ACSEventInvalidCardNumber' ,'Cardreader 01' ,'' ,'本亚' ,'杨'  )");
//
// 	return ret;
// }
//
// bool DirectConnectMode_SQLServer()
// {
// 	std::string server_ip = "10.19.181.104";
// 	int server_port = 1433;
// 	std::string user_name = "sa";
// 	std::string user_password = "Abc12345";
// 	std::string db_name = "cms";
//
// 	/*cout << "SQL Server IP: ";
// 	cin >> server_ip;
// 	cout << "SQL Server PORT: ";
// 	cin >> server_port;
// 	cout << "USER NAME: ";
// 	cin >> user_name;
// 	cout << "PASSWORD: ";
// 	cin >> user_password;
// 	cout << "DB NAME: ";
// 	cin >> db_name;
//
// 	cout << endl;
// 	cout << "Check Param Failed, IP " << server_ip << ", Port " << server_port << ", User " << user_name << ", Pwd " << user_password << ", DBName " << db_name << "." << endl;*/
//
// 	if (server_ip.empty() || user_name.empty() || user_password.empty() || db_name.empty() || server_port <= 0)
// 	{
// 		IVMS_LOG_ERROR("DBLayer", "[DirectConnectMode_SQLServer] Check Param Failed, IP %s, Port %d, user %s, Pwd %s, dbname %s.", server_ip.c_str(), server_port, user_name.c_str(), user_password.c_str(), db_name.c_str());
// 		return false;
// 	}
//
// // 	_ConnectionPtr m_pConnection;
// //
// // 	CoInitialize(NULL); // 初始化
// // 	HRESULT hr = m_pConnection.CreateInstance(__uuidof(Connection));
// // 	if (FAILED(hr))
// // 	{
// // 		cout << "_ConnectionPtr.CreateInstance Failed." << endl;
// // 		IVMS_LOG_ERROR("DBLayer", "[DirectConnectMode_SQLServer] _ConnectionPtr.CreateInstance Failed.");
// // 		return false;
// // 	}
// //
// // 	char conn_info[512] = { 0 };
// // 	sprintf_s(conn_info, "Provider=SQLOLEDB; Persist Security Info=false; Data Source=%s,%d; DataBase=%s; UID=%s; PWD=%s",
// // 		server_ip.c_str(), server_port, db_name.c_str(), user_name.c_str(), user_password.c_str());
// //
// // 	cout << "Connect String: " << conn_info << endl;
// // 	IVMS_LOG_INFO("DBLayer", "[DirectConnectMode_SQLServer] Connect String: %s.", conn_info);
// //
// // 	_bstr_t bstrConnectInfo_ = conn_info;
// //
// // 	try
// // 	{
// // 		HRESULT conn_ret = m_pConnection->Open(bstrConnectInfo_, "", "", adModeUnknown);
// // 	}
// // 	catch (_com_error &e)
// // 	{
// // 		/*std::string str_desp = SQLServer_TCHAR2STRING((LPCTSTR)e.Description());
// // 		string err_info = SQLServer_TCHAR2STRING(e.ErrorMessage());
// // 		IVMS_LOG_ERROR("DBLayer", "[CDBSQLServer::Open] Open connection Failed, description: %s, err_info: %s", str_desp.c_str(), err_info.c_str());
// // 		cout << "Open connection Failed, description: " << str_desp << ", err_info: " << err_info << endl;*/
// //
// // 		return false;
// // 	}
//
// 	cout << "Open connection succeed." << endl;
//
// 	return true;
// }
//
// void DBLayerConnectHistory_PostgreSQL()
// {
// 	DBParam param;
// 	param.db_type = DBType::PostgreSQL;
// 	param.ip = "10.19.181.104";
// 	param.user = "postgres";
// 	param.passwd = "ZjhhYTE0ZGEyMzAxZTIwMWU4MTdmNWI4NjY3YTM2YmI0MGM4Y2E0OWRhNjliMzQ3MGE3NGQwZjRlYzE5NDk2MQ==";
// 	param.db_name = "vsm";
// 	param.port = 5432;
// 	param.min_conn = 1;
// 	param.max_conn = 3;
//
// 	int layer_id = 0;
// 	db_layer::DBLayer_Init(param, layer_id);
//
// 	db_layer::CConnect* x_conn = db_layer::DBLayer_ApplyConn(layer_id);
// 	if (x_conn != nullptr)
// 	{
// 		int ret_num = 0;
// 		x_conn->EstimateResult("select * from dbversion;", ret_num);
// 		std::cout << ret_num << endl;
// 	}
//
// 	int layer_id_1 = 0;
// 	db_layer::DBLayer_Init(param, layer_id_1);
//
// 	int layer_id_2 = 0;
// 	db_layer::DBLayer_Init(param, layer_id_2);
//
// 	db_layer::CConnect* conn_1 = db_layer::DBLayer_ApplyConn(layer_id_1);
// 	if (conn_1 != nullptr)
// 	{
// 		conn_1->ExecuteQuery("SELECT 1;");
// 		int ret_num = conn_1->GetInt(0);
// 		std::cout << ret_num << endl;
// 	}
//
// 	db_layer::CConnect* conn_2 = db_layer::DBLayer_ApplyConn(layer_id_2);
// 	if (conn_2 != nullptr)
// 	{
// 		conn_2->ExecuteQuery("SELECT 2;");
// 		int ret_num = conn_2->GetInt(0);
// 		std::cout << ret_num << endl;
// 	}
//
// 	x_conn->Begin();
// 	bool x = x_conn->ExecuteQuery("SELECT 1;");
// 	// x_conn->Commit();
//
// 	db_layer::CConnect* y_conn = db_layer::DBLayer_ApplyConn(layer_id);
// 	y_conn->Begin();
// 	bool y = y_conn->Execute("INSERT INTO dbversion(version, update_time, up_status) VALUES ('2.0.0.1', now(), 1);");
// 	// y_conn->Commit();
//
// 	db_layer::CConnect* z_conn = db_layer::DBLayer_ApplyConn(layer_id);
// 	z_conn->Begin();
// 	bool z = z_conn->ExecuteQuery("SELECT * FROM dbversion;");
// 	// z_conn->Commit();
//
// 	db_layer::CConnect* r_conn_1 = db_layer::DBLayer_ApplyConn(layer_id);
//
// 	x_conn->Commit();
// 	y_conn->Commit();
// 	z_conn->Commit();
//
// 	db_layer::DBLayer_FreeConn(x_conn, layer_id);
// 	db_layer::DBLayer_FreeConn(y_conn, layer_id);
// 	db_layer::DBLayer_FreeConn(z_conn, layer_id);
//
// 	db_layer::CConnect* r_conn_2 = db_layer::DBLayer_ApplyConn(layer_id);
//
// 	db_layer::CDBLayer* ptr_dblayer = new db_layer::CDBLayer(param);
// 	if (ptr_dblayer != nullptr)
// 	{
// 		Sleep(500);
// 		db_layer::CConnect* ptr_conn = ptr_dblayer->ApplyConn();
//
// 		if (ptr_conn != nullptr)
// 		{
// 			string sql = "SELECT version FROM dbversion WHERE up_status = 1 ORDER BY update_time DESC LIMIT 1;";
// 			IVMS_LOG_INFO("DBUPGRADE", "[CDBPermission::Query] %s.", sql.c_str());
// 			if (ptr_conn->ExecuteQuery(sql))
// 			{
// 				for (; !ptr_conn->IsEOF(); ptr_conn->Next())
// 				{
// 					string version = ptr_conn->GetString("version");
// 				}
// 			}
//
// 			ptr_dblayer->FreeConn(ptr_conn);
// 		}
//
// 		delete ptr_dblayer;
// 		ptr_dblayer = nullptr;
// 	}
//
// 	auto func = [&](db_layer::CConnect& rs)
// 	{
// 		for (; !rs.IsEOF(); rs.Next())
// 		{
// 			int id = rs.GetInt("id");
// 			string info = rs.GetString("info");
// 			string crt_time = rs.GetString("crt_time");
//
// 			cout << id << ", " << info << ", " << crt_time << endl;
// 		}
// 	};
//
// 	boost::this_thread::sleep(boost::posix_time::milliseconds(500));
// 	cout << "Start TEST:" << endl;
// 	IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] Start TEST.");
//
// 	int dblayer_no_1 = 0;
// 	std::unique_ptr<db_layer::CDBLayer> ptr_dblayer_1;
// 	if (DBLayer_Register(ptr_dblayer_1, param))
// 	{
// 		dblayer_no_1 = ptr_dblayer_1->GetInstanceID();
// 		cout << "create dblayer : " << dblayer_no_1 << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] create dblayer : %d.", dblayer_no_1);
// 	}
//
// 	int dblayer_no_2 = 0;
// 	std::unique_ptr<db_layer::CDBLayer> ptr_dblayer_2;
// 	param.min_conn = 5;
// 	param.max_conn = 10;
// 	if (DBLayer_Register(ptr_dblayer_2, param))
// 	{
// 		dblayer_no_2 = ptr_dblayer_2->GetInstanceID();
// 		cout << "create dblayer : " << dblayer_no_2 << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] create dblayer : %d.", dblayer_no_2);
// 	}
//
// 	int dblayer_no_3 = 0;
// 	if (DBLayer_Init(param, dblayer_no_3))
// 	{
// 		cout << "create dblayer : " << dblayer_no_3 << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] create dblayer : %d.", dblayer_no_3);
// 	}
// 	CConnect* ptr_conn_3_1 = DBLayer_ApplyConn(dblayer_no_3);
// 	if (ptr_conn_3_1 == nullptr)
// 	{
// 		cout << "dblayer-" << dblayer_no_3 << ": ptr_conn is NULL..." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, ptr_conn is NULL...", dblayer_no_3);
// 		return;
// 	}
//
// 	if (!ptr_conn_3_1->Execute("insert into t1 (info, crt_time) values (md5(random()::text), now());"))
// 	{
// 		string err_info("");
// 		int err_code;
// 		ptr_conn_3_1->GetLastErrorCode(err_code, err_info);
// 		assert(0);
// 		printf("Execute Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 		return;
// 	}
//
// 	if (ptr_conn_3_1->ExecuteQuery("select * from t1 order by random() limit 5;"))
// 	{
// 		func(*ptr_conn_3_1);
// 	}
// 	else
// 	{
// 		string err_info("");
// 		int err_code;
// 		ptr_conn_3_1->GetLastError(err_code, err_info);
// 		assert(0);
// 		printf("Execute Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 		return;
// 	}
//
// 	CRecordSet* agent_rs;
// 	if (ptr_conn_3_1->ExecuteQuery("select * from t1 order by random() limit 5;", agent_rs))
// 	{
// 		for (; !agent_rs->IsEOF(); agent_rs->Next())
// 		{
// 			int id = agent_rs->GetInt("id");
// 			string info = agent_rs->GetString("info");
// 			string crt_time = agent_rs->GetString("crt_time");
//
// 			cout << id << ", " << info << ", " << crt_time << endl;
// 		}
// 	}
//
// 	delete agent_rs;
// 	agent_rs = nullptr;
//
// 	int ret_set_size = 0;
// 	if (!ptr_conn_3_1->EstimateResult("select * from t1 where id % 2 = 0;", ret_set_size))
// 	{
// 		string err_info("");
// 		int err_code;
// 		ptr_conn_3_1->GetLastError(err_code, err_info);
// 		assert(0);
// 		printf("Execute Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 		return;
// 	}
//
// 	CConnect* ptr_conn_1_1 = ptr_dblayer_1->ApplyConn();
// 	if (ptr_conn_1_1 == nullptr)
// 	{
// 		cout << "dblayer-" << dblayer_no_1 << ": ptr_conn is NULL..." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, ptr_conn is NULL...", dblayer_no_1);
// 		return;
// 	}
// 	else
// 	{
// 		CConnect* ptr_conn_1_2 = ptr_dblayer_1->ApplyConn();
//
// 		if (ptr_conn_1_2 == nullptr)
// 		{
// 			cout << "dblayer-" << dblayer_no_1 << ": ptr_conn is NULL..." << endl;
// 			IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, ptr_conn is NULL...", dblayer_no_1);
// 		}
// 		else
// 		{
// 			if (ptr_conn_1_2->ExecuteQuery("select * from t1 order by random() limit 5;"))
// 			{
// 				func(*ptr_conn_1_2);
// 			}
// 			else
// 			{
// 				string err_info("");
// 				int err_code;
// 				ptr_conn_1_2->GetLastError(err_code, err_info);
// 				assert(0);
// 				printf("Execute Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 				return;
// 			}
//
// 			ptr_dblayer_1->FreeConn(ptr_conn_1_2);
// 		}
//
// 		cout << "dblayer-" << dblayer_no_1 << ": apply ptr_conn succeed." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, apply ptr_conn succeed, apply ptr_conn succeed.", dblayer_no_1);
// 	}
//
// 	ptr_dblayer_1->FreeConn(ptr_conn_1_1);
//
// 	CConnect* ptr_conn_2_1 = ptr_dblayer_2->ApplyConn();
// 	if (ptr_conn_2_1 == nullptr)
// 	{
// 		cout << "dblayer-" << dblayer_no_2 << ": ptr_conn is NULL..." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, ptr_conn is NULL...", dblayer_no_2);
// 		return;
// 	}
//
// 	if (!ptr_conn_2_1->Execute("insert into t1 (info, crt_time) values (md5(random()::text), now());"))
// 	{
// 		string err_info("");
// 		int err_code;
// 		ptr_conn_2_1->GetLastErrorCode(err_code, err_info);
// 		assert(0);
// 		printf("Execute Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 		return;
// 	}
//
// 	boost::this_thread::sleep(boost::posix_time::milliseconds(3000));
//
// 	ptr_dblayer_2->FreeConn(ptr_conn_2_1);
//
// 	DBLayer_FreeConn(ptr_conn_3_1, dblayer_no_3);
//
// 	if (!DBLayer_Remove(ptr_dblayer_2))
// 	{
// 		cout << "dblayer-" << dblayer_no_2 << ": remove ptr_dblayer failed..." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, remove ptr_dblayer failed...", dblayer_no_2);
// 	}
// 	else
// 	{
// 		cout << "dblayer-" << dblayer_no_2 << ": ptr_dblayer has been removed." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, ptr_dblayer has been removed.", dblayer_no_2);
// 	}
//
// 	if (!DBLayer_Remove(dblayer_no_3))
// 	{
// 		cout << "dblayer-" << dblayer_no_3 << ": remove ptr_dblayer failed..." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, remove ptr_dblayer failed...", dblayer_no_3);
// 	}
// 	else
// 	{
// 		cout << "dblayer-" << dblayer_no_3 << ": ptr_dblayer has been removed." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, ptr_dblayer has been removed.", dblayer_no_3);
// 	}
//
// 	if (!DBLayer_Remove(dblayer_no_1))
// 	{
// 		cout << "dblayer-" << dblayer_no_1 << ": remove ptr_dblayer failed..." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, remove ptr_dblayer failed...", dblayer_no_1);
// 	}
// 	else
// 	{
// 		cout << "dblayer-" << dblayer_no_1 << ": ptr_dblayer has been removed." << endl;
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, ptr_dblayer has been removed.", dblayer_no_1);
// 	}
//
// 	return;
// }
//
// void DBLayerConnectMode_MySQL(int id_mark)
// {
// 	int num = 0;
// 	auto func = [&](db_layer::CConnect& rs)
// 	{
// 		for (; !rs.IsEOF(); rs.Next())
// 		{
// 			num = rs.GetInt("num");
// 			// cout << num << endl;
// 		}
// 	};
//
// 	DWORD time_start, time_end;
//
// 	time_start = GetTickCount();
// 	cout << "Start ID: " << id_mark << endl;
//
// 	db_layer::CConnect* ptr_connect = db_layer::DBLayer_ApplyConn("Plugin_C");
//
// 	if (ptr_connect->ExecuteQuery("SELECT \
// 		COUNT(a.resource_id) as num \
// 		FROM \
// 		hpcvmsres_logic_element a, \
// 		hpcvmsauth_role_to_resource_1 b \
// 		WHERE \
// 		a.system_id = 'sys_a_1' AND \
// 		a.resource_type = 1001 AND \
// 		a.delete_flag = 0 AND \
// 		b.role_id IN('sys_a_1_r_1', 'sys_a_1_r_10001') AND \
// 		b.resource_type IN(1001) AND \
// 		a.resource_id = b.resource_id; "))
// 	{
// 		func(*ptr_connect);
// 	}
// 	else
// 	{
// 		string err_info("");
// 		int err_code;
// 		ptr_connect->GetLastError(err_code, err_info);
// 		assert(0);
// 		printf("ExecuteQuery Failed, error code: %d, error info: %s", err_code, err_info.c_str());
// 		ptr_connect->Rollback();
// 	}
//
// 	time_end = GetTickCount();
// 	// cout << "End = " << time_end << endl;
// 	cout << "ID: " << id_mark << ", Start = " << time_start << ", End = " << time_end << ", Cost = " << time_end - time_start << ", Ret: " << num << endl;
//
// 	return;
// }
//
// void DBLayerConnectMode_SQLite()
// {
// 	DBParam param;
// 	param.db_type = DBType::SQLite;
// 	param.ip = "127.0.0.1";
// 	param.user = "user_wim";
// 	param.passwd = "Q2Ob9r3bzfcqk0rdqhNffZu5Izrlx4GWMnz3zDpE4PM=";
// 	param.db_name = "AccessController";
// 	param.port = 1000;
// 	param.min_conn = 1;
// 	param.max_conn = 1;
// 	int layer_id = 10;
// 	db_layer::DBLayer_Init(param, layer_id);
// 	db_layer::CConnect* x_conn = db_layer::DBLayer_ApplyConn(layer_id);
// 	if (x_conn != nullptr)
// 	{
// 		std::string sql = "SELECT * FROM ACCScheduleHoliday";
// 		auto res = x_conn->ExecuteQuery(sql);
// 		if (res)
// 		{
// 			while (!x_conn->IsEOF())
// 			{
// 				std::string res1 = x_conn->GetString(0);
// 				std::string res2 = x_conn->GetString(1);
// 				std::string res3 = x_conn->GetString(2);
// 				std::cout << res1 << " " << res2 << " " << res3 << endl;
// 				x_conn->Next();
// 			}
// 		}
// 		else
// 		{
// 			std::cout << "ExecuteQuery Failed!" << std::endl;
// 		}
// 	}
// 	else
// 	{
// 		std::cout << "x_conn is null" << std::endl;
// 	}
//
// }
//
// void DBLayerConnectMode_MySQLDB()
// {
//
// 	DBParam param;
// 	param.db_type = DBType::PostgreSQL;
// 	param.ip = "127.0.0.1";
// 	param.port = 5432;
// 	param.user = "postgres";
// 	param.passwd = "ZjhhYTE0ZGEyMzAxZTIwMWU4MTdmNWI4NjY3YTM2YmI0MGM4Y2E0OWRhNjliMzQ3MGE3NGQwZjRlYzE5NDk2MQ==";
// 	param.db_name = "cms";
// 	param.min_conn = 5;
// 	param.max_conn = 10;
//
// 	int layer_id = 0;
// 	if (!db_layer::DBLayer_Init(param, layer_id))
// 	{
// 		std::cout << "init db failed!" << std::endl;
// 		return;
// 	}
// 	std::string sql = "UPDATE public.dblayer_config_conn SET low = 60 WHERE id = 1;";
//
// 	string layer_id_str = to_string(layer_id);
//
// 	db_layer::CConnect* x_conn = db_layer::DBLayer_ApplyConn(layer_id);
//
// 	//x_conn->Begin();
//
// 	if (x_conn->ExecuteQuery(sql))
// 	{
// 		IVMS_LOG_INFO("DBLayer", "SUCC");
// // 		for (; !x_conn->IsEOF(); x_conn->Next())
// // 		{
// // 			auto x = x_conn->GetInt("num_integer");
// // 			std::cout << "num_integer" ": " << x << std::endl;
// // 		}
// 	}
// 	else
// 	{
// 		int err_code;
// 		std::string err_info;
// 		x_conn->GetLastError(err_code, err_info);
// 		std::cout << err_info << endl;
// // 		IVMS_LOG_ERROR("DBLayer", "[DBLayerConnectMode_DB] Error Info: %s.", err_info.c_str());
// // 		x_conn->Rollback();
// 	}
//
// 	if (!db_layer::DBLayer_FreeConn(x_conn))
// 	{
// 		int err_code;
// 		std::string err_info;
// 		x_conn->GetLastError(err_code, err_info);
// 		std::cout << err_info << endl;
// 		IVMS_LOG_ERROR("DBLayer", "[DBLayerConnectMode_DB] Error Info: %s.", err_info.c_str());
// 	}
// 	else
// 	{
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_DB] SUCC FREE.");
// 	}
//
// 	if (x_conn == nullptr)
// 	{
// 		IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_DB] SUCC FREE.");
// 	}
// 	else
// 	{
// 		IVMS_LOG_ERROR("DBLayer", "[DBLayerConnectMode_DB] FAILED FREE.");
// 	}
//
//
// 	string query_sql = "SELECT COUNT(*) FROM baseacs.slot_card_records;";
// 	if (x_conn->ExecuteQueryUDT(sql,5))
// 	{
// 		IVMS_LOG_INFO("DBLayer", "SUCC");
// 	}
// 	else
// 	{
// 		int err_code;
// 		std::string err_info;
// 		x_conn->GetLastError(err_code, err_info);
// 		std::cout << err_info << endl;
// 	}
//
// 	x_conn->Rollback();
//
// 	//x_conn->Commit();
//
// 	db_layer::DBLayer_Remove(layer_id);
//
// 	return;
//
//
// }
//
// void ChangeTablePartitionFunc()
// {
// 	DBParam param_x;
// 	param_x.db_type = DBType::PostgreSQL;
// 	param_x.ip = "192.168.56.103";
// 	param_x.port = 5432;
// 	param_x.user = "postgres";
// 	param_x.passwd = "Abc12345++";
// 	param_x.db_name = "cms";
// 	param_x.min_conn = 1;
// 	param_x.max_conn = 3;
//
// 	// 初始化数据库接入层
// 	db_layer::DBLayer_Init(param_x);
//
// 	db_layer::Module_Register(param_x, "Plugin_A");
// 	db_layer::Module_Register(param_x, "Plugin_B");
//
// 	bool k_conn = db_layer::DBLayer_Valid("Plugin_A") && db_layer::DBLayer_Valid("Plugin_B");
//
// 	if (k_conn)
// 	{
// 		db_layer::CConnect* conn_A = db_layer::DBLayer_ApplyConn("Plugin_A");
// 		db_layer::CConnect* conn_B = db_layer::DBLayer_ApplyConn("Plugin_B");
//
// 		bool count_ret = conn_A->ExecuteQuery("select * from public.trigger_func_content;");
//
// 		for (; count_ret && !conn_A->IsEOF(); conn_A->Next())
// 		{
// 			string str_1 = conn_A->GetString("nspname");
// 			string str_2 = conn_A->GetString("proname");
// 			string str_3 = conn_A->GetString("func_content");
// 			string str_4 = conn_A->GetString("tabname");
//
// 			str_3.erase(remove(str_3.begin(), str_3.end(), '\r'), str_3.end());
// 			str_3 = "SET client_encoding TO 'UTF8';\n\n" + str_3;
//
// 			string sql_path = "E:\\Projects\\HCP 2.6\\TablePartitionChange\\" + str_1;
// 			mkdir(sql_path.c_str());
//
// 			sql_path += "\\" + str_4;
// 			mkdir(sql_path.c_str());
//
// 			// bool mark = CreateDirectory((LPCTSTR)sql_path.c_str(), NULL);
// 			// string sql_path = "E:\\Projects\\HCP 2.6\\TablePartitonChange\\Raw\\" + str_1 + "." + str_2 + ".sql";
//
// 			/*regex mon_reg(".*'YYYY_MM_DD'\\s*\\)\\s*FROM\\s+1\\s+FOR\\s+7\\s*\\)\\s*INTO.*");
// 			regex week_reg(".*EXTRACT\\s*\\(\\s*WEEK\\s+FROM\\s+NEW.*");
// 			regex day_reg(".*'YYYY_MM_DD'\\s*\\)\\s*INTO.*");
// 			smatch result;*/
//
// 			if (str_3.find("'YYYY_MM_DD') FROM 1 FOR 4) INTO") != string::npos)
// 			{
// 				string path_year = sql_path + "\\Year_Def";
// 				mkdir(path_year.c_str());
// 				string path_mon = sql_path + "\\Month";
// 				mkdir(path_mon.c_str());
// 				string path_week = sql_path + "\\Week";
// 				mkdir(path_week.c_str());
// 				string path_day = sql_path + "\\Day";
// 				mkdir(path_day.c_str());
//
// 				path_year += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_1(path_year);
// 				SQLFile_1 << str_3;
// 				SQLFile_1.close();
//
// 				path_mon += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_2(path_mon);
// 				SQLFile_2 << str_3;
// 				SQLFile_2.close();
//
// 				unsigned int cur_line = 0;
// 				string line_str, str_buffer;
//
// 				ifstream file_in_mon;
// 				line_str = "";
// 				str_buffer = "";
// 				file_in_mon.open(path_mon);
// 				cur_line = 0;
// 				while (getline(file_in_mon, line_str, '\n'))
// 				{
// 					if (line_str.find("'YYYY_MM_DD') FROM 1 FOR 4) INTO") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "SELECT SUBSTRING(TO_CHAR(NEW.XXXXXXXXXXXXXXX, 'YYYY_MM_DD') FROM 1 FOR 7) INTO postfix_content;\n";
// 					}
// 					else if (line_str.find("check_begin :=", 0) != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_begin := '''' || DATE_PART('year'::text, NEW.XXXXXXXXXXXXXXX) || '-' || DATE_PART('month'::text, NEW.XXXXXXXXXXXXXXX) || '-01''::TIMESTAMP';\n";
// 					}
// 					else if (line_str.find("check_end :=", 0) != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_end := '''' || DATE_PART('year'::TEXT, (NEW.XXXXXXXXXXXXXXX + '1 month'::INTERVAL)::DATE) || '-' || DATE_PART('month'::TEXT, (NEW.XXXXXXXXXXXXXXX + '1 month'::INTERVAL)::DATE) || '-01''::TIMESTAMP';\n";
// 					}
// 					else
// 					{
// 						str_buffer += line_str + "\n";
// 					}
//
// 					cur_line++;
// 				}
// 				file_in_mon.close();
// 				ofstream os_mon(path_mon);
// 				os_mon << str_buffer;
// 				os_mon.close();
//
// 				path_week += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_3(path_week);
// 				SQLFile_3 << str_3;
// 				SQLFile_3.close();
//
// 				ifstream file_in_week;
// 				line_str = "";
// 				str_buffer = "";
// 				file_in_week.open(path_week);
// 				cur_line = 0;
// 				while (getline(file_in_week, line_str, '\n'))
// 				{
// 					if (line_str.find("'YYYY_MM_DD') FROM 1 FOR 4) INTO") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "postfix_content := EXTRACT(ISOYEAR FROM NEW.XXXXXXXXXXXXXXX) || '_' || EXTRACT(WEEK FROM NEW.XXXXXXXXXXXXXXX);\n";
// 					}
// 					else if (line_str.find("check_begin :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_begin := '''' || (NEW.XXXXXXXXXXXXXXX - ((EXTRACT(DOW FROM NEW.XXXXXXXXXXXXXXX)::INT + 6) % 7 || ' day')::INTERVAL)::DATE::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else if (line_str.find("check_end :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_end := '''' || (NEW.XXXXXXXXXXXXXXX::DATE + '7 day'::INTERVAL)::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else
// 					{
// 						str_buffer += line_str + "\n";
// 					}
//
// 					cur_line++;
// 				}
// 				file_in_week.close();
// 				ofstream os_week(path_week);
// 				os_week << str_buffer;
// 				os_week.close();
//
// 				path_day += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_4(path_day);
// 				SQLFile_4 << str_3;
// 				SQLFile_4.close();
//
// 				ifstream file_in_day;
// 				line_str = "";
// 				str_buffer = "";
// 				file_in_day.open(path_day);
// 				cur_line = 0;
// 				while (getline(file_in_day, line_str, '\n'))
// 				{
// 					if (line_str.find("'YYYY_MM_DD') FROM 1 FOR 4) INTO") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "SELECT TO_CHAR(NEW.XXXXXXXXXXXXXXX, 'YYYY_MM_DD') INTO postfix_content;\n";
// 					}
// 					else if (line_str.find("check_begin :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_begin := '''' || NEW.XXXXXXXXXXXXXXX::DATE::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else if (line_str.find("check_end :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_end := '''' || (NEW.XXXXXXXXXXXXXXX::DATE + '1 day'::INTERVAL)::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else
// 					{
// 						str_buffer += line_str + "\n";
// 					}
//
// 					cur_line++;
// 				}
// 				file_in_day.close();
// 				ofstream os_day(path_day);
// 				os_day << str_buffer;
// 				os_day.close();
//
// 				conn_B->Execute("update public.trigger_func_content set part_type = 'Year' where tabname = '" + str_4 + "'; ");
// 			}
// 			// if (regex_match(str_3, result, mon_reg))
// 			else if (str_3.find("'YYYY_MM_DD') FROM 1 FOR 7) INTO") != string::npos)
// 			{
// 				string path_mon = sql_path + "\\Month_Def";
// 				mkdir(path_mon.c_str());
// 				string path_week = sql_path + "\\Week";
// 				mkdir(path_week.c_str());
// 				string path_day = sql_path + "\\Day";
// 				mkdir(path_day.c_str());
//
// 				path_mon += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_2(path_mon);
// 				SQLFile_2 << str_3;
// 				SQLFile_2.close();
//
// 				path_week += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_3(path_week);
// 				SQLFile_3 << str_3;
// 				SQLFile_3.close();
//
// 				unsigned int cur_line = 0;
// 				string line_str, str_buffer;
//
// 				ifstream file_in_week;
// 				line_str = "";
// 				str_buffer = "";
// 				file_in_week.open(path_week);
// 				cur_line = 0;
// 				while (getline(file_in_week, line_str, '\n'))
// 				{
// 					if (line_str.find("'YYYY_MM_DD') FROM 1 FOR 7) INTO") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "postfix_content := EXTRACT(ISOYEAR FROM NEW.XXXXXXXXXXXXXXX) || '_' || EXTRACT(WEEK FROM NEW.XXXXXXXXXXXXXXX);\n";
// 					}
// 					else if (line_str.find("check_begin :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_begin := '''' || (NEW.XXXXXXXXXXXXXXX - ((EXTRACT(DOW FROM NEW.XXXXXXXXXXXXXXX)::INT + 6) % 7 || ' day')::INTERVAL)::DATE::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else if (line_str.find("check_end :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_end := '''' || (NEW.XXXXXXXXXXXXXXX::DATE + '7 day'::INTERVAL)::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else
// 					{
// 						str_buffer += line_str + "\n";
// 					}
//
// 					cur_line++;
// 				}
// 				file_in_week.close();
// 				ofstream os_week(path_week);
// 				os_week << str_buffer;
// 				os_week.close();
//
// 				path_day += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_4(path_day);
// 				SQLFile_4 << str_3;
// 				SQLFile_4.close();
//
// 				ifstream file_in_day;
// 				line_str = "";
// 				str_buffer = "";
// 				file_in_day.open(path_day);
// 				cur_line = 0;
// 				while (getline(file_in_day, line_str, '\n'))
// 				{
// 					if (line_str.find("'YYYY_MM_DD') FROM 1 FOR 7) INTO") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "SELECT TO_CHAR(NEW.XXXXXXXXXXXXXXX, 'YYYY_MM_DD') INTO postfix_content;\n";
// 					}
// 					else if (line_str.find("check_begin :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_begin := '''' || NEW.XXXXXXXXXXXXXXX::DATE::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else if (line_str.find("check_end :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_end := '''' || (NEW.XXXXXXXXXXXXXXX::DATE + '1 day'::INTERVAL)::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else
// 					{
// 						str_buffer += line_str + "\n";
// 					}
//
// 					cur_line++;
// 				}
// 				file_in_day.close();
// 				ofstream os_day(path_day);
// 				os_day << str_buffer;
// 				os_day.close();
//
// 				conn_B->Execute("update public.trigger_func_content set part_type = 'Mon' where tabname = '" + str_4 + "'; ");
// 			}
// 			// else if (regex_match(str_3, result, week_reg))
// 			else if (str_3.find("EXTRACT(ISOYEAR FROM NEW") != string::npos && str_3.find("EXTRACT(WEEK FROM NEW") != string::npos)
// 			{
// 				string path_week = sql_path + "\\Week_Def";
// 				mkdir(path_week.c_str());
// 				string path_day = sql_path + "\\Day";
// 				mkdir(path_day.c_str());
//
// 				path_week += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_3(path_week);
// 				SQLFile_3 << str_3;
// 				SQLFile_3.close();
//
// 				path_day += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_4(path_day);
// 				SQLFile_4 << str_3;
// 				SQLFile_4.close();
//
// 				unsigned int cur_line = 0;
// 				string line_str, str_buffer;
//
// 				ifstream file_in_day;
// 				line_str = "";
// 				str_buffer = "";
// 				file_in_day.open(path_day);
// 				cur_line = 0;
// 				while (getline(file_in_day, line_str, '\n'))
// 				{
// 					if (line_str.find("EXTRACT(ISOYEAR FROM NEW") != string::npos && line_str.find("EXTRACT(WEEK FROM NEW") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "SELECT TO_CHAR(NEW.XXXXXXXXXXXXXXX, 'YYYY_MM_DD') INTO postfix_content;\n";
// 					}
// 					else if (line_str.find("check_begin :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_begin := '''' || NEW.XXXXXXXXXXXXXXX::DATE::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else if (line_str.find("check_end :=") != string::npos)
// 					{
// 						str_buffer += "--" + line_str + "\n";
// 						str_buffer += "check_end := '''' || (NEW.XXXXXXXXXXXXXXX::DATE + '1 day'::INTERVAL)::TEXT || '''::TIMESTAMP';\n";
// 					}
// 					else
// 					{
// 						str_buffer += line_str + "\n";
// 					}
//
// 					cur_line++;
// 				}
// 				file_in_day.close();
// 				ofstream os_day(path_day);
// 				os_day << str_buffer;
// 				os_day.close();
//
// 				conn_B->Execute("update public.trigger_func_content set part_type = 'Week' where tabname = '" + str_4 + "'; ");
// 			}
// 			// else if (regex_match(str_3, result, day_reg))
// 			else if (str_3.find("'YYYY_MM_DD') INTO") != string::npos || str_3.find("'YYYY_MM_DD') FROM 1 FOR 10) INTO") != string::npos)
// 			{
// 				string path_day = sql_path + "\\Day_Def";
// 				mkdir(path_day.c_str());
//
// 				path_day += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile_4(path_day);
// 				SQLFile_4 << str_3;
// 				SQLFile_4.close();
//
// 				conn_B->Execute("update public.trigger_func_content set part_type = 'Day' where tabname = '" + str_4 + "'; ");
// 			}
// 			else
// 			{
// 				string path_year = sql_path + "\\Year";
// 				mkdir(path_year.c_str());
// 				string path_mon = sql_path + "\\Month";
// 				mkdir(path_mon.c_str());
// 				string path_week = sql_path + "\\Week";
// 				mkdir(path_week.c_str());
// 				string path_day = sql_path + "\\Day";
// 				mkdir(path_day.c_str());
//
// 				sql_path += "\\" + str_1 + "." + str_2 + ".sql";
// 				ofstream SQLFile(sql_path);
// 				SQLFile << str_3;
// 				SQLFile.close();
// 			}
// 		}
//
// 		db_layer::DBLayer_FreeConn(conn_A);
// 	}
// }
//
// void Encrypt_test() {
// 		{
// 			std::string input_str = "";
// 			std::string  output_str = "";
// 			if (DBLayer_Encrypt(input_str, output_str))
// 			{
// 				std::cout << "origin str:" << input_str << ",encrypt str:" << output_str << std::endl;
// 			}
// 		}
// 		{
// 			std::string input_str = "杭州海康威视";
// 			std::string  output_str = "";
// 			if (DBLayer_Encrypt(input_str, output_str))
// 			{
// 				std::cout << "origin str:" << input_str << ",encrypt str:" << output_str << std::endl;
// 			}
// 		}
// 		{
// 			std::string input_str = "海康威视";
// 			std::string  output_str = "";
// 			if (DBLayer_Encrypt(input_str, output_str))
// 			{
// 				std::cout << "origin str:" << input_str << ",encrypt str:" << output_str << std::endl;
// 			}
// 		}
// 		{
// 			std::string input_str = "杭州";
// 			std::string  output_str = "";
// 			if (DBLayer_Encrypt(input_str, output_str))
// 			{
// 				std::cout << "origin str:" << input_str << ",encrypt str:" << output_str << std::endl;
// 			}
// 		}
// 		{
// 			std::string input_str = "杭州2024";
// 			std::string  output_str = "";
// 			if (DBLayer_Encrypt(input_str, output_str))
// 			{
// 				std::cout << "origin str:" << input_str << ",encrypt str:" << output_str << std::endl;
// 			}
// 		}
// 		{
// 			std::string input_str = "杭";
// 			std::string  output_str = "";
// 			if (DBLayer_Encrypt(input_str, output_str))
// 			{
// 				std::cout << "origin str:" << input_str << ",encrypt str:" << output_str << std::endl;
// 			}
// 		}
// 		{
// 			std::string input_str = "20";
// 			std::string  output_str = "";
// 			if (DBLayer_Encrypt(input_str, output_str))
// 			{
// 				std::cout << "origin str:" << input_str << ",encrypt str:" << output_str << std::endl;
// 			}
// 		}
// 		{
// 			std::string input_str = "2";
// 			std::string  output_str = "";
// 			if (DBLayer_Encrypt(input_str, output_str))
// 			{
// 				std::cout << "origin str:" << input_str << ",encrypt str:" << output_str << std::endl;
// 			}
// 	    }
//
// }
//
//
// void thread_test(db_layer::CConnect * conn,const std::string& lable) {
//
// 	std::string sql_insert = " INSERT INTO platform.test (id,info,updatetime,name,filepath) values("
// 		+ std::to_string(1) + ",\'test\',current_timestamp,\'test\',\'test_xxxxx_0\');";
//
// 	std::string sql_select = " select * from platform.test";
// 	if (conn) {
// 		std::cout << lable << "-thread begin" << std::endl;
// 		if (conn->Begin()){
// 			boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
// 			std::cout << lable << " begin success" << std::endl;
// 			bool exexute_status = true;
// 			if (!conn->ExecuteQuery(sql_insert)) {
// 				exexute_status = false;
// 				int error_code = 0;
// 				std::string error_msg = "";
// 				conn->GetLastError(error_code, error_msg);
// 				std::cout << "error_info:" << error_msg << std::endl;
// 			}
// 			if (!conn->Execute(sql_insert)) {
// 				exexute_status = false;
// 				int error_code = 0;
// 				std::string error_msg = "";
// 				conn->GetLastError(error_code, error_msg);
// 				std::cout << "error_info:" << error_msg << std::endl;
// 			}
// 			if (exexute_status) {
// 				std::cout << lable << " execute success" << std::endl;
// 				if (conn->Commit()) {
// 					std::cout << lable << " Commited " << std::endl;
// 				}
// 				else {
// 					std::cout << lable << " commit failed " << std::endl;
// 				}
// 			}
// 			else {
// 				std::cout << lable << " execute failed" << std::endl;
// 				if (conn->Rollback()) {
// 					std::cout << lable << " Rollbacked " << std::endl;
// 				}
// 				else {
// 					std::cout << lable << " Rollback failed " << std::endl;
// 				}
// 			}
// 		}
// 		else {
// 			std::cout << lable << " begin failed! " << std::endl;
// 		}
// 	}
// }
//
//
// void DBLayer_Init_SQLite_TEST() {
//
// 	std::string old_psw="Abc12345";
// 	std::string new_psw="xxxx";
//
// 	//{
// 	//	db_layer::DBParam dataimport_param_sqlite;
// 	//	dataimport_param_sqlite.db_name= Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "AccessController.db");
// 	//	dataimport_param_sqlite.db_type = DBType::SQLCipher3;
// 	//	dataimport_param_sqlite.passwd = "D1hr+ZwKjidKf2/iVq11Ab6TBkOQAkTni+sjmomdgxw=";
// 	//	dataimport_param_sqlite.max_conn = 3;
// 	//	dataimport_param_sqlite.min_conn = 1;
// 	//	int instance_id = 0;
// 	//	std::string sql = "select * from AccessGroup";
// 	//	bool db_layer_init = db_layer::DBLayer_Init(dataimport_param_sqlite, instance_id);
// 	//	if (db_layer_init) {
// 	//		auto conn = DBLayer_ApplyConn(instance_id);
// 	//		if (conn) {
// 	//			auto res=conn->ExecuteQuery(sql);
// 	//			if (!res) {
// 	//				std::string err_msg = "";
// 	//				int errcode = conn->GetLastErrorCode(err_msg);
// 	//				std::cout << "errormsg:" << err_msg<< std::endl;
// 	//			}
// 	//			else {
// 	//				std::cout << "query success"<<std::endl;
// 	//			}
// 	//
// 	//		}
// 	//		db_layer::DBLayer_Remove(instance_id);
// 	//	}
// 	//}
//
//
// 	{
// 		std::string dataimport_plugin_name = "dataimport.s";
// 		db_layer::DBParam dataimport_param_sqlite;
// 		dataimport_param_sqlite.db_type = db_layer::DBType::Type::SQLite;
// 		//param_sqlite.passwd = "Abc112233";
// 		dataimport_param_sqlite.db_name = Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "dataimport.db");
// 		dataimport_param_sqlite.min_conn = 10;
// 		dataimport_param_sqlite.max_conn = 10;
// 		bool db_layer_init = db_layer::Isomerism_Register(dataimport_param_sqlite, dataimport_plugin_name);
// 		if (db_layer_init) {
// 			std::cout << "success";
// 			auto sql = " SELECT flag_type, flag_sourcekey, flag_destkey, id FROM progressflag;";
// 			auto ptr_conn = db_layer::DBLayer_ApplyConn(dataimport_plugin_name);
// 			if (ptr_conn) {
// 				if (ptr_conn->Begin()) {
// 					std::cout << "begin success" << std::endl;
// 					std::cout << "apply a sqlite conn" << endl;
// 					if (!ptr_conn->ExecuteQuery(sql)) {
// 						int error_code = 0;
// 						std::string error_msg = "";
// 						ptr_conn->GetLastError(error_code, error_msg);
// 						std::cout << "error_info:" << error_msg << std::endl;
// 					}
// 					else {
// 						std::cout << "query success" << endl;
// 					}
// 					ptr_conn->Commit();
// 				}
// 			}
// 		}
// 	}
//
// 	std::string public_plugin_name = "public";
// 	db_layer::CPluginDBConfig public_config;
// 	public_config.set_plugin_name( public_plugin_name);
// 	public_config.set_db_file_name(Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "public.db"));
// 	public_config.set_sqlite_func_dll_name( Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "public.func.sqlite.dll"));
//
//
// 	db_layer::CPluginDBConfig platform_config;
// 	platform_config.set_plugin_name("platform");
// 	platform_config.set_db_file_name(Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "platform.db"));//db文件绝对路径
// 	platform_config.set_attach_to_public(true);//是否需要附加到public
//
// 	//db_layer::CPluginDBConfig map_config;
// 	//platform_config.set_plugin_name("map.s");
// 	//platform_config.set_db_file_name(Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "map.db"));//db文件绝对路径
// 	//platform_config.set_attach_to_public(true);//是否需要附加到public
//
// 	db_layer::CPluginDBConfig forgic_config;
// 	forgic_config.set_plugin_name("forgic.s");
// 	forgic_config.set_db_file_name(Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "forgic.db"));//db文件绝对路径
// 	forgic_config.set_attach_to_public(false);//是否需要附加到public
// 	forgic_config.set_load_to_public(false);
//
//
// 	db_layer::CSYSDbConfig sys_config;
// 	sys_config.add_db_config(public_config);
// 	sys_config.add_db_config(platform_config);
// 	sys_config.add_db_config(forgic_config);
// 	db_layer::SetSYSDBConfigParam(sys_config);
//
//
//
//
// 	db_layer::DBParam public_param_sqlite;
// 	public_param_sqlite.db_type = db_layer::DBType::Type::SQLite;
// 	public_param_sqlite.passwd = old_psw;
// 	public_param_sqlite.db_name = Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "public.db");
//
// 	public_param_sqlite.min_conn = 10;
// 	public_param_sqlite.max_conn = 10;
// 	bool db_layer_init = db_layer::DBLayer_Init(public_param_sqlite); // 初始化数据库
// 	if (db_layer_init) {
// 		std::cout << "sqlite_init public db success" << endl;
// 	}
//
// 	std::string platform_plugin_name = "platform";
// 	db_layer::DBParam platform_param_sqlite;
// 	platform_param_sqlite.db_type = db_layer::DBType::Type::SQLite;
// 	platform_param_sqlite.passwd = old_psw;
// 	platform_param_sqlite.db_name = Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "platform.db");
// 	platform_param_sqlite.min_conn = 10;
// 	platform_param_sqlite.max_conn = 10;
//
// 	bool db_register = db_layer::Module_Register(platform_param_sqlite, platform_plugin_name); // 初始化数据库
// 	if (db_register) {
// 		std::cout << "sqlite_module  platform db success" << endl;
// 		/*db_layer::CheckPoint(platform_plugin_name);*/
// 	}
//
//
// 	std::string create_table_sql1 = "CREATE TABLE map(\
// 		name            TEXT    NOT NULL UNIQUE,\
// 		type            INT     NOT NULL UNIQUE,\
// 		filepath        TEXT    NOT NULL,\
// 		updatetime      TEXT    NOT NULL,\
// 		primary key(name, type)\
// 		); ";
// 	std::string create_table_sql2 = "CREATE TABLE test(\
//            id int,\
//            info CHAR(100),\
//            updatetime      TEXT    NOT NULL,\
// 		   name  CHAR(100),\
// 		   filepath  text); ";
//
//
// 	/**
// 	 * 重置密码
// 	 */
// 	//std::string passwd = new_psw;
// 	//bool res = db_layer::DBLayer_ReSetDbPsw(passwd);
// 	//if (res) {
// 	//	std::cout << "reset psw success" << endl;
// 	//}
// 	//else {
// 	//	return;
// 	//}
//
// 	std::string DBUpgradeTool = "tool";
// 	db_layer::DBParam upgrade_param_sqlite;
// 	upgrade_param_sqlite.db_type = DBType::SQLite;
// 	bool db_register_dbupgrade = db_layer::Module_Register(upgrade_param_sqlite, DBUpgradeTool); // 初始化数据库
// 	if (db_register_dbupgrade) {
// 		std::cout << "sqlite_module  db_register_dbupgrade success" << endl;
// 		/*db_layer::CheckPoint(platform_plugin_name);*/
// 	}
//
// 	/*{
// 		auto ptr_conn = db_layer::DBLayer_ApplyConn("platform");
// 		if (ptr_conn) {
// 			std::cout << "apply a sqlite  public conn" << endl;
// 			for (int i = 0; i < 5000; i++) {
// 				std::string sql_insert = " INSERT INTO platform.test (id,info,updatetime,name,filepath) values("
// 					+ std::to_string(i) + ",\'test\',current_timestamp,\'test\',\'test\') RETURNING(id)";
// 				bool res = ptr_conn->Execute(sql_insert);
// 				if (!res) {
// 					int error_code = 0;
// 					std::string error_msg = "";
// 					ptr_conn->GetLastError(error_code, error_msg);
// 					std::cout << "error_info:" << error_msg << std::endl;
// 					break;
// 				}
// 			}
//
// 		}
//
// 	}*/
//
//
// 	{
// 		auto ptr_conn_B = db_layer::DBLayer_ApplyConn(DBUpgradeTool);
// 		auto ptr_conn_A = db_layer::DBLayer_ApplyConn(DBUpgradeTool);
// 		std::string A = "A";
// 		std::string B = "B";
// 		boost::function<void(db_layer::CConnect * , const std::string& )> func_manager_A = boost::bind(&thread_test, ptr_conn_A, A);
// 		boost::thread A_thread(func_manager_A, ptr_conn_A,A);
// 		A_thread.detach();
//
// 		boost::function<void(db_layer::CConnect *, const std::string&)> func_manager_B = boost::bind(&thread_test, ptr_conn_B, B);
// 		boost::thread B_thread(func_manager_B, ptr_conn_B, B);
// 		B_thread.detach();
//
// 		boost::this_thread::sleep(boost::posix_time::seconds(100));
//
// 		if (ptr_conn_A) {
// 			db_layer::DBLayer_FreeConn(ptr_conn_A);
// 		}
// 		if (ptr_conn_B) {
// 			db_layer::DBLayer_FreeConn(ptr_conn_B);
// 		}
// 	}
// 	db_layer::DBLayer_Remove();
// }
//
// void DBLayer_Init_ISO_SQLite_TEST() {
//
//
// 	db_layer::CPluginDBConfig deviceaccess_config;
// 	deviceaccess_config.set_plugin_name("deviceaccess.d");
// 	deviceaccess_config.set_db_file_name(Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "deviceaccess.db"));//db文件绝对路径
// 	deviceaccess_config.set_attach_to_public(false);//是否需要附加到public
// 	deviceaccess_config.set_load_to_public(false);
//
// 	db_layer::CSYSDbConfig sys_config;
// 	//sys_config.add_db_config(public_config);
// 	//sys_config.add_db_config(platform_config);
// 	//sys_config.add_db_config(forgic_config);
// 	sys_config.add_db_config(deviceaccess_config);
// 	db_layer::SetSYSDBConfigParam(sys_config);
//
// 	std::string platform_plugin_name = "deviceaccess.d";
// 	db_layer::DBParam platform_param_sqlite;
// 	platform_param_sqlite.db_type = db_layer::DBType::Type::SQLite;
// 	platform_param_sqlite.passwd = "default##pwd";
// 	platform_param_sqlite.db_name = Bee::Base::StringUtil::UTF82Local(Bee::Base::FileUtil::GetCurExeDir() + "deviceaccess.db");
// 	platform_param_sqlite.min_conn = 10;
// 	platform_param_sqlite.max_conn = 10;
//
//
// 	bool db_register = db_layer::Isomerism_Register(platform_param_sqlite, "deviceaccess.d"); // 初始化数据库
// 	if (db_register) {
// 		std::cout << "sqlite_module  platform db success" << endl;
// 		/*db_layer::CheckPoint(platform_plugin_name);*/
//
// 		auto conn = db_layer::DBLayer_ApplyConn("deviceaccess.d");
// 		if (conn) {
//
// 			std::string sql = "SELECT alarm_input_id,alarm_host_id,name,alarm_input_no,is_local_zone\
// 				, zone_type, detector_type, enter_delay, exit_delay, is_support_bypass\
// 				, subsystem_no, guid, linkage_subsystem_no FROM deviceaccess.zone LEFT JOIN\
// 				deviceaccess.alarm_input ON deviceaccess.zone.alarm_input_id = deviceaccess\
// 				.alarm_input.id WHERE deviceaccess.alarm_input.is_deleted = 0 ORDER BY\
// 				deviceaccess.zone.alarm_host_id;";
// 			if (conn->ExecuteQuery(sql)) {
// 				if (!conn->IsEOF()) {
// 					std::cout << "is not eof" << std::endl;
// 				}
// 			}
// 			else {
// 				int error_code=0;
// 				std::string err = "";
// 				conn->GetLastErrorCode(error_code, err);
// 				std::cout << "error:" << err << std::endl;
// 			}
//
// 		}
// 	}
//
//
// }
//
//
// void DBLayer_Init_PG_TEST() {
//
// 	std::string old_psw = "Abc12345++";
// 	std::string new_psw = "xxxx";
//
// 	std::string plugin_name = "public";
// 	db_layer::DBParam param_pg;
// 	param_pg.db_type = db_layer::DBType::Type::PostgreSQL;
// 	param_pg.ip = "10.19.215.206";
// 	param_pg.port = 5432;
// 	param_pg.user = "postgres";
// 	param_pg.passwd = "Abc12345==";
// 	param_pg.db_name = "cms";
// 	param_pg.min_conn = 10;
// 	param_pg.max_conn = 10;
// 	bool db_layer_init = db_layer::DBLayer_Init(param_pg);
// 	if (db_layer_init) {
// 		std::cout << "success";
// 		if (Module_Register(param_pg, "her.s")) {
// 			std::cout << "Module_Register success" << std::endl;
// 		}
//
// 		if (Module_Register(param_pg, "alarm.s")) {
// 			std::cout << "Module_Register success" << std::endl;
// 		}
//
// 		if (Module_Register(param_pg, "platform.s")) {
// 			std::cout << "Module_Register success" << std::endl;
// 		}
//
// 		for (int i = 0; i < 100; i++)
// 		{
// 			auto ptr_conn = db_layer::DBLayer_ApplyConn("her.s");
// 			if (ptr_conn) {
// 				std::cout << "apply a  conn" << endl;
// 			}
// 			DBLayer_FreeConn(ptr_conn);
// 		}
// 	}
// 	/**
// 	* 重置密码
// 	*/
// 	//std::string passwd = "Abc12345++";
// 	//bool res = db_layer::DBLayer_ReSetDbPsw(passwd);
// 	//if (res) {
// 	//	std::cout << "reset psw success" << endl;
// 	//}
// 	//else {
// 	//	return;
// 	//}
//
// 	for (int i = 0; i < 100; i++)
// 	{
// 		auto ptr_conn = db_layer::DBLayer_ApplyConn("her.s");
// 		if (ptr_conn) {
// 			std::cout << "apply a  conn" << endl;
// 		}
// 		DBLayer_FreeConn(ptr_conn);
// 	}
//
// 	db_layer::DBLayer_Remove();
// }
//
// int main()
// {
// 	Bee::Base::details::CLog::Instance().InitLog("test.log4cxx.properties");
// 	/*DBParam param_s;
// 	param_s.db_type = DBType::SQLServer;
// 	param_s.ip = "10.67.32.57";
// 	param_s.port = 1433;
// 	param_s.user = "sa";
// 	param_s.passwd = "Abc123456";
// 	param_s.db_name = "master";
// 	param_s.min_conn = 1;
// 	param_s.max_conn = 3;
//
// 	db_layer::Isomerism_Register(param_s, "xxx");*/
//
// 	/*Encrypt_test();*/
// 	/*DBLayer_Init_PG_TEST();*/
// 	//DBLayer_Init_ISO_SQLite_TEST();
// 	DBLayer_Init_SQLite_TEST();
//
// 	system("pause");
// 	// std::string strSource = "1, 2,3 , 4 , 5  , ,   ,,";
// 	// int iContType = 1;
// 	std::string strSource = "'abc', 'xyz', '123'   , '!@#', '\\n\\t', 'mn'o";
// 	int iContType = 2;
//
// 	std::string strTransRet("");
// 	std::vector<std::string> vct_str;
//
// 	// IN条件必须使用逗号分隔
// 	Bee::Base::StringUtil::String2Array(vct_str, strSource, ',');
//
// 	switch (iContType)
// 	{
// 		// 数值类型（INT...）
// 	case 1:
// 		// 遍历字符串队列，依次判断是否为数值类型，若出现非数值内容，则赋值为0
// 		for (auto it : vct_str)
// 		{
// 			strTransRet += strTransRet.empty() ? "" : ",";
//
// 			if (!it.empty())
// 			{
// 				// 去首尾空格，规避格式干扰
// 				it.erase(0, it.find_first_not_of(" "));
// 				it.erase(it.find_last_not_of(" ") + 1);
//
// 				strTransRet += it.find_first_not_of("0123456789") == string::npos ? it : "0";
// 			}
// 		}
// 		break;
// 		// 字符类型（TEXT...）
// 	case 2:
// 		for (auto it : vct_str)
// 		{
// 			strTransRet += strTransRet.empty() ? "" : ",";
//
// 			// 去首尾空格，规避格式干扰
// 			it.erase(0, it.find_first_not_of(" "));
// 			it.erase(it.find_last_not_of(" ") + 1);
//
// 			// 未被完整的单引号括住
// 			if (it.find('\'') == std::string::npos || it.find_first_of('\'') == it.find_last_of('\''))
// 			{
// 				// 若单引号数量不足以括住字符串，则SQL异常
// 				return false;
// 			}
//
// 			// 去空格后，单引号未放置在字符串的首尾
// 			if (it.find_first_of('\'') != 0 || it.find_last_of('\'') != it.size() - 1)
// 			{
// 				// 存在SQL注入的风险
// 				return false;
// 			}
//
// 			// 默认带双边单引号
// 			it.erase(0, it.find_first_not_of('\''));
// 			it.erase(it.find_last_not_of('\'') + 1);
//
// 			// 替换单引号，覆盖明确为字符串输入的场景
// 			strTransRet += "'" + Bee::Base::StringUtil::StringReplace(it, "'", "''") + "'";
// 		}
// 		break;
// 	default:
// 		break;
// 	}
//
//
//
// 	// DBLayerConnectMode_MySQL();
// 	DBLayerConnectMode_MySQLDB();
//
//
//
// 	//DBLayerConnectMode_SQLServerDB();
// 	//DBLayerConnectMode_SQLite();
// 	char* collate_str = setlocale(LC_COLLATE, NULL);
//
// 	cout << "Wait" << endl;
//
// 	// DirectConnectMode_Oracle();
//
// 	// Sleep(600000);
//
// 	// DBLayerConnectMode_DB();
//
// 	// DirectConnectMode_SQLServer();
//
// 	// DBLayerConnectHistory_PostgreSQL();
//
// 	DBLayerConnectMode_PostgreSQL();
//
// 	// DirectConnectMode_SQLServer();
//
// 	DBParam param;
// 	param.db_type = DBType::MySQL;
// 	// param.ip = "10.230.0.92";
// 	param.ip = "10.19.218.229";
// 	// param.port = 23306;
// 	param.port = 3306;
// 	param.user = "root";
// 	// param.passwd = "TXqeeC6MgvyQoy3Z7CQ0";
// 	param.passwd = "Abc12345";
// 	// param.db_name = "hpc_uat_2_convergence_cloud_vms";
// 	param.db_name = "cms";
// 	param.min_conn = 100;
// 	param.max_conn = 150;
//
// 	bool k_pool = db_layer::Isomerism_Register(param, "Plugin_C");
//
// 	Sleep(5000);
//
// 	int try_times = 100;
// 	while (try_times--)
// 	{
// 		boost::function<void()> conn_func = boost::bind(&DBLayerConnectMode_MySQL, try_times);
// 		boost::thread thrd(conn_func);
// 	}
//
// 	Sleep(180000);
//
// 	db_layer::DBLayer_Remove();
//
// 	// system("pause");
// 	return 0;
// }
// #endif
//
// #ifdef __linux__
// using namespace std;
// using namespace db_layer;
// void DBLayerConnectMode_JDBC_MySQL()
// {
// 	db_layer::DBParam param;
// 	//param.db_type = DBType::PostgreSQL;
// 	//param.ip = "127.0.0.1";
// 	//param.port = 5432;
// 	//param.user = "postgres";
// 	//param.passwd = "ZjhhYTE0ZGEyMzAxZTIwMWU4MTdmNWI4NjY3YTM2YmI0MGM4Y2E0OWRhNjliMzQ3MGE3NGQwZjRlYzE5NDk2MQ==";
// 	//param.db_name = "postgres";
// 	param.db_type = db_layer::DBType::MySQL;
// 	/****************** MYcat ***********************/
// 	//param.ip = "10.19.218.219";
// 	//param.port = 8066;
// 	////param.user = "user_ccfperson";
// 	//param.user = "root";
// 	//param.passwd = "Abc12345";
// 	//param.db_name = "db_global";
// 	////param.db_name = "db_ccfperson";
// 	/****************** MYSQL ******************************/
// 	param.ip = "10.19.218.242";
// 	param.port = 3306;
// 	param.user = "root";
// 	param.passwd = "Abc12345";
// 	param.db_name = "db_global";
// 	param.min_conn = 5;
// 	param.max_conn = 10;
//
// 	// 初始化数据库接入层
// 	db_layer::DBLayer_Init(param);
// 	const std::string plugin("Plugin_A");
// 	db_layer::Module_Register(param, plugin);
//
// 	bool k_conn = db_layer::DBLayer_Valid(plugin);
// 	if (k_conn)
// 	{
// 		{
// 			std::cout << "apply a conn[0]" << endl;
// 			db_layer::CConnect* conn_A = db_layer::DBLayer_ApplyConn(plugin);
// 			std::cout << "Free a conn[1]" << endl;
// 			conn_A->Begin();
// 			conn_A->ExecuteQuery("SELECT 1;");
// 			conn_A->Commit();
// 			db_layer::DBLayer_FreeConn(conn_A);
//
// 			std::cout << "apply a conn[2]" << endl;
// 			db_layer::CConnect* conn_B = db_layer::DBLayer_ApplyConn(plugin);
//
// 			//bool ver_ret = conn_B->ExecuteQuery("SELECT id from ccfperson_group;");
// 			bool ver_ret = conn_B->ExecuteQuery("SELECT * from test_bit;");
// 			conn_B->First();
// 			for (; ver_ret && !conn_B->IsEOF(); conn_B->Next())
// 			{
// 				/*std::string vers = conn_B->GetString("id");*/
// 				auto res = conn_B->GetDecimal("demic",3);
// 				std::string ss3 = Bee::Base::StringUtil::T2String(res);
// 				std::cout << ss3 << std::endl;
// 				//std::string vers = conn_B->GetString(conn_B->GetFieldIndex("my_bit"));
// 				//cout << res << endl;
// 			}
// 			std::cout << "Free a conn[3]" << endl;
// 			db_layer::DBLayer_FreeConn(conn_B);
// 		}
// 		if (!db_layer::DBLayer_Remove())
// 		{
// 			std::cout << "remove failed" << endl;
// 			/*cout << "dblayer-" << dblayer_no << ": remove ptr_dblayer failed..." << endl;
// 			IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, remove ptr_dblayer failed...", dblayer_no);*/
// 		}
// 		else
// 		{
// 			std::cout << "removed" << endl;
// 			/*cout << "dblayer-" << dblayer_no << ": ptr_dblayer has been removed." << endl;
// 			IVMS_LOG_INFO("DBLayer", "[DBLayerConnectMode_PostgreSQL] dblayer : %d, ptr_dblayer has been removed.", dblayer_no);*/
// 		}
//
// 		return;
// 	}
// }
//
// //测试老接口是否兼容
// void DBLayerConnectMode_JDBC_MySQL_OLD()
// {
// 	db_layer::DBParam param;
// 	param.db_type = db_layer::DBType::MySQL;
// 	/****************** MYSQL ***********************/
// 	param.ip = "10.19.218.229";
// 	param.port = 8066;
// 	param.user = "user_ccfperson";
// 	param.passwd = "Abc12345";
// 	param.db_name = "db_ccfperson";
// 	/****************** MYcat ******************************/
// 	/*param.ip = "10.19.218.242";
// 	param.port = 3306;
// 	param.user = "user_mycat_m1";
// 	param.passwd = "Abc12345";
// 	param.db_name = "db1";*/
// 	param.min_conn = 5;
// 	param.max_conn = 10;
// 	int dblayer_no = 0;
// 	int instance = 5;
// 	db_layer::DBLayer_Init(param, instance);
// 	CConnect* ptr_conn = DBLayer_ApplyConn(instance);
// 	bool ver_ret = ptr_conn->ExecuteQuery("SELECT id from ccfperson_group;");
// 	ptr_conn->First();
// 	for (; ver_ret && !ptr_conn->IsEOF(); ptr_conn->Next())
// 	{
// 		/*std::string vers = conn_B->GetString("id");*/
// 		std::string vers = ptr_conn->GetString(ptr_conn->GetFieldIndex("id"));
// 		cout << vers << endl;
// 	}
// 	db_layer::DBLayer_FreeConn(ptr_conn,instance);
// 	DBLayer_Remove(instance);
// }
//
// int main()
// {
// 	// DBLayerConnectMode_MySQL();
//
// 	char* collate_str = setlocale(LC_COLLATE, NULL);
//
// 	cout << "Wait" << endl;
// 	//DBLayerConnectMode_JDBC_MySQL_OLD();
// 	DBLayerConnectMode_JDBC_MySQL();
//
// 	int try_times = 100;
// 	// while (try_times--)
// 	// {
// 	// 	boost::function<void()> conn_func = boost::bind(DBLayerConnectMode_JDBC_MySQL, try_times);
// 	// 	boost::thread thrd(conn_func);
// 	// }
// 	// sleep(180000);
//
//
// 	// system("pause");
// 	return 0;
// }
//
// #endif