#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <atomic>
#include <string>
#include <concurrent_unordered_map.h>
#include "ThreadPool.h"
#include "SVD.h"

using namespace concurrency;
using namespace boost::filesystem;


class Preprocessor {
private:
	path curPath;
	std::vector<std::string> docNames;
	std::map<std::string, std::vector<float>> preprocessedTexts;
	int docs;
	std::string saver;
	ThreadPool & threadPool;

public:
	Preprocessor(ThreadPool & _threadPool): threadPool(_threadPool){
		curPath = current_path();
		docs = 0;
	}

	Preprocessor(std::string path, std::string _saver, ThreadPool & _threadPool): curPath(path), threadPool(_threadPool), saver(_saver) {
		docs = 0;
	}
	
	void fileProcess(const path & fPath, int docNum) {
		std::string filePath = fPath.generic_string();
		std::ifstream ifs(filePath);
		std::string text;
		int words = 0;
		std::string str;
		while (getline(ifs, str)) {
			while(str.find(" ") != std::string::npos) {	
				text = str.substr(0, str.find(" "));
				str = str.substr(str.find(" ") + 1);
				if (preprocessedTexts.find(text) == preprocessedTexts.end()) {
					std::vector<float> newWord;
					newWord.assign(docs, 0);
					preprocessedTexts.insert(std::make_pair(text, newWord));
				}
				preprocessedTexts.find(text)->second[docNum]++;
			}
			if (preprocessedTexts.find(str) == preprocessedTexts.end()) {
				std::vector<float> newWord;
				newWord.assign(docs, 0);
				preprocessedTexts.insert(std::make_pair(str, newWord));
			}
			preprocessedTexts.find(str)->second[docNum]++;
		}
	}

	ublas::matrix <float> preprocess() {
		directory_iterator end_iter;
		try {
			if (exists(curPath)) {
				if (is_regular_file(curPath)) {
					docs = 1;
					docNames.push_back(curPath.filename().generic_string());
					fileProcess(curPath, 0);  //или использовать async
				}
				if (is_directory(curPath)) {
					for (directory_iterator p(curPath); p != end_iter; ++p) {
						docNames.push_back(p->path().filename().generic_string());
						docs++;
					}
					int docNum = 0;
					for (directory_iterator pathIter(curPath); pathIter != end_iter; ++pathIter) {
						fileProcess(pathIter->path(), docNum); //или использовать async
						docNum++;
					}
				}				
			}
		} catch (const filesystem_error& ex) {
			std::cout << ex.what() << std::endl;
		}  

		std::vector<std::vector<float>> result;
		for (std::map<std::string, std::vector<float>>::iterator iter = preprocessedTexts.begin(); iter != preprocessedTexts.end(); iter++) { 
			//std::cout << iter -> first << " " << iter -> second.size() << std::endl;  //отладочный вывод
			result.push_back(iter->second);
		}
		ublas::matrix <float> wordsFreq (result.size(), result[0].size());
		//std::cout << "Matrix: " << std::endl;
		for (int i = 0; i < result.size(); ++i) {
			for (int j = 0; j < result[i].size(); ++j) {
				wordsFreq(i, j) = result[i][j];
				//std::cout << result[i][j] << " ";
			}
			//std::cout << std::endl;
		}
		//std::cout << "Pre Size:" << result.size() * result[0].size() << std::endl;  отладочный вывод
		wordsFreq = ublas::trans(wordsFreq);
		return matrixReducing(wordsFreq);
	}



	~Preprocessor() {
		/*while(counter.load() != 0)
			;*/
	}
};
