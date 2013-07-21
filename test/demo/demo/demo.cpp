// demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <serialize.hpp>




void test1( )
{
	//set_test_name("basic serialize");

	char buf[1024] = { 0 };
	serialize::mem_serialize os(buf);

	int b1 = 1;
	long b2 = 1L;
	long long b3 = 1LL;
	double b4 = 1.111;
	char b5 = 'a';
	char b6[] = "bc";
	wchar_t b7 = L'c';
	wchar_t b8[32] = L"Yu";
	os << b1;
	os << b2;
	os << b3;
	os << b4;
	os << b5;
	os << b6;
	os << b7;
	os << b8;

	std::string b9("123");
	std::wstring b10(L"233");
	os << b9;
	os << b10;

	char *b11 = "asd";
	wchar_t *b12 = L"bbb";
	os << b11 << b12;

	serialize::mem_serialize in(buf);
	int a1 = 0;
	long a2 = 0L;
	long long a3 = 0LL;
	double a4 = 0.0;
	char a5 = 0;
	char a6[32] = { 0 };
	wchar_t a7 = L'';
	wchar_t a8[32] = { 0 };

	std::string a9;
	std::wstring a10;

	in >> a1;
	in >> a2;
	in >> a3;
	in >> a4;
	in >> a5;
	in >> a6;
	in >> a7;
	in >> a8;
	in >> a9;
	in >> a10;

	char tmp1[4] = { 0 };
	wchar_t tmp2[4] = { 0 };
	char *a11 = tmp1;
	wchar_t *a12 = tmp2;
	in >> a11 >> a12;


	assert(b1 == a1);
	assert(b2 == a2);
	assert(b3 == a3);
	assert(b4 == a4);
	assert(b5 == a5);
	assert(strcmp(b6, a6) == 0);
	assert(b7 == a7);
	assert(wcscmp(b8, a8) == 0);
	assert(b9 == a9);
	assert(b10 == a10);
	assert(strcmp(b11, a11) == 0);
	assert(wcscmp(b12, a12) == 0);
}




void test2( )
{
	//set_test_name("unicode");

	wchar_t buf[1024] = { 0 };
	serialize::mem_wserialize os(buf);

	int b1 = 1;
	long b2 = 1L;
	long long b3 = 1LL;
	double b4 = 1.111;
	char b5 = 'a';
	char b6[] = "bc";
	wchar_t b7 = L'c';
	wchar_t b8[] = L"Yu";
	os << b1;
	os << b2;
	os << b3;
	os << b4;
	os << b5;
	os << b6;
	os << b7;
	os << b8;

	std::string b9("233");
	std::wstring b10(L"23");
	os << b9;
	os << b10;

	char *b11 = "asd";
	wchar_t *b12 = L"bbb";
	os << b11 << b12;

	serialize::mem_wserialize in(buf);
	int a1 = 0;
	long a2 = 0L;
	long long a3 = 0LL;
	double a4 = 0.0;
	char a5 = 0;
	char a6[1024] = { 0 };
	wchar_t a7 = L'';
	wchar_t a8[1024] = { 0 };

	std::string a9;
	std::wstring a10;

	in >> a1;
	in >> a2;
	in >> a3;
	in >> a4;
	in >> a5;
	in >> a6;
	in >> a7;
	in >> a8;
	in >> a9;
	in >> a10;

	char tmp1[4] = { 0 };
	wchar_t tmp2[4] = { 0 };
	char *a11 = tmp1;
	wchar_t *a12 = tmp2;
	in >> a11 >> a12;


	assert(b1 == a1);
	assert(b2 == a2);
	assert(b3 == a3);
	assert(b4 == a4);
	assert(b5 == a5);
	assert(strcmp(b6, a6) == 0);
	assert(b7 == a7);
	assert(wcscmp(b8, a8) == 0);
	assert(b9 == a9);
	assert(b10 == a10);
	assert(strcmp(b11, a11) == 0);
	assert(wcscmp(b12, a12) == 0);
}




