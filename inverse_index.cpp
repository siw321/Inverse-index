
#define CATCH_CONFIG_MAIN
#include <iostream>
#include <map>
#include <vector>
#include <cassert>
#include <string>
#include <fstream>
#include "catch.hpp"



using namespace std;

class inverse_index_interface {
public:
	virtual void load(string path) = 0;
	virtual string get_context(string word, int words_count) = 0;
	virtual string search(string word) = 0;
	virtual void output(string path) = 0;
};

class inverse_index: public inverse_index_interface {
public:
	void load(string path) override {
		ifstream fin;
		fin.open(path);
		getline(fin, text);
		fin.close();
		int last_space = -1;
		int space;
		for (int i = 0; i < text.length() + 1; i++) {
			if (text[i] < 'A') {
				space = i;
				string str(text, last_space + 1, space - last_space - 1);
				if (!str.empty()) {
					bool found = false;
					for (auto it : data_) {
						if (it.first == str) {
							data_[str].push_back(last_space);
							found = true;
							break;
						}
					}
					if (!found) {
						data_.insert(pair<string, vector<int>>(str, NULL));
						data_[str].push_back(last_space);
					}
				}
				last_space = space;
			}
		}
	}
	string get_context(string word, int words_count) override {
		string result = "";
		for (auto it : data_[word]) {
			result += "\t...";
			int i = it;
			for (int space_count = 0; i >= 0 && space_count - 1 < (words_count + 1) / 2; i--) {
				if (text[i] < 'A' && text[i + 1] >= 'A') {
					space_count++;
				}
			}
			{
				string first(text, i + 1, it - i);
				result += first;
			}
			i = it;
			for (int space_count = 0; i < text.length() && space_count - 2 < words_count / 2; i++) {
				if (text[i] < 'A' && text[i - 1] >= 'A') {
					space_count++;
				}
			}
			string second(text, it + 1, i - it - 1);
			result += second;
			result += "...\n";
		}
		return result;
	}
	string search(string word) override {
		string result = word + ": ";
		for (auto it : data_[word]) {
			result += to_string(it) + "  ";
		}
		return result + "\n" + get_context(word, 10);
	}
	void output(string path) override {
		ofstream fout;
		fout.open(path);
		for (auto it : data_) {
			fout << search(it.first);
		}
		fout << "\ntext:\n\t" << text;
		fout.close();
	}
protected:
	map<string, vector<int>> data_;
	string text;
};

class inverse_index_mock : public inverse_index {
public:
	void load(string path) override {
		text = this->text = path;
		int last_space = -1;
		int space;
		for (int i = 0; i < text.length() + 1; i++) {
			if (text[i] < 'A') {
				space = i;
				string str(text, last_space + 1, space - last_space - 1);
				if (!str.empty()) {
					bool found = false;
					for (auto it : data_) {
						if (it.first == str) {
							data_[str].push_back(last_space);
							found = true;
							break;
						}
					}
					if (!found) {
						data_.insert(pair<string, vector<int>>(str, NULL));
						data_[str].push_back(last_space);
					}
				}
				last_space = space;
			}
		}
	}
};

int main() {
#if true

	while (true) {
		inverse_index a;
		string b;
		cout << "enter input file path: ";
		cin >> b;
		a.load(b);
		cout << "enter output file path: ";
		cin >> b;
		a.output(b);
	}
#else
	inverse_index a;
	a.load("Lorem ipsum.txt");
	a.output("out.txt");
#endif
	return 0;
}
