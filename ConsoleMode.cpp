#include "InquiryProcessor.hpp"
#include <boost/program_options.hpp>

int main(int argc, char *argv[]) {
	boost::program_options::options_description desc("General options");
	std::string task_type;
	desc.add_options()
    ("dir,p", boost::program_options::value<std::string>(), "Directory path")
    ;
	boost::program_options::variables_map vm;
	boost::program_options::parsed_options parsed = boost::program_options::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
	boost::program_options::store(parsed, vm);
	boost::program_options::notify(vm);
	bool directoryPassed = vm.count("dir");
	string path;
	if (directoryPassed) {
		path = vm["dir"].as<std::string>();
	} else {
		path = boost::filesystem::current_path().generic_string();
	}
	ThreadPool threadPool(std::thread::hardware_concurrency());
	InquiryProcessor finder(threadPool, path);

	std::locale cp_1251("Russian_Russia.1251");  //Russian language support
	std::locale::global(cp_1251);
	std::cout.imbue(cp_1251);
	std::ios::sync_with_stdio(false);

	//тестирование на первом корпусе текстов
	std::cout << "Корпус текстов - физика" << std::endl;
	std::cout <<  std::endl << "Тест1" << std::endl;
	std::vector<std::string> result;
	try {
		result = finder.inquiry(1, 0);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	std::cout <<  std::endl << "Тест2" << std::endl;
	result.clear();
	try {
		result = finder.inquiry(1, 2);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	std::cout <<  std::endl << "Тест3" << std::endl;
	result.clear();
	try {
		result = finder.inquiry(1, 100);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	std::cout << std::endl << "Тест4" << std::endl;
	result.clear();
	try {
		result = finder.inquiry(3, 0);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	std::cout << std::endl << "Тест5" << std::endl;
	result.clear();
	try {
		result = finder.inquiry(1, 4);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	//Тесты на "Войне и мире"
	/*std::cout << "Корпус текстов - первые пять глав из Войны и Мира" << std::endl;
	std::cout <<  std::endl << "Тест1" << std::endl;
	std::vector<std::string> result;
	try {
		result = finder.inquiry(1, 0);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	std::cout <<  std::endl << "Тест2" << std::endl;
	result.clear();
	try {
		result = finder.inquiry(1, 2);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	std::cout <<  std::endl << "Тест3" << std::endl;
	result.clear();
	try {
		result = finder.inquiry(1, 100);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	std::cout << std::endl << "Тест4" << std::endl;
	result.clear();
	try {
		result = finder.inquiry(3, 0);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}

	std::cout << std::endl << "Тест5" << std::endl;
	result.clear();
	try {
		result = finder.inquiry(1, 4);
	} catch (exception & err) {
		std::cout << err.what() << std::endl;
	}
	for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
		std::cout << *iter << std::endl;
	}*/
	
	int inquiries;
	std::cout << "Введите количество запросов: ";
	std::cin >> inquiries;
	std::string text;
	int k;
	for (size_t i = 0; i < inquiries; ++i) {
		std::cout << "Введите название файла: ";
		cin >> text;
		std::cout << "Введите количество близких файлов: ";
		cin >> k;
		result.clear();
		int num = finder.findTextNumber(text);
		if (num == -1) {
			std::cout << "Текст с таким именем не был найден" << std::endl;
			continue;

		}
	
		try {
			result = finder.inquiry(k, num);
		} catch (exception & err) {
			std::cout << err.what() << std::endl;
		}
		for (std::vector<std::string>::iterator iter = result.begin(); iter != result.end(); ++iter) {
			std::cout << *iter << std::endl;
		}
	}
		
	return 0;
}
