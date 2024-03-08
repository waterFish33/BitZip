#define _CRT_SECURE_NO_WARNINGS 1

#include"FCHuffMan.h"

FCHuffMan::FCHuffMan() {
	_fileinfo.resize(256);
	for (int i = 0; i < _fileinfo.size(); i++) {
		_fileinfo[i]._ch = i;
	}
}
void _GenerateHUffManCode(vector<ByteInfo>& _fileinfo ,HuffmanTreeNode<ByteInfo>* root, string add, string str) {
	str += add;
	if (root->_left == nullptr && root->_right == nullptr) {
		_fileinfo[root->_weight._ch]._chcode = str;
		return;
	}

	_GenerateHUffManCode(_fileinfo,root->_left, "0", str);
	_GenerateHUffManCode(_fileinfo,root->_right, "1", str);
}
void GenerateHUffManCode(vector<ByteInfo>& _fileinfo,HuffmanTree<ByteInfo>& ht) {
	string str;
	HuffmanTreeNode<ByteInfo>* root = ht.getroot();
	_GenerateHUffManCode(_fileinfo,root->_left, "0", str);
	_GenerateHUffManCode(_fileinfo,root->_right, "1", str);
}
void FCHuffMan::CompressFile(const string& filepath) {
	//ͳ��Դ�ļ���ÿ���ַ����ֵĴ���
	FILE* file = fopen(filepath.c_str(), "rb");
	if (file == nullptr) {
		cout << "�ļ���ʧ��"<<endl;
		return;
	}
	unsigned char rdBuff[1024];
	while (1) {
		size_t rdSize =fread(rdBuff,1,1024,file);
		if (rdSize == 0) {
			break;
		}
		for (size_t i = 0; i < rdSize; i++) {
			_fileinfo[rdBuff[i]]._appearcount++;
		}
	}
	//ͳ�ƵĽ�������������
	ByteInfo valid;
	HuffmanTree<ByteInfo> ht(_fileinfo, valid );
	//��ȡhuffman����
	GenerateHUffManCode(_fileinfo,ht);
	//�Ȱѽ�ѹ��������д���ļ���
	FILE* fout = fopen("2.hz", "wb");
	WriteHeadInfo(filepath, fout);
	//��huffman�����Դ�ļ����и�д
	fseek(file, 0, SEEK_SET);
	
	char bits = 0;
	int bitcount = 0;
	while (1) {
		size_t rdSize = fread(rdBuff, 1, 1024, file);
		if (rdSize == 0) {
			break;
		}
		for (size_t i = 0; i < rdSize; i++) {
			string strCode=_fileinfo[rdBuff[i]]._chcode;
			for (size_t j = 0; j < strCode.size(); j++) {
				bits <<= 1;
				if (strCode[j] == '1') {
					bits |= 1;
				}
				bitcount++;
				if (bitcount == 8) {//д��8�Ż������䣬���������ѭ��������Ҫ��ʣ�µ��������ļ���д
					fputc(bits, fout);
					bitcount = 0;
					bits = 0;
				}
			}
		}
	}
	//���һ�Σ���bitsʣ�µ����ݶ�д���ļ���
	if (bitcount > 0 && bitcount < 8) {
		bits <<= (8 - bitcount);
		fputc(bits, fout);
		bitcount = 0;
		bits = 0;
	}
	fclose(file);
	fclose(fout);
	
}
string FCHuffMan::GetFilePostFix(const string& filePath) {
	return filePath.substr(filePath.find_last_of('.') + 1);

}

void FCHuffMan::WriteHeadInfo(const string& filePath, FILE* fout) {
	//��ȡԭ�ļ���׺
	string headInfo;
	headInfo += GetFilePostFix(filePath);
	headInfo += '\n';
	//��ȡƵ����Ϣ
	size_t LineCount = 0;
	string chInfo;
	for (auto& e : _fileinfo) {
		if (e._appearcount == 0) {
			continue;
		}
		LineCount++;
		chInfo += e._ch;
		chInfo += ':';
		chInfo += to_string(e._appearcount);
		chInfo += '\n';
	}
	headInfo += to_string(LineCount);
	headInfo += '\n';
	headInfo += chInfo;
	fwrite(headInfo.c_str(), 1, headInfo.size(), fout);
}
void FCHuffMan::GetLine(FILE* fin, string& strInfo) {
	char ch=0;
	while (ch!=EOF) {
		ch = fgetc(fin);
		if (ch == '\n') {
			break;
		}
		strInfo += ch;
	}
}
void FCHuffMan::UNCompressFile(const string& filepath) {
	if (GetFilePostFix(filepath) != "hz") {
		cout << "�ļ���ʽ����ȷ" << endl;
		return;
	}
	FILE* fin = fopen(filepath.c_str(), "rb");
	if (fin == nullptr) {
		cout << "�ļ���ʧ��" << endl;
		return;
	}
	//��ȡԴ�ļ���׺
	string unCompressFile("222.");
	string strInfo;
	GetLine(fin, strInfo);
	unCompressFile += strInfo;

	//��ȡƵ����Ϣ����
	size_t lineCounnt = 0;
	strInfo = "";
	GetLine(fin, strInfo);
	lineCounnt = atoi(strInfo.c_str());
	for (size_t i = 0; i < lineCounnt; i++) {
		strInfo = ""; 
		GetLine(fin, strInfo);
		//���н������⴦��
		if (strInfo == "") {
			strInfo += '\n';
			GetLine(fin, strInfo);
		}
		unsigned char ch = strInfo[0];
		_fileinfo[ch]._appearcount = atoi(strInfo.c_str() + 2);
	}
	//��ԭhuffman��
	ByteInfo valid;	
	HuffmanTree<ByteInfo> ht(_fileinfo, valid);
	HuffmanTreeNode<ByteInfo>* cur = ht.getroot();
	//��ѹ��
	FILE* fout = fopen(unCompressFile.c_str(), "wb");
	size_t fileSize = 0;
	while (1) {
		unsigned char rdBuff[1024];
		size_t rdSize=fread(rdBuff, 1, 1024, fin);
		if (rdSize == 0) {
			break;
		}
		for (size_t i = 0; i < rdSize; i++) {
			unsigned char ch = rdBuff[i];
			for (int j = 0; j < 8; j++) {
				if (ch & 0x80) {
					cur = cur->_right;
				}else {
					cur = cur->_left;
				}
				ch <<= 1;
				if (cur->_left == nullptr && cur->_right == nullptr) {
					fputc(cur->_weight._ch, fout);
					fileSize++;
					cur = ht.getroot();
					if (fileSize == cur->_weight._appearcount) {
						break;
					}
				}
			}
			
		}
	}
	fclose(fout);
	fclose(fin);

}