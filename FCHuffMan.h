#pragma once
#include <string>
#include <queue>
#include <vector>
#include <iostream>
#include "HuffmanTree.h"
using namespace std;
struct ByteInfo
{
	unsigned char _ch;
	size_t _appearcount;
	string _chcode;
	ByteInfo(size_t count = 0):_appearcount(count) {
		
	}
	ByteInfo operator+(const ByteInfo& other)const {
		return ByteInfo(_appearcount + other._appearcount);
	}
	bool operator>(const ByteInfo& other)const {
		return _appearcount > other._appearcount;
	}
	bool operator!=(const ByteInfo& other)const {
		return _appearcount != other._appearcount;
	}
	bool operator==(const ByteInfo& other)const {
		return _appearcount == other._appearcount;
	}
};

class FCHuffMan {
private:
	vector<ByteInfo> _fileinfo;
	void WriteHeadInfo(const string& filePath, FILE* fout);
	string GetFilePostFix(const string& filePath);
	void GetLine(FILE* fin, string& strInfo);
public:
	FCHuffMan(); 
	void CompressFile(const string& filepath);
	void UNCompressFile(const string& filepath);
	
};