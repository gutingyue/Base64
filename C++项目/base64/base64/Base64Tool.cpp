#include "Base64.h"

#include <iostream>
#include <string>
using namespace std;

#include "httplib.h"

#include "json.h" //包含Json头文件
//加载Json静态库文件
#pragma comment(lib,".\\..\\Debug\\json_vc71_libmtd.lib")


//测试Json序列化使用
void TestJson(){
	Json::Value wr_val;

	//定义学生信息 {name:perper,gender:男,age:18,score:[70,80,90]}

	//将要序列化的数据，放入Json的al中
	wr_val["name"] = "perper";
	wr_val["gender"] = "男";
	wr_val["age"] = 18;

	int scores[] = { 70, 80, 90 };
	for (auto e : scores){
		wr_val["score"].append(e);//循环存入数组元素
	}

	//////序列化：将val中的数据序列化到一个字符串中
	//序列化对象（带格式的序列化）
	Json::StyledWriter sw;
	string strJsonData = sw.write(wr_val);//对val进行序列化，write序列化后，返回string类型字符串
	cout << strJsonData << endl;
	//{ 
	//	    "age" : 18,
	//		"gender" : "男",
	//		"name" : "perper",
	//		"score" : [70, 80, 90]
	//}
	
	//序列化对象（不带格式的序列化）
	Json::FastWriter fw;
	strJsonData = fw.write(wr_val);
	cout << strJsonData << endl;
	/*{"age":18, "gender" : "男", "name" : "perper", "score" : [70, 80, 90]}*/

	//////反序列化：将字符串中的数据还原到一个对象中
	Json::Reader rd;
	Json::Value re_val;

	rd.parse(strJsonData, re_val);//将字符串中的数据解析到re_val对象中
	cout << re_val["name"] << re_val["age"] << endl;

}

//编码
void Test_Encode(){
	string enData;

	cout << "enData:";
	getline(cin, enData);
	Base64 str;
	string enstr = str.Encode(enData);
	cout << enstr << endl;
}

//解码
void Test_Decode(){
	string deData;

	cout << "deData:";
	getline(cin, deData);
	Base64 str;
	string destr = str.Decode(deData);
	cout << destr << endl;
}

//hellworld( 客户端http请求 , 服务端http响应 ) //回调函数
void helloworld(const httplib::Request& req,httplib::Response& rsp){
	//rsp.set_content(响应内容，内容格式);
	rsp.set_content("<html><head><title>helloworld!</title></head><body><h1>hello!</h1></body></html>","text/html");
	rsp.status = 200;//响应状态码
}

int main(){
	//Test_Encode();
	//Test_Decode();

	TestJson();

	//搭建http服务器
	httplib::Server s;//创建服务器对象

	//注册请求
	//Get请求到来，连接服务器Get请求，调用helloworld回调函数来进行处理，
	//s.Get("/", helloworld);
	//对Post请求进行注册, Post(资源路径,响应函数);
	//s.Post("/", Str2Base64);

	//设置目录
	s.set_base_dir(".//..//Debug");//.当前目录//..上一层目录//Debug目录
	//搜索框中只输入IP地址和端口号，没有给资源路径，默认在设置的路径下找html界面，进行响应


	s.listen("127.0.0.1", 9000);//本地回环地址



	return 0;
}