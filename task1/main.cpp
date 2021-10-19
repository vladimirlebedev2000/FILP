#include <iostream>
#include <algorithm>
#include <windows.h>
#include <sstream>
#include <map>
#include <ctime>
#include "allocator.h"
using namespace std;


#define DELIMITERS " .\n\r, ? !'\":;//()-_"
#define BOOK_NAME "text.txt"


char* read() {
	HANDLE file = CreateFile(BOOK_NAME, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	auto size = GetFileSize(file, nullptr);
	char* text = new char[size + 1];
	ReadFile(file, text, size, 0, NULL);
	CloseHandle(file);
	return text;
}

template <typename T>
void fill_map(char *text, T &words) {
	int a = 10;
	char *next_toc = NULL;
	char *chars_array = strtok_s(text, DELIMITERS, &next_toc);
	while (chars_array)
	{
		map <string, int> ::iterator pos;
		for (int i = 0; i <= strlen(chars_array); i++) {
			chars_array[i] = tolower(chars_array[i]);
		}
		pos = words.find(chars_array);
		if (pos == words.end()) {
			words[chars_array] = 1;
		}
		else {
			pos->second++;
		}
		chars_array = strtok_s(NULL, DELIMITERS, &next_toc);
	}
}


int main()
{
	map<string, int, less<string>, CustomAllocator<pair<string, int>>> custom_map;
	map<string, int> default_map;
	char *text = read();
	unsigned int start, time1, time2;
	start = clock();
	fill_map(text, custom_map);
	time1 = clock() - start;
	text = read();
	start = clock();
	fill_map(text, default_map);
	time2 = clock() - start;
	size_t size = default_map.size();
	auto *words = new pair<int, string>[size];
	int i = 0;
	for (auto word : default_map) {
		words[i] = pair<int, string>(word.second, word.first);
		i++;
	}
	sort(words, words + size);
	for (i = size - 1; i >=0 ; i--) {
		cout << words[i].second << "\t\t" << words[i].first << endl;
	}
	cout << "custom allocator time: " << time1 << "ms" << endl;
	cout << "standart allocator time: " << time2 << "ms" << endl;
	return 0;
}
