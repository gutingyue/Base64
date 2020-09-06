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

////Get�������Ӧ������helloworld( �ͻ���http���� , �����http��Ӧ ) //�ص�����
//void helloworld(const httplib::Request& req,httplib::Response& rsp){
//	//rsp.set_content(��Ӧ���ݣ����ݸ�ʽ);
//	rsp.set_content("<html><head><title>helloworld!</title></head><body><h1>hello!</h1></body></html>","text/html");
//	rsp.status = 200;//��Ӧ״̬��
//}


//����ע���Post������·��Ϊ/str_to_base64����������
void Str2Base64(const httplib::Request& req,httplib::Response& res){
	//�յ�ǰ�˷��������󣬶��������ݽ��н���
	Json::Value req_val;
	Json::Reader re;
	if (!re.parse(req.body, req_val)){ //ʹ��parse()�����������е�body���ֽ��н��������������ݷ���val��,�����ɹ�����true,���򷵻�flase
		res.set_content("error", "text/html");
		res.status = 500;//ʧ�ܷ���״̬��Ϊ500
		return;
	}
	//���ı�ת��Ϊbase64
	Base64 base64;
	string res_str = base64.Encode(req_val["strData"].asString());//��ȡ���ݣ����б���

	//��ת�������ݽ������л�
	Json::StyledWriter sw;//���л�����
	Json::Value res_val;
	res_val["base64Data"] = res_str;//��ת�������ݷ���val��
	res_str = sw.write(res_val);//��val�����ݽ������л�

	//��ת����������ͻ���
	res.set_content(res_str, "text/html");
	res.status = 200;//�ɹ�����״̬��Ϊ200
}

void Base642Str(const httplib::Request& req,httplib::Response& res){
	//��ȡ�����е����л�����
	Json::Value req_val;
	Json::Reader re;//�����л�����
	if (!re.parse(req.body,req_val)){
		res.set_content("base64 to str error", "text/html");
		res.status = 500;
		return;
	}

	//���ı�ת��Ϊbase64
	Base64 base64;
	string res_str = base64.Decode(req_val["base64Data"].asString());//��ȡ���ݣ����б���
	//cout << res_str << endl;

	//��ת�������ݽ������л�
	Json::StyledWriter sw;//���л�����
	Json::Value res_val;
	res_val["strData"] = res_str;//��ת�������ݷ���val��
	res_str = sw.write(res_val);//��val�����ݽ������л�

	//��ת����������ͻ���
	res.set_content(res_str, "text/html");
	res.status = 200;//�ɹ�����״̬��Ϊ200
}


void Pic2Base64(const httplib::Request& req, httplib::Response& res){
	//�������л�ȡͼƬ����
	auto picFile = req.files;
	auto formdata = picFile.find("strpicData")->second;//����ΪstrpicData�ļ��ĵڶ����ֶΣ��õ�һ����
	auto picdata = formdata.content;//�ӱ��������õ�ͼƬ���ݣ���ȡ�����ݣ�������Ϊһ��ͼƬ

	//��ͼƬ����ת����base64����
	Base64 base64;
	string res_str;
	//��Ӧ������һ����ǩ������ͼƬ·��+ת���base64����
	res_str += "<img src='data:img/jpg;base64,";
	res_str += base64.Encode(picdata);//��ȡ���ݣ����б���
	res_str += "'/>";

	//��ת�������ݽ������л���ʹ��Json����֯���ص�����
	Json::Value res_val;
	res_val["picData"] = res_str;//��ת�������ݷ���val��

	Json::StyledWriter sw;//���л�����
	res_str = sw.write(res_val);//��val�����ݽ������л�

	//��ת����������һ����Ӧ���������ͻ���
	res.set_content(res_str, "text/html");
	res.status = 200;//�ɹ�����״̬��Ϊ200

}

int main(){
	//Test_Encode();
	//Test_Decode();
	//TestJson(); //Json���Ժ���

	//�http������
	httplib::Server s;//��������������

	//ע������
	//��Post�������ע��, Post(��Դ·��,��Ӧ����);
	s.Post("/str_to_base64", Str2Base64);

	s.Post("/base64_to_str", Base642Str);

	s.Post("/pic_to_base64", Pic2Base64);

	//����Ŀ¼
	s.set_base_dir(".//..//Debug");//.��ǰĿ¼//..��һ��Ŀ¼//DebugĿ¼
	//��������ֻ����IP��ַ�Ͷ˿ںţ�û�и���Դ·����Ĭ�������õ�·������html���棬������Ӧ

	s.listen("127.0.0.1", 9000);//���ػػ���ַ


	return 0;
}