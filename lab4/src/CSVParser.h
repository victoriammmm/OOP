#pragma once
#include <istream>
#include <tuple>
#include <vector>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <utility>

//exception
class CsvReadException final : public std::runtime_error {
public:
    explicit CsvReadException(const std::string& msg): std::runtime_error(msg) {}
};

inline void raiseParseError(std::size_t row, std::size_t col, const std::string& msg) {
    std::ostringstream out;
    out << "Parsing error in row " << row << ", column " << col << ": " << msg;
    throw CsvReadException(out.str());
}

//tuple builder
template<typename T>
T parseCell(const std::string& cell, std::size_t row, std::size_t col) {
    if (cell.empty()) {
        return T{};
    }

    T value{};
    std::stringstream ss(cell);
    ss >> value;
    if (ss.fail() || !ss.eof()) {
        raiseParseError(row, col, "Cannot convert \"" + cell + "\"");
    }
    return value;
}

template<>
inline std::string parseCell<std::string>(const std::string& cell, std::size_t row, std::size_t col) {
    return cell;
}

//config
struct CsvFormat {
    char columnSep;
    char rowSep;
    char quote;

    CsvFormat(): columnSep(','), rowSep('\n'), quote('"') {}

    explicit CsvFormat(char columnDelimiter, char rowDelimiter = '\n', char escapeChar = '"')
        :columnSep(columnDelimiter),rowSep(rowDelimiter),quote(escapeChar) {}
};

//CSV parser
template<typename... Types>
class CsvReader {
    std::istream& source;
    CsvFormat fmt;
    std::size_t currentRow = 0;
    bool eofReached = false; //end flag

    std::string readRawLine() {
        std::string line;
        if (fmt.rowSep == '\n') {
            if (!std::getline(source, line)) {
                eofReached = true;
            }
        }
        else {
            char current;
            while (source.get(current) && current != fmt.rowSep) {
                line.push_back(current);
            }
            if (!source && line.empty()) {
                eofReached = true;
            }
        }
        return line;
    }

    std::vector<std::string> splitLine(const std::string& line) {
        std::vector<std::string> cells;
        std::string buffer;
        bool quoted = false;//inside quote flag
        std::size_t col = 1;
        bool escapedQuote = false;//double quote flag

        for (std::size_t i = 0; i < line.size(); ++i) {
            char current = line[i];

            if (current == fmt.quote) {
                if (escapedQuote) {
                    buffer += fmt.quote;
                    escapedQuote = false;
                }
                else if (quoted) {
                    if (i + 1 < line.size() && line[i + 1] == fmt.quote) {//next char is quote
                        escapedQuote = true;
                    }
                    else {
                        quoted = false;
                    }
                }
                else {
                    if (!buffer.empty()) {
                        raiseParseError(currentRow, col, "quote inside unquoted cell");
                    }
                    quoted = true;
                }
            }
            else if (current == fmt.columnSep && !quoted) {
                cells.push_back(buffer);
                buffer.clear();
                ++col;
                escapedQuote = false;
            }
            else {
                if (escapedQuote) {
                    raiseParseError(currentRow, col, "extra characters after quote");
                }
                buffer.push_back(current);
                escapedQuote = false;
            }
        }

        if (escapedQuote) {
            raiseParseError(currentRow, col, "extra characters after quote");
        }
        if (quoted) {
            raiseParseError(currentRow, col, "unclosed quoted field");
        }

        cells.push_back(buffer);
        return cells;
    }

    template<std::size_t... Is>
    std::tuple<Types...> buildTuple(const std::vector<std::string>& cells, std::index_sequence<Is...>) {
        return std::make_tuple(parseCell<typename std::tuple_element<Is, std::tuple<Types...>>::type>(//type of tuple element by index Is
            cells[Is], currentRow, Is + 1)...);//for every cell
    }

    std::tuple<Types...> parseRow() {
        std::string raw = readRawLine();
        if (raw.empty() && eofReached)//end?
            throw CsvReadException("EOF");

        ++currentRow;
        auto cols = splitLine(raw);
        if (cols.size() != sizeof...(Types)) {//number of columns
            raiseParseError(currentRow, cols.size(), "expected " + std::to_string(sizeof...(Types)) +
                           " columns, got " + std::to_string(cols.size()));
        }
        return buildTuple(cols, std::index_sequence_for<Types...>{});
    }

public:
    //constructors
    explicit CsvReader(std::istream& in, std::size_t skip = 0, char columnDelimiter = ',',
    char rowDelimiter = '\n', char escapeChar = '"'):source(in), fmt(columnDelimiter, rowDelimiter, escapeChar){
        for (std::size_t i = 0; i < skip; ++i) {
            readRawLine();
        }
    }
    CsvReader(std::istream& in, const CsvFormat& format, std::size_t skip = 0): source(in), fmt(format){
        for (std::size_t i = 0; i < skip; ++i) {
            readRawLine();
        }
    }

    class InputIterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::tuple<Types...>;
        using difference_type = std::ptrdiff_t;
        using pointer = const std::tuple<Types...>*;
        using reference = const std::tuple<Types...>&;

        InputIterator() : parser(nullptr), finished(true) {}

        explicit InputIterator(CsvReader* rdr, bool end = false): parser(rdr), finished(end) {
            if (!finished) {
                ++(*this);
            }
        }

        InputIterator& operator++() {
            if (finished) {
                return *this;
            }
            try {
                current = parser->parseRow();
            }
            catch (const CsvReadException& e) {
                if (std::string(e.what()) == "EOF") {
                    finished = true;
                }
                else {
                    throw;
                }
            }
            return *this;
        }

        bool operator!=(const InputIterator& other) const {
            return finished != other.finished;
        }

        bool operator==(const InputIterator& other) const {
            return finished == other.finished;
        }

        const std::tuple<Types...>& operator*() const {
            return current;
        }
    private:
        CsvReader* parser;
        std::tuple<Types...> current;
        bool finished;
    };

    InputIterator begin() {
        return InputIterator(this);
    }

    InputIterator end() {
        return InputIterator(nullptr, true);
    }
};