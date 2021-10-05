#include "test_runner.h"
#include <memory>
#include <algorithm>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate() {
	if (free.empty()) {
      free.push(make_unique<T>());
	}
	auto ptr = std::move(free.front());
	auto ret = ptr.get();
	free.pop();
	allocated.insert(std::move(ptr));
	return ret;
  }
  T* TryAllocate() {
    if (free.empty()) {
	  return nullptr;
	}
	return Allocate();
  }

  void Deallocate(T* object) {
	auto it = allocated.find(object);
    if (it == allocated.end()) {
	  throw invalid_argument("");
	}
    free.push(move(allocated.extract(it).value()));
  }

private:

  struct Compare {
	using is_transparent = void;
	bool operator()(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs) const {
	  return lhs < rhs;
	}
	bool operator()(const unique_ptr<T>& lhs, const T* rhs) const {
	  return less<const T*>()(lhs.get(), rhs);
	}
	bool operator()(const T* lhs, const unique_ptr<T>& rhs) const {
	  return less<const T*>()(lhs, rhs.get());
	}
  };
  queue<unique_ptr<T>> free;
  set<unique_ptr<T>, Compare> allocated;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
