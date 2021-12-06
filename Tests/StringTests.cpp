#include "Util.h"

#include <iostream>

#include <PGE/String/String.h>
#include <PGE/Exception/Exception.h>

using namespace PGE;

TEST_SUITE("Strings") {

TEST_CASE("Very basic tests") {
	CHECK(String().byteLength() == 0);
	CHECK(String().length() == 0);

	CHECK(String() == "");
	CHECK(String().equalsIgnoreCase(""));

	String empty;
	CHECK(empty.begin() == empty.end());
	CHECK(empty.rbegin() == empty.rend());

	CHECK(empty.begin() != empty.rbegin());
	CHECK(empty.end() != empty.rend());
	CHECK(empty.rbegin() != empty.end());
	CHECK(empty.begin() != empty.rend());
}

TEST_CASE("Basic iterator tests") {
	String a;
	SUBCASE_PARAMETERIZE((a, "A", u8"�"));

	CHECK(a.begin() == a.rbegin());
	CHECK(a.begin() + 1 == a.end());
	CHECK(a.begin() == a.end() - 1);
	CHECK(a.rbegin() + 1 == a.rend());
	CHECK(a.rbegin() == a.rend() - 1);
}

TEST_CASE("Multiplication") {
	String a; String sep;
	SUBCASE_PARAMETERIZE(
		(a, "A", u8"�"),
		(sep, "", ", ")
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
