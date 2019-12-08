#include <pe/Defines.hpp>
#include <catch.hpp>

#include <pe/core/storage/Queue.hpp>

TEST_CASE("Queue constructor and basic operations tests", "[Queue]")
{
	::pe::core::storage::Queue<int> q;
	::pe::core::storage::Queue<int> r(q);

	REQUIRE(q.isEmpty() == true);
	REQUIRE(r.isEmpty() == true);

	q.pushBack(5);
	REQUIRE(q.isEmpty() == false);
	::pe::core::storage::Queue<int> s(q);

	REQUIRE(r.isEmpty() == true);
	REQUIRE(q.isEmpty() == false);
	REQUIRE(s.isEmpty() == false);
	REQUIRE(q.back() == 5);
	REQUIRE(s.back() == 5);
	REQUIRE(q.front() == 5);
	REQUIRE(s.front() == 5);

	q.pushBack(8);
	::pe::core::storage::Queue<int> t(q);

	REQUIRE(t.isEmpty() == false);
	REQUIRE(q.back() == 8);
	REQUIRE(t.back() == 8);
	REQUIRE(s.back() == 5);
	REQUIRE(q.front() == 5);
	REQUIRE(t.front() == 5);
	REQUIRE(s.front() == 5);

	::pe::core::storage::Queue<int> u(std::move(q));
	REQUIRE(u.isEmpty() == false);
	REQUIRE(q.isEmpty() == true);
}

TEST_CASE("Queue assignment operator tests", "[Queue]")
{
	::pe::core::storage::Queue<int> q;
	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	q.pushBack(5);

	::pe::core::storage::Queue<int> u = q;
	REQUIRE(u.back() == 5);
	REQUIRE(u.front() == 1);

	::pe::core::storage::Queue<int> v = std::move(u);
	REQUIRE(v.back() == 5);
	REQUIRE(v.front() == 1);
	REQUIRE(u.isEmpty() == true);
	v.clear();
	REQUIRE(v.isEmpty() == true);
}

TEST_CASE("Queue initializer list tests", "[Queue]")
{
	::pe::core::storage::Queue<int> x = { 1, 2, 3 };
	REQUIRE(x.getSize() == 3);

	::pe::core::storage::Queue<int> y { 1, 2, 3 };
	REQUIRE(y.getSize() == 3);
	
	::pe::core::storage::Queue<int> z;
	z = std::move(x);
	REQUIRE(z.getSize() == 3);
	REQUIRE(x.isEmpty() == true);

	int i1 = z.popBack();
	REQUIRE(i1 == 3);
	int i2 = z.popFront();
	REQUIRE(i2 == 1);
	int i3 = z.popFront();
	REQUIRE(i3 == 2);
	REQUIRE(z.isEmpty() == true);
}

TEST_CASE("Queue tests for equality operators", "[Queue")
{
	bool eval = false;
	::pe::core::storage::Queue<int> q;
	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	
	::pe::core::storage::Queue<int> r;
	r.pushBack(1);
	r.pushBack(2);
	r.pushBack(3);
	r.pushBack(4);

	eval = (q == r);
	REQUIRE(eval == true);

	r.pushBack(5);
	eval = (q != r);
	REQUIRE(eval == true);

	q.pushBack(5);
	eval = (q == r);
	REQUIRE(eval == true);

	q.pushFront(6);
	r.pushBack(6);
	eval = (q == r);
	REQUIRE(eval == false);
}

TEST_CASE("Queue tests for STL forward iterators", "[Queue]")
{
	::pe::core::storage::Queue<int> q;

	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	q.pushBack(5);

	int idx = 0;
	for(auto& e : q)
	{
		++idx;
		REQUIRE(e == idx);
		if(idx == 2)
			e = 20;
	}

	REQUIRE(idx == q.getSize());

	idx = 0;
	for(auto e : q)
	{
		++idx;
		e = -idx;
		auto it = std::find(q.begin(), q.end(), e);
		REQUIRE(it == q.end());
	}

	auto it = std::find(q.begin(), q.end(), 20);
	REQUIRE(it != q.end());
	*it = 30;
	auto it2 = std::find(q.begin(), q.end(), 30);
	REQUIRE(it2 != q.end());
	*it2 = 2;
	q.popBack();
	q.popBack();
	q.popFront();
	q.popFront();
	for(auto e : q)
	{
		REQUIRE(e == 3);
	}
	q.pushBack(4);
	q.pushBack(5);
	q.pushFront(2);
	q.pushFront(1);
	q.pushBack(6);
	q.pushBack(7);
	q.pushBack(8);
	idx = 0;
	for(auto& e : q)
	{
		++idx;
		REQUIRE(e == idx);
	}

	idx = static_cast<int>(q.getSize());
	for(auto it = std::prev(q.end()); it != q.begin(); --it)
	{
		REQUIRE(*it == idx);
		--idx;
	}
	REQUIRE(idx == 1);
}

