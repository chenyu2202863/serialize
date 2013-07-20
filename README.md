serialize
=========
Copyright (c) 2011, Chen Yu. All rights reserved.

Use of this source code is governed by a BSD-style license that can be found in the License file.

序列化库,支持C++基础类型及多种stl容器。包括输出为binary及text两种方式，方便扩展。

使用方式:
  char buf[1024] = { 0 };
  serialize::mem_serialize os(buf);


	{
		std::pair<int, std::string> v{ 1, "1" };
		os << v;
		std::pair<int, std::string> v2;
		os >> v2;
	}


	{
		std::tuple<int, std::pair<int, std::string>> v{ 1, {1, "1"} };
		os << v;
		std::tuple<int, std::pair<int, std::string>> v2;
		os >> v2;
	}


	std::vector<int> a1(10);
	std::fill(a1.begin(), a1.end(), 10);
	os << a1;
	std::vector<int> b1;
	os >> b1;
	assert(std::equal(a1.begin(), a1.end(), b1.begin()));


	std::list<std::string> a2;
	a2.push_back("chenyu");
	a2.push_back("test");
	os << a2;
	std::list<std::string> b2;
	os >> b2;
	assert(std::equal(a2.begin(), a2.begin(), b2.begin()));


	std::deque<std::wstring> a3;
	a3.push_back(L"test1");
	a3.push_back(L"test2");
	os << a3;
	std::deque<std::wstring> b3;
	os >> b3;
	assert(std::equal(a3.begin(), a3.begin(), b3.begin()));


	std::set<long> a4;
	a4.insert(1L);
	a4.insert(10L);
	a4.insert(100L);
	os << a4;
	std::set<long> b4;
	os >> b4;
	assert(std::equal(a4.begin(), a4.begin(), b4.begin()));


	std::map<std::string, long> a5;
	a5.insert(std::make_pair("123", 1L));
	a5.insert(std::make_pair("1234", 10L));
	a5.insert(std::make_pair("12345", 100L));
	os << a5;
	std::map<std::string, long> b5;
	os >> b5;
	assert(std::equal(a5.begin(), a5.begin(), b5.begin()));




	std::multiset<long> a6;
	a6.insert(1L);
	a6.insert(10L);
	a6.insert(10L);
	a6.insert(10L);
	a6.insert(100L);
	a6.insert(100L);
	os << a6;
	std::set<long> b6;
	os >> b6;
	assert(std::equal(a6.begin(), a6.begin(), b6.begin()));




	std::multimap<std::string, long> a7;
	a7.insert(std::make_pair("123", 1L));
	a7.insert(std::make_pair("123", 1L));
	a7.insert(std::make_pair("1234", 10L));
	a7.insert(std::make_pair("1234", 100L));
	a7.insert(std::make_pair("1234", 1000L));
	a7.insert(std::make_pair("12345", 100L));
	os << a7;
	std::multimap<std::string, long> b7;
	os >> b7;
	assert(std::equal(a7.begin(), a7.begin(), b7.begin()));


	typedef std::map<std::pair<std::string, long>, std::vector<std::wstring>> VecMap;
	VecMap a8;
	std::vector<std::wstring> vec;
	vec.push_back(L"Test");
	vec.push_back(L"asd");
	a8.insert(std::make_pair(std::make_pair("10", 10), vec));
	a8.insert(std::make_pair(std::make_pair("11", 101), vec));
	a8.insert(std::make_pair(std::make_pair("11", 1011), vec));
	os << a8;
	VecMap b8;
	os >> b8;
	assert(std::equal(a8.begin(), a8.begin(), b8.begin()));
