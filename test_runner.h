#pragma once
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

template <typename Collection>
string Join(const Collection& c, char d);

template <typename First, typename Second>
ostream& operator << (ostream& out, const pair<First, Second>& p);

template <typename T>
ostream& operator << (ostream& out, const set<T>& vi);

template <typename Key, typename Value>
ostream& operator << (ostream& out, const map<Key, Value>& m);

template<typename T, typename U>
void AssertEqual(const T& t, const U& u, const string& hint);

void Assert(bool b , const string& hint);

class TestRunner{
public:
	template<typename Func>
	void RunTest(const Func& func, const string& func_name);
	~TestRunner();
private:
	int count_fails = 0;
};

template <typename Collection>
string Join(const Collection& c, char d) {
	  stringstream ss;
	  bool first = true;
	  for (const auto& i : c) {
		if (!first) {
		  ss << d;
		}
		first = false;
		ss << i;
	  }
	  return ss.str();
}

template <typename First, typename Second>
ostream& operator << (ostream& out, const pair<First, Second>& p) {
	return out << p.first << ' ' << p.second;
}
template <typename T>
ostream& operator << (ostream& out, const set<T>& vi) {
  return out << '[' << Join(vi, ' ') << ']';
}

template <typename T>
ostream& operator << (ostream& out, const vector<T>& vi) {
  return out << '[' << Join(vi, ',') << ']';
}

template <typename Key, typename Value>
ostream& operator << (ostream& out, const map<Key, Value>& m) {
	return out << '{' << Join(m, ',') << '}';
}
template<typename T, typename U>
void AssertEqual(const T& t, const U& u, const string& hint){
	if(t != u){
		ostringstream os;
		os << "Assertion failed:" << t << " != " << u << " Hint: " << hint;
		throw runtime_error(os.str());
	}
}

template<typename Func>
void TestRunner::RunTest(const Func& func, const string& func_name){
	try{
		func();
		cerr << func_name << " is OK" << endl;
	}
	catch(runtime_error& e){
		cerr << func_name << " is failed: " << e.what() << endl;
		count_fails++;
	}
}

#define RUN_TEST(tr, func) tr.RunTest(func, #func)
#define ASSERT_EQUAL(x, y) {  		                                          \
		ostringstream os; 			                                          \
		os << #x << " != " << #y << ", " <<__FILE__ << ":" << __LINE__;       \
		AssertEqual(x, y, os.str());                                          \
}

#define ASSERT(x) {  		                                                  \
		ostringstream os; 			                                          \
		os << #x << " is false, " <<__FILE__ << ":" << __LINE__;              \
		Assert(x, os.str());                                                  \
}
