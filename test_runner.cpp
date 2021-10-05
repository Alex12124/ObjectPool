#include "test_runner.h"

void Assert(bool b , const string& hint){
	AssertEqual(b, true, hint);
}

TestRunner::~TestRunner(){
	if(count_fails > 0){
		cerr << count_fails << " tests failed. Terminate";
		exit(1);
	}
}



