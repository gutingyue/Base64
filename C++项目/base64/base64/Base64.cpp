#include "Base64.h"

//编码
string Base64::Encode(const string& strData){
	string strEncode;

	//编码表
	string EncodeTable("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	unsigned char temp[3];

	int x = 0;
	//3个一组
	for (size_t i = 0; i < strData.size() / 3; ++i){
		temp[0] = strData[x++];
		temp[1] = strData[x++];
		temp[2] = strData[x++];

		//第一个字节的高2位
		strEncode += EncodeTable[temp[0] >> 2];
		//第一个字节的低2位+第二个字节的高4位
		strEncode += EncodeTable[((temp[0] << 4) | (temp[1] >> 4)) & 0x3F];
		//第二个字节低4位+第三个字节高2位
		strEncode += EncodeTable[((temp[1] << 2) | (temp[2] >> 6)) & 0x3F];
		//第三个字节低6位
		strEncode += EncodeTable[temp[2] & 0x3F];
	}

	//模完后余两个字节
	if (strData.size() % 3 == 2){
		temp[0] = strData[x++];
		temp[1] = strData[x++];

		strEncode += EncodeTable[temp[0] >> 2];

		strEncode += EncodeTable[((temp[0] << 4) | (temp[1] >> 4)) & 0x3F];

		//最后4位，放在最后一个字节的低6位中的高4位
		strEncode += EncodeTable[(temp[1] << 2)  & 0x3F];
		strEncode += '=';

	}
	//模完后余一个字节
	else if (strData.size() % 3 == 1){
		temp[0] = strData[x++];

		strEncode += EncodeTable[temp[0] >> 2];

		//剩下2位放在最后一个字节低6位的高2位
		strEncode += EncodeTable[(temp[0] << 4) & 0x3F];

		strEncode += '=';
		strEncode += '=';
	}

	return strEncode;
}

//解码
string Base64::Decode(const string& strData){
	string strDecode;

	//解码表
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};

	size_t value;
	for(int i = 0; i < strData.size();){
		if (strData[i] != '\r'&&strData[i + 1] != '\n'){
			//一行没解析完

			//解码第一个编码
			value = DecodeTable[strData[i++]]<<18;

			//解码第二个编码
			value += DecodeTable[strData[i++]] << 12;
			strDecode += ((value >> 16) & 0xFF);

			if (strData[i] != '='){
				//解码第三个编码
				value += DecodeTable[strData[i++]] << 6;
				strDecode += ((value >> 8) & 0xFF);

				if (strData[i] != '='){
					//解码第四个编码
					value += DecodeTable[strData[i++]];
					strDecode += (value & 0xFF);
				}
			}
			else{
				break;
			}


		}
		else{
			//解码到达末尾，跳过"\r\n"
			i += 2;
		}
	}


	return strDecode;
}