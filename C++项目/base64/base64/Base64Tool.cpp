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

////Get请求的响应函数，helloworld( 客户端http请求 , 服务端http响应 ) //回调函数
//void helloworld(const httplib::Request& req,httplib::Response& rsp){
//	//rsp.set_content(响应内容，内容格式);
//	rsp.set_content("<html><head><title>helloworld!</title></head><body><h1>hello!</h1></body></html>","text/html");
//	rsp.status = 200;//响应状态码
//}


//对于注册的Post方法，路径为/str_to_base64的请求处理函数
void Str2Base64(const httplib::Request& req,httplib::Response& res){
	//收到前端发来的请求，对请求内容进行解析
	Json::Value req_val;
	Json::Reader re;
	if (!re.parse(req.body, req_val)){ //使用parse()方法对请求中的body部分进行解析，将解析数据放入val中,解析成功返回true,否则返回flase
		res.set_content("error", "text/html");
		res.status = 500;//失败返回状态码为500
		return;
	}
	//将文本转换为base64
	Base64 base64;
	string res_str = base64.Encode(req_val["strData"].asString());//获取数据，进行编码

	//对转码后的数据进行序列化
	Json::StyledWriter sw;//序列化对象
	Json::Value res_val;
	res_val["base64Data"] = res_str;//将转码后的数据放入val中
	res_str = sw.write(res_val);//对val中数据进行序列化

	//将转化结果发给客户端
	res.set_content(res_str, "text/html");
	res.status = 200;//成功返回状态码为200
}

void Base642Str(const httplib::Request& req,httplib::Response& res){
	//获取请求中的序列化数据
	Json::Value req_val;
	Json::Reader re;//反序列化对象
	if (!re.parse(req.body,req_val)){
		res.set_content("base64 to str error", "text/html");
		res.status = 500;
		return;
	}

	//将文本转换为base64
	Base64 base64;
	string res_str = base64.Decode(req_val["base64Data"].asString());//获取数据，进行编码
	//cout << res_str << endl;

	//对转码后的数据进行序列化
	Json::StyledWriter sw;//序列化对象
	Json::Value res_val;
	res_val["strData"] = res_str;//将转码后的数据放入val中
	res_str = sw.write(res_val);//对val中数据进行序列化

	//将转化结果发给客户端
	res.set_content(res_str, "text/html");
	res.status = 200;//成功返回状态码为200
}


void Pic2Base64(const httplib::Request& req, httplib::Response& res){
	//从请求中获取图片数据
	auto picFile = req.files;
	auto formdata = picFile.find("strpicData")->second;//从名为strpicData文件的第二个字段，拿到一个表单
	auto picdata = formdata.content;//从表单数据中拿到图片数据，获取表单内容，其内容为一张图片

	//将图片数据转换成base64编码
	Base64 base64;
	string res_str;
	//响应数据是一个标签，包括图片路径+转码后base64编码
	res_str += "<img src='data:img/jpg;base64,";
	res_str += base64.Encode(picdata);//获取数据，进行编码
	res_str += "'/>";

	//对转码后的数据进行序列化，使用Json来组织返回的数据
	Json::Value res_val;
	res_val["picData"] = res_str;//将转码后的数据放入val中

	Json::StyledWriter sw;//序列化对象
	res_str = sw.write(res_val);//对val中数据进行序列化

	//将转化结果构造成一个响应包，发给客户端
	res.set_content(res_str, "text/html");
	res.status = 200;//成功返回状态码为200

}

int main(){
	//Test_Encode();
	//Test_Decode();
	//TestJson(); //Json测试函数

	//搭建http服务器
	httplib::Server s;//创建服务器对象

	//注册请求
	//对Post请求进行注册, Post(资源路径,响应函数);
	s.Post("/str_to_base64", Str2Base64);

	s.Post("/base64_to_str", Base642Str);

	s.Post("/pic_to_base64", Pic2Base64);

	//设置目录
	s.set_base_dir(".//..//Debug");//.当前目录//..上一层目录//Debug目录
	//搜索框中只输入IP地址和端口号，没有给资源路径，默认在设置的路径下找html界面，进行响应

	s.listen("127.0.0.1", 9000);//本地回环地址


	return 0;
}