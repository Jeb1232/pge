#include <fstream>
#include <iostream>

#include <String/String.h>
#include <Exception/Exception.h>
#include <Misc/FilePath.h>

using namespace PGE;

#define PGE_UNICODE_ASSERT_FILE(stream) PGE_ASSERT(stream.good(), "Stream is not good!")

static constexpr int READ_BUFFER_SIZE = 256;
static constexpr const char* INDENT = "    ";
static constexpr const char* TAIL = "};";
static const String MAP_TYPE = "std::unordered_map<wchar, wchar>";

struct Mapping {
    String from;
    String to;
};

static String rawToChar(const String& str) {
    return "u'\\u" + str + "'";
}

static String entriesToPair(const String& a, const String& b) {
    return "{ " + rawToChar(a) + ", " + rawToChar(b) + " }, ";
}

static String toMapHead(const String& identifier) {
    return MAP_TYPE + " Char::" + identifier + " = " + MAP_TYPE + " {";
}

static std::ostream& operator<<(std::ostream& stream, const std::vector<Mapping>& mappings) {
    for (const Mapping& mapping : mappings) {
        stream << INDENT << entriesToPair(mapping.from, mapping.to) << std::endl;
    }
    return stream;
}

int main() {
    FilePath checkFile = FilePath::fromStr(".check");
    if (checkFile.exists()) {
        std::cout << "Check file already exists!" << std::endl;
        return 0;
    }

    std::ofstream out;
    out.open("../../Src/String/Unicode.cpp");
    PGE_UNICODE_ASSERT_FILE(out);

    out << "// AUTOGENERATED FILE" << std::endl;
    out << "// DO NOT EDIT" << std::endl;
    out << std::endl;
    out << "#include <String/Char.h>" << std::endl;
    out << std::endl;
    out << "using namespace PGE;" << std::endl;
    out << std::endl;

    char buffer[READ_BUFFER_SIZE];
    std::ifstream in;
    in.open("CaseFolding.txt");
    PGE_UNICODE_ASSERT_FILE(in);

    out << toMapHead("folding") << std::endl;

    while (!in.eof()) {
        in.getline(buffer, READ_BUFFER_SIZE);
        String line = buffer;
        if (line.length() == 0 || line.charAt(0) == L'#') { continue; }
        std::vector<String> params = line.split(";", true);
        if (params[1].trim() != "C") { continue; }
        params[0] = params[0].trim();
        // We reached UTF-32 range!
        if (params[0].length() == 5) { break; }
        out << INDENT << entriesToPair(params[0], params[2].trim()) << std::endl;
    }

    in.close();

    out << TAIL << std::endl;
    out << std::endl;

    std::vector<Mapping> up;
    std::vector<Mapping> down;

    in.open("UnicodeData.txt");
    PGE_UNICODE_ASSERT_FILE(in);

    while (!in.eof()) {
        in.getline(buffer, READ_BUFFER_SIZE);
        String line = buffer;
        std::vector<String> params = line.split(";", false);
        // We reached UTF-32 range!
        if (params[0].length() == 5) { break; }
        if (params[12].length() != 0) {
            up.push_back({ params[0], params[12] });
        }
        if (params[13].length() != 0) {
            down.push_back({ params[0], params[13] });
        }
    }

    in.close();

    out << toMapHead("up") << std::endl;
    out << up << std::endl;
    out << TAIL << std::endl;

    out << std::endl;

    out << toMapHead("down") << std::endl;
    out << down << std::endl;
    out << TAIL << std::endl;

    out.close();

    out.open(checkFile.cstr());
    PGE_UNICODE_ASSERT_FILE(out);
    out << u8"\u0D9E\u0073\u0075\u0073";
    out.close();

    std::cout << "Generated Unicode code!" << std::endl;
    return 0;
}