void test3( )
{
	//set_test_name("array");


	char buf[1024] = { 0 };
	serialize::mem_serialize os(buf);

	char a1[] = "1234";
	unsigned char a2[] = "QWER";
	short a3[] = { 1, 2, 3, 4 };
	int a4[] = { 5, 6, 7, 8 };
	long a5[] = { 10, 10, 10, 10 };
	unsigned long long a6[] = { 100LL, 100LL, 100LL };

	os << a1;
	os << a2;
	os << a3 << a4 << a5 << a6;

	char b1[5] = { 0 };
	unsigned char b2[5] = { 0 };
	short b3[4] = { 0 };
	int b4[4] = { 0 };
	long b5[4] = { 0 };
	unsigned long long b6[3] = { 0 };
	os >> b1 >> b2 >> b3 >> b4 >> b5 >> b6;

	assert(strcmp(b1, a1) == 0);
	assert(memcmp(b2, a2, _countof(a2)) == 0);
	assert(memcmp(b3, a3, _countof(a3)) == 0);
	assert(memcmp(b4, a4, _countof(a4)) == 0);
	assert(memcmp(b5, a5, _countof(a5)) == 0);
	assert(memcmp(b6, a6, _countof(a6)) == 0);
}


struct CustomType
{
	int age_;
	wchar_t class_[64];
	std::wstring name_;

	CustomType()
		: age_(0)
	{
		std::fill(class_, class_ + _countof(class_), 0);
	}
	CustomType(int age, const wchar_t * cla, const wchar_t * name)
		: age_(age)
		, name_(name)
	{
		wcscpy(class_, cla);
	}
};

bool operator==( const CustomType &lhs, const CustomType &rhs )
{
	return lhs.age_ == rhs.age_ &&
		wcscmp(lhs.class_, rhs.class_) == 0 &&
		lhs.name_ == rhs.name_;
}

serialize::mem_serialize &operator<<( serialize::mem_serialize &os, const CustomType &type )
{
	os << type.age_ << type.class_ << type.name_;
	return os;
}

serialize::mem_serialize &operator>>( serialize::mem_serialize &os, CustomType &type )
{
	os >> type.age_ >> type.class_ >> type.name_;
	return os;
}




void test4( )
{
	//set_test_name("custom type");

	char buf[1024];
	CustomType customType(10, _T("rrrr"), _T("aaaa"));
	serialize::mem_serialize os(buf);
	os << customType;

	CustomType dst1, dst2;
	os >> dst1;

	os << &customType;
	os >> dst2;

	CustomType arr[2] =
	{
		CustomType(10, _T("heihei"), _T("123")),
		CustomType(11, _T("hoho"), _T("323"))
	};
	os << arr;

	CustomType brr[2];
	os >> brr;

	assert(dst1 == customType);
	assert(dst2 == customType);

	for(std::uint32_t i = 0; i != _countof(arr); ++i)
	{
		assert(arr[i] == brr[i]);
	}
}




void test5( )
{
	//set_test_name("container");

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
}




void test6( )
{
	//set_test_name("buffer file");


	serialize::file_serialize file("test_file.dat");
	int a1 = 10;
	long a2 = 10L;
	char a3 = 'c';
	char a4[1024] = "chenyu";
	wchar_t a5[1024] = L"chenyu";

	file << a1 << a2 << a3 << a4 << a5;

	int b1 = 0;
	long b2 = 0L;
	char b3 = 0;
	char b4[1024] = { 0 };
	wchar_t b5[1024] = { 0 };

	file >> b1 >> b2 >> b3 >> b4 >> b5;

	assert(a1 == b1);
	assert(a2 == b2);
	assert(a3 == b3);
	assert(strcmp(a4, b4) == 0);
	assert(wcscmp(a5, b5) == 0);
}


struct undefine_info
{
	std::string name_;
	int age_;

	undefine_info()
		: age_(0)
	{}

	undefine_info(const std::string &name, int age)
		: name_(name)
		, age_(age)
	{}
};



void test7( )
{
	//set_test_name("don't custom struct");

	char buf[1024];
	undefine_info info("123", 12);
	serialize::mem_serialize os(buf);
	//os << info;

	std::map<int, std::vector<undefine_info>> infos;
	std::vector<undefine_info> tmp;
	tmp.push_back(undefine_info("baby", 1));
	tmp.push_back(undefine_info("baby2", 2));
	infos.insert(std::make_pair(1, tmp));

	//os << infos;

	std::map<int, std::vector<undefine_info>> infos2;
	//os >> infos2;
}


void test8()
{
	// text binary
	char buffer[1024] = {0};
	serialize::text_serialize os(buffer);

	os << "1 " << 123 << " test " << " 测试 " << std::vector<std::string>{"test1", "test2", "test3"};
}

void test9()
{
	char buffer[1024] = { 0 };
	
	{
		serialize::o_serialize os(buffer);
		os << 1;

		serialize::i_serialize in(buffer);
		int n = 0;
		in >> n;
	}

	{
		serialize::o_text_serialize os(buffer);
		os << __FUNCTION__ << ' ' << "test";

		//std::string name;
		//os >> name;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	test9();

	system("pause");
	return 0;
}

