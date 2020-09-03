#include "Base64.h"

#include <iostream>
#include <string>
using namespace std;

#include "httplib.h"

#include "json.h" //����Jsonͷ�ļ�
//����Json��̬���ļ�
#pragma comment(lib,".\\..\\Debug\\json_vc71_libmtd.lib")


//����Json���л�ʹ��
void TestJson(){
	Json::Value wr_val;

	//����ѧ����Ϣ {name:perper,gender:��,age:18,score:[70,80,90]}

	//��Ҫ���л������ݣ�����Json��al��
	wr_val["name"] = "perper";
	wr_val["gender"] = "��";
	wr_val["age"] = 18;

	int scores[] = { 70, 80, 90 };
	for (auto e : scores){
		wr_val["score"].append(e);//ѭ����������Ԫ��
	}

	//////���л�����val�е��������л���һ���ַ�����
	//���л����󣨴���ʽ�����л���
	Json::StyledWriter sw;
	string strJsonData = sw.write(wr_val);//��val�������л���write���л��󣬷���string�����ַ���
	cout << strJsonData << endl;
	//{ 
	//	    "age" : 18,
	//		"gender" : "��",
	//		"name" : "perper",
	//		"score" : [70, 80, 90]
	//}
	
	//���л����󣨲�����ʽ�����л���
	Json::FastWriter fw;
	strJsonData = fw.write(wr_val);
	cout << strJsonData << endl;
	/*{"age":18, "gender" : "��", "name" : "perper", "score" : [70, 80, 90]}*/

	//////�����л������ַ����е����ݻ�ԭ��һ��������
	Json::Reader rd;
	Json::Value re_val;

	rd.parse(strJsonData, re_val);//���ַ����е����ݽ�����re_val������
	cout << re_val["name"] << re_val["age"] << endl;

}

//����
void Test_Encode(){
	string enData;

	cout << "enData:";
	getline(cin, enData);
	Base64 str;
	string enstr = str.Encode(enData);
	cout << enstr << endl;
}

//����
void Test_Decode(){
	string deData;

	cout << "deData:";
	getline(cin, deData);
	Base64 str;
	string destr = str.Decode(deData);
	cout << destr << endl;
}

//hellworld( �ͻ���http���� , �����http��Ӧ ) //�ص�����
void helloworld(const httplib::Request& req,httplib::Response& rsp){
	//rsp.set_content(��Ӧ���ݣ����ݸ�ʽ);
	rsp.set_content("<html><head><title>helloworld!</title></head><body><h1>hello!</h1></body></html>","text/html");
	rsp.status = 200;//��Ӧ״̬��
}

int main(){
	//Test_Encode();
	//Test_Decode();

	TestJson();

	//�http������
	httplib::Server s;//��������������

	//ע������
	//Get�����������ӷ�����Get���󣬵���helloworld�ص����������д���
	//s.Get("/", helloworld);
	//��Post�������ע��, Post(��Դ·��,��Ӧ����);
	//s.Post("/", Str2Base64);

	//����Ŀ¼
	s.set_base_dir(".//..//Debug");//.��ǰĿ¼//..��һ��Ŀ¼//DebugĿ¼
	//��������ֻ����IP��ַ�Ͷ˿ںţ�û�и���Դ·����Ĭ�������õ�·������html���棬������Ӧ


	s.listen("127.0.0.1", 9000);//���ػػ���ַ



	return 0;
}