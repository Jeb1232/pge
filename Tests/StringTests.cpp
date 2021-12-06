#include "Util.h"

#include <iostream>

#include <PGE/String/String.h>
#include <PGE/Exception/Exception.h>

using namespace PGE;

TEST_SUITE("Strings") {

static void checkLength(const String& str, int byteLength, int length) {
	CHECK(str.byteLength() == byteLength);
	CHECK(str.length() == length);
}

TEST_CASE("Lengths") {
	checkLength("", 0, 0);
	checkLength("A", 1, 1);
	checkLength(L"�", 2, 1);
	checkLength(u8"���", 6, 3);
	checkLength(L"�AA", 4, 3);
	checkLength(u8"A�A", 4, 3);
	checkLength(L"AA�", 4, 3);
	checkLength("Hello, this is a very long string, it is very long, yeah, very long, like very long.\n"
		u8"Ok, I'll also make it include special characters like � and � and idfk \u2764 (it's a heart)", 176, 172);
}

TEST_CASE("Basic iterator tests") {
	String a;
	SUBCASE_PARAMETERIZE((a, "A", u8"�", L"\u2764"));

	CHECK(a.begin() == a.rbegin());
	CHECK(a.begin() + 1 == a.end());
	CHECK(a.begin() == a.end() - 1);
	CHECK(a.rbegin() + 1 == a.rend());
	CHECK(a.rbegin() == a.rend() - 1);

	String e;
	CHECK(e.begin() == e.end());
	CHECK(e.rbegin() == e.rend());

	CHECK(e.begin() != e.rbegin());
	CHECK(e.end() != e.rend());
	CHECK(e.rbegin() != e.end());
	CHECK(e.begin() != e.rend());

	String o;
	CHECK(e.begin() != o.begin());
	CHECK(e.end() != o.end());
	CHECK(e.rbegin() != o.rbegin());
	CHECK(e.rend() != o.rend());
}

TEST_CASE("Multiplication") {
	String a; String sep;
	SUBCASE_PARAMETERIZE(
		(a, "A", u8"�", "ASDFG", u8"�AD�", u8"�aaa", u8"ooo�"),
		(sep, "", ", ", "A", u8"���")
	);

	CHECK(a.multiply(0) == String());
	CHECK(a.multiply(1) == a);
	
}

TEST_CASE("Multiplication explicit") {
	String a = "A";
	CHECK(a.multiply(5) == "AAAAA");
	CHECK(a.multiply(5, ", ") == "A, A, A, A, A");

	String u = u8"�";
	CHECK(u.multiply(5) == u8"�����");
	CHECK(u.multiply(5, ", ") == u8"�, �, �, �, �");
}

TEST_CASE("Reverse explicit") {
	CHECK(String().reverse() == "");
	String a = L"g�n�g";
	CHECK(a.reverse() == a);
	CHECK(String(L"�").reverse() == L"�");
	CHECK(String("reverse").reverse() == "esrever");
	CHECK(String(L"�BC").reverse() == L"CB�");
}

TEST_CASE("Split explicit") {
	String e;
	CHECK(e.split("", true).empty());
	CHECK(e.split("", false) == std::vector<String>(2, ""));
	CHECK(e.split("asd", false) == std::vector<String>{ "" });

	String a = "pulseyesgun";
	CHECK(a.split("yes", true) == std::vector<String>{ "pulse", "gun" });
	CHECK(a.split("pulse", false) == std::vector<String>{ "", "yesgun" });
	CHECK(a.split("pulse", true) == std::vector<String>{ "yesgun" });
	CHECK(a.split("gun", false) == std::vector<String>{ "pulseyes", "" });
	CHECK(a.split("gun", true) == std::vector<String>{ "pulseyes" });
	CHECK(a.split("pulseyesgun", false) == std::vector<String>(2));
	CHECK(a.split("pulseyesgun", true).empty());
	CHECK(a.split("", false).size() == a.length() + 2);
	CHECK(a.split("", true).size() == a.length());

	String u = L"�������";
	CHECK(u.split(u8"�", false) == std::vector<String>{ "", L"������" });
	CHECK(u.split(u8"�", false) == std::vector<String>{ L"���", L"���" });
	CHECK(u.split(u8"�", false) == std::vector<String>{ L"������", "" });
	CHECK(u.split("", false).size() == u.length() + 2);
	CHECK(u.split("", true).size() == u.length());

	CHECK(String(L"������").split(L"��", false) == std::vector<String>(4, ""));
	CHECK(String(L"�������").split(L"��", false) == std::vector<String>{ "", "", "", L"�" });
	CHECK(String(L"���").split(L"�", false) == std::vector<String>(4, ""));
}

TEST_CASE("Multiply split join same") {
	String a; String sep; int i;
	SUBCASE_PARAMETERIZE(
		(a, "A", L"���"),
		(sep, ",", "", "   ", "�"),
		(i, 10, 1, 0, 100)
	);

	a = a.multiply(i, sep);	
	CHECK(a == String::join(a.split(sep, true), sep));
}

}
