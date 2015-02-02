/*#include "InquiryProcessor.hpp"

#define BOOST_TEST_MODULE TEXT_FINDER_TESTER

#include <boost/test/unit_test.hpp>

ThreadPool threadPool(std::thread::hardware_concurrency());
InquiryProcessor finder(threadPool);
std::vector<std::string> result;


//для каждой группы тестов надо делать отдельный препроцессинг(они базируются на разных корпусах текстов)

//naive tests with short texts
BOOST_AUTO_TEST_CASE(test1) {
	result.clear();
	result.push_back("test5.txt");
	BOOST_REQUIRE(finder.inquiry(1, 0) == result);
}

BOOST_AUTO_TEST_CASE(test2) {
	result.clear();
	result.push_back("test5.txt");
	result.push_back("test2.txt");
	result.push_back("test4.txt");
	BOOST_REQUIRE(finder.inquiry(3, 0) == result);
}

BOOST_AUTO_TEST_CASE(test3) {
	result.clear();
	result.push_back("test1.txt");
	result.push_back("test5.txt");
	result.push_back("test4.txt");
	result.push_back("test3.txt");
	BOOST_REQUIRE(finder.inquiry(4, 1) == result);
}




//tests based on directory with texts about countries (11 texts)
BOOST_AUTO_TEST_CASE(test4) {
	result.clear();
	result.push_back("test10.txt");
	result.push_back("test4.txt");
	result.push_back("test11.txt");
	BOOST_REQUIRE(finder.inquiry(3, 0) == result);
}

BOOST_AUTO_TEST_CASE(test5) {
	result.clear();
	result.push_back("test10.txt");
	BOOST_REQUIRE(finder.inquiry(1, 0) == result);
}

BOOST_AUTO_TEST_CASE(test6) {
	result.clear();
	result.push_back("test10.txt");
	result.push_back("test4.txt");
	result.push_back("test11.txt");
	result.push_back("test9.txt");
	result.push_back("test6.txt");
	result = finder.inquiry(5, 0);
	BOOST_REQUIRE(finder.inquiry(5, 0) == result);
}

BOOST_AUTO_TEST_CASE(test7) {
	result.clear();
	result.push_back("test6.txt");
	BOOST_REQUIRE(finder.inquiry(1, 9) == result);
}

BOOST_AUTO_TEST_CASE(test8) {
	result.clear();
	result.push_back("test6.txt");
	result.push_back("test9.txt");
	result.push_back("test11.txt");
	result.push_back("test10.txt");
	result.push_back("test5.txt");
	result.push_back("test3.txt");
	result.push_back("test1.txt");
	result.push_back("test7.txt");
	BOOST_REQUIRE(finder.inquiry(8, 5) == result);
}

BOOST_AUTO_TEST_CASE(test9) {
	result.clear();
	result.push_back("test10.txt");
	result.push_back("test4.txt");
	result.push_back("test11.txt");
	result.push_back("test6.txt");
	result.push_back("test9.txt");
	result.push_back("test3.txt");
	result.push_back("test7.txt");
	result.push_back("test5.txt");
	result.push_back("test8.txt");
	result.push_back("test2.txt");
	BOOST_REQUIRE(finder.inquiry(10, 0) == result);
}

BOOST_AUTO_TEST_CASE(test10) {
	result.clear();
	result.push_back("test6.txt");
	result.push_back("test4.txt");
	result.push_back("test11.txt");
	BOOST_REQUIRE(finder.inquiry(3, 9) == result);
}




//tests based on "The War and Peace"
BOOST_AUTO_TEST_CASE(test11) {
	result.clear();
	result.push_back("Part_two.txt");
	BOOST_REQUIRE(finder.inquiry(1, 0) == result);
}

BOOST_AUTO_TEST_CASE(test12) {
	result.clear();
	result.push_back("Part_two.txt");
	result.push_back("Part_four.txt");
	result.push_back("Part_three.txt");
	BOOST_REQUIRE(finder.inquiry(3, 0) == result);
}

BOOST_AUTO_TEST_CASE(test13) {
	result.clear();
	result.push_back("Part_five.txt");
	result.push_back("Part_four.txt");
	result.push_back("Part_three.txt");
	result.push_back("Part_one.txt");
	BOOST_REQUIRE(finder.inquiry(4, 4) == result);
}

BOOST_AUTO_TEST_CASE(test14) {
	result.clear();
	result.push_back("Part_two.txt");
	BOOST_REQUIRE(finder.inquiry(1, 3) == result);
}

BOOST_AUTO_TEST_CASE(test15) {
	result.clear();
	result.push_back("Part_two.txt");
	result.push_back("Part_five.txt");
	result.push_back("Part_four.txt");
	BOOST_REQUIRE(finder.inquiry(3, 2) == result);
}



//test with invalid arguments
BOOST_AUTO_TEST_CASE(test16) {
	BOOST_CHECK_THROW(finder.inquiry(-1, 2), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test17) {
	BOOST_CHECK_THROW(finder.inquiry(-100, 2), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test18) {
	BOOST_CHECK_THROW(finder.inquiry(1, 10), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test19) {
	BOOST_CHECK_THROW(finder.inquiry(100, 1), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test20) {
	BOOST_CHECK_THROW(finder.inquiry(1, 10), std::invalid_argument);
}*/

