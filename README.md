serialize
=========
Copyright (c) 2011, Chen Yu. All rights reserved.

Use of this source code is governed by a BSD-style license that can be found in the License file.

序列化库,支持C++基础类型及多种stl容器。包括输出为binary及text两种方式，方便扩展。

特点:
1. 接口简单，易于集成，方便维护
2. 基于Policy-based设计模式，默认提供binary及text两种格式化方式
3. 采用C++流式方式方便易用
4. 扩展性强（针对用户自定义类型，输出目标）
5. 编译期对用户自定义类型侦错，及in、out误用侦错

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
	


