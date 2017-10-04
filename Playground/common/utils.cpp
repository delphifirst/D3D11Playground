#include "utils.h"

#include <cassert>

using namespace std;

vector<char> ReadFile(const string& filename)
{
	vector<char> content;
	ifstream fin(filename, ios_base::binary);
	assert(fin);
	fin.seekg(0, ios_base::end);
	int file_size = fin.tellg();
	fin.seekg(0, ios_base::beg);
	content.resize(file_size);
	fin.read(content.data(), file_size);
	return content;
}