TEST_CASE("Queue tests for STL const forward iterators", "[Queue]")
{
	::pe::core::storage::Queue<int> q;

	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	q.pushBack(5);
	
	int idx = 1;
	for(auto it = q.cbegin(); it != q.cend(); ++it)
	{
		REQUIRE(*it == idx);
		++idx;
	}

	q.clear();
	idx = 0;
	for(auto it = q.cbegin(); it != q.cend(); ++it)
	{
		++idx;
	}
	REQUIRE(idx == 0);

	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	q.pushBack(5);

	idx = static_cast<int>(q.getSize());
	for(auto it = std::prev(q.cend()); it != q.cbegin(); --it)
	{
		REQUIRE(*it == idx);
		--idx;
	}
	REQUIRE(idx == 1);
}

TEST_CASE("Queue tests for STL reverse iterators", "[Queue]")
{
	::pe::core::storage::Queue<int> q;

	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	q.pushBack(5);

	size_t idx = q.getSize();
	
	for(auto it = q.rbegin(); it != q.rend(); ++it)
	{
		REQUIRE(*it == idx);
		--idx;
	}
	REQUIRE(idx == 0);
	q.popFront();
	q.popFront();
	q.popFront();
	q.popBack();
	for(auto it = q.rbegin(); it != q.rend(); ++it)
		REQUIRE(*it == 4);

	q.popBack();
	REQUIRE(q.isEmpty() == true);

	q.clear();
	idx = 0;
	for(auto it = q.rbegin(); it != q.rend(); ++it)
	{
		--idx;
	}
	REQUIRE(idx == 0);

	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	q.pushBack(5);

	idx = 1;
	for(auto it = std::prev(q.rend()); it != q.rbegin(); --it)
	{
		REQUIRE(*it == idx);
		++idx;
	}
	REQUIRE(idx == q.getSize());
}

TEST_CASE("Queue tests for STL const reverse iterators", "[Queue]")
{
	::pe::core::storage::Queue<int> q;

	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	q.pushBack(5);
	
	int idx = static_cast<int>(q.getSize());
	for(auto it = q.crbegin(); it != q.crend(); ++it)
	{
		REQUIRE(*it == idx);
		--idx;
	}

	q.clear();
	idx = 0;
	for(auto it = q.crbegin(); it != q.crend(); ++it)
	{
		--idx;
	}
	REQUIRE(idx == 0);

	q.pushBack(1);
	idx = static_cast<int>(q.getSize());
	for(auto it = q.crbegin(); it != q.crend(); ++it)
	{
		--idx;
	}
	REQUIRE(idx == 0);
	q.clear();

	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	q.pushBack(5);

	idx = 1;
	for(auto it = std::prev(q.crend()); it != q.crbegin(); --it)
	{
		REQUIRE(*it == idx);
		++idx;
	}
	REQUIRE(idx == q.getSize());
}

TEST_CASE("Queue test for wrapping coherency", "[Queue]")
{
	::pe::core::storage::Queue<int> q;
	q.reserve(6);
	q.pushBack(5);
	q.pushBack(4);
	q.pushBack(3);
	q.pushBack(2);
	q.pushBack(1);
	q.pushBack(0);
	REQUIRE(q.getSize() == 6);

	q.popFront();
	q.popFront();

	REQUIRE(q.getSize() == 4);
	REQUIRE(q.front() == 3);
	REQUIRE(q.back() == 0);

	q.pushBack(10);
	REQUIRE(q.getSize() == 5);
	REQUIRE(q.front() == 3);
	REQUIRE(q.back() == 10);

	int i = 0;
	for(auto it = q.begin(); it != q.end(); ++it)
	{
		UNUSED(it);
		++i;
	}
	REQUIRE(i == 5);
	
	i = 0;
	for(auto& e : q)
	{
		UNUSED(e);
		++i;
	}
	REQUIRE(i == 5);

	i = 0;
	for(auto it = q.rbegin(); it != q.rend(); ++it)
	{
		UNUSED(it);
		++i;
	}
	REQUIRE(i == 5);
}

TEST_CASE("Queue tests (with BaseObject)", "[Queue]")
{

	class Test : ::pe::core::BaseObject<>
	{
	public:
		Test() {}
		~Test() {}
	};

	::pe::core::storage::Queue<Test> q;
	::pe::core::storage::Queue<Test> r(q);

	REQUIRE(q.isEmpty() == true);
	REQUIRE(r.isEmpty() == true);

	Test a = Test();
	q.pushBack(a);
	q.pushBack(a);
	q.pushBack(a);
	q.pushBack(Test());
	q.popBack();
}