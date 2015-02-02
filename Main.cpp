#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include "FindStrategy.h"
#include "..\TextFinder\constants.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	const char DIRECTORY_OPTION[] = "dir";
	const char THREADS_OPTION[] = "threads";
	int threadsNumber = 3;
	boost::program_options::options_description desc("General options");
	std::string task_type;
	desc.add_options()
    ("dir,p", boost::program_options::value<std::string>(), "Directory path")
	("save,s", boost::program_options::value<std::string>(), "Saver path")
	("threads,t", boost::program_options::value<std::string>(), "Number of the threads")
    ;
	boost::program_options::variables_map vm;
	boost::program_options::parsed_options parsed = boost::program_options::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
	boost::program_options::store(parsed, vm);
	boost::program_options::notify(vm);
	bool directoryPassed = vm.count("dir");
	string path;
	string saver;
	if (directoryPassed) {
		path = vm[DIRECTORY_OPTION].as<std::string>();
	} else {
		path = boost::filesystem::current_path().generic_string();
	}
	if (vm.count(THREADS_OPTION)) {
		threadsNumber = boost::lexical_cast<int>(vm[THREADS_OPTION].as<std::string>());
    } else {
		threadsNumber = 3;
	}
	if (vm.count("save")) {
		saver = vm["save"].as<std::string>();
    } else {
		saver = boost::filesystem::current_path().generic_string();
	}
	std::ofstream info_file(saver);
	ThreadPool threadPool(threadsNumber);
	Preprocessor preprocessor(path, saver, threadPool); 
	ublas::matrix<float> reducedMatrix = preprocessor.preprocess();
	info_file.setf(ios::showpos);
	info_file.setf(ios::internal);
	info_file.setf(ios::fixed);
	info_file << path << std::endl;
	for (int i = 0; i < reducedMatrix.size1(); ++i) {
		for (int j = 0; j < reducedMatrix.size2(); ++j) {
			info_file << std::setprecision(0) << reducedMatrix(i, j) << " ";
		}
		info_file << std::endl;
	}
}
