#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <boost/filesystem.hpp>
#include <atomic>
#include <future>

#include "constants.hpp"
#include "..\TextPreprocessor\ThreadPool.h"
#include "KDTree.h"

using namespace boost::filesystem;

const int DIM = 4;


class InquiryProcessor {
private:
	path curPath;
	std::vector<int> docID;
	std::vector<std::string> docNames;
	std::vector<std::vector<double>> matrix;
	int docs;
	std::string saverPath;
	ThreadPool threadPool;
	std::atomic<double> dist;
	std::atomic<int> counter;
	KDNode* root;
	std::vector<CPoint> KDTreeBase;

public:
	InquiryProcessor (ThreadPool & _threadPool, std::string _saver): threadPool(_threadPool), saverPath(_saver) {
		std::ifstream ifs(saverPath);
		std::string textsPath;
		getline(ifs, textsPath);
		curPath = path(textsPath);
		std::string text;
		int words = 0;
		std::string str; 
		int counter = 0;
		std::vector<double> newText;
		while (getline(ifs, str)) {
			while(str.find(" ") != std::string::npos) {	
				text = str.substr(0, str.find(" "));
				str = str.substr(str.find(" ") + 1);
				newText.push_back(atof(text.c_str()));
			}
			matrix.push_back(newText);
			CPoint kdTreePoint(newText, counter);
			counter++;
			KDTreeBase.push_back(kdTreePoint);
			newText.clear();
		}
		ifs.close();

		directory_iterator end_iter;
		try {
			if (exists(curPath)) {
				if (is_regular_file(curPath)) {
					docs = 1;
					docNames.push_back(curPath.filename().generic_string());
				}
				docs = 0;
				if (is_directory(curPath)) {
					for (directory_iterator p(curPath); p != end_iter; ++p) {
						docNames.push_back(p->path().filename().generic_string());
						docs++;
					}
				}				
			}
		} catch (const filesystem_error& ex) {
			std::cout << ex.what() << std::endl;
		} 
		for (int i = 0; i < docs; ++i) {
			docID.push_back(i);
		}
		// build для одного, потом выкидываем точку из кдтри бэйз и снова строим вектор рутов
		root = BuildKDTree(KDTreeBase);
	}	

	double parallelDistCounter (double a, double b) {
		counter++;
		double res = pow((a - b), 2);
		counter--;
		return res;
	}

	double distance(const std::vector<double> & a, const std::vector<double> & b) {
		std::vector<std::future<double>> futures;
		double sum = 0;
		for (size_t i = 0; i < a.size(); ++i) {
			futures.push_back(std::async(&InquiryProcessor::parallelDistCounter, this, a[i], b[i]));
		}
		while (counter != 0)
			;
		for (auto & fut: futures) {
			sum = sum + fut.get();
		}
		futures.clear();
		return pow(sum, 0.5);
	}

	std::vector<std::string> inquiry(int k, int docNumber) {
		if ((k > docs) || (docNumber >= docs) || (docNumber < 0) || (k < 0)) {
			throw std::invalid_argument("Illegal argument!" );
   		}
		//std::pair<size_t, double> result;
		//FindClosestPoint(root, KDTreeBase[docNumber], result);
		//QueryProcessing(KDTreeBase, root);
		//std::cout<< "Tester! " << result.first << " " << result.second << std::endl;
		std::nth_element(docID.begin(), docID.begin() + k, docID.end(), [&](size_t i, size_t j) {
			if (i == docNumber) {
				return false;
			}
			if (j == docNumber) {
				return true;
			}
			return GetDistanceBetweenPoints(KDTreeBase[i], KDTreeBase[docNumber]) < GetDistanceBetweenPoints(KDTreeBase[j], KDTreeBase[docNumber]);
		});
		std::vector<std::string> result;
		for (size_t i = 0; i < k; ++i) {
			result.push_back(docNames[docID[i]]);
		}
		return result;
	}

	int findTextNumber(std::string text) {
		for (int i = 0; i < docNames.size(); ++i) {
			if (docNames[i] == text) {
				return i;
			}
		}
		return -1;
	}

	void textProcess (std::string fPath) {
		std::map<std::string, double> preprocessedTexts;
		std::string filePath = fPath;
		std::ifstream ifs(filePath);
		std::string text;
		int words = 0;
		std::string str;
		while (getline(ifs, str)) {
			while(str.find(" ") != std::string::npos) {	
				text = str.substr(0, str.find(" "));
				str = str.substr(str.find(" ") + 1);
				if (preprocessedTexts.find(text) == preprocessedTexts.end()) {
					//std::vector<float> newWord;
					//newWord.assign(docs, 0);
					preprocessedTexts.insert(std::make_pair(text, 0));
				}
				preprocessedTexts.find(text)++;
			}
			if (preprocessedTexts.find(str) == preprocessedTexts.end()) {
				//std::vector<float> newWord;
				//newWord.assign(docs, 0);
				preprocessedTexts.insert(std::make_pair(str, 0));
			}
			preprocessedTexts.find(str)++;
		}
		std::vector<double> v;
		for (std::map<std::string, double>::iterator iter = preprocessedTexts.begin(); iter != preprocessedTexts.end(); ++iter) {
			v.push_back(iter->second);
		}
		if (v.size() > 100) {
			v.resize(100);
		}
		CPoint point(v, docs + 1);
		int n = QueryProcessing(KDTreeBase, root, point, docs + 1);
		std::cout << docNames[n] << std::endl;
	}
};
