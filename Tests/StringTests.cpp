#include "Util.h"

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
	SUBCASE("A") {
		a = "A";
	}
	SUBCASE("�") {
		a = u8"�";
	}

	CHECK(a.begin() == a.rbegin());
	CHECK(a.begin() + 1 == a.end());
	CHECK(a.begin() == a.end() - 1);
	CHECK(a.rbegin() + 1 == a.rend());
	CHECK(a.rbegin() == a.rend() - 1);
}

TEST_CASE("Multiplication") {
	String a; String sep;
	SUBCASE_PR(
		SUBCASE_P(sep, "", ", "),
		a, "A", u8"�"
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

}
