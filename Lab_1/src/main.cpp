#include <algorithm>
#include <cstdio>
#include <ios>
#include <iostream>
#include <array>
#include <limits>
#include <string>
#include <string_view>

//Cryptography lab turned into mini UTF encoding library project!

constexpr std::u32string_view g_ralph = U"AĂÂBCDEFGHIÎJKLMNOPQRSȘTȚUVWXYZ";
constexpr std::u32string_view g_ralph_lower = U"aăâbcdefghiîjklmnopqrsștțuvwxyz";
constexpr long long emptySpace{U' '};

std::u32string utf8_to_utf32(std::string_view utf8)
{
    std::size_t len {std::size(utf8)};
    std::u32string output{};
    std::size_t in_idx{};

    while(in_idx < len)
    {
        char32_t ch{};
        unsigned char b1{static_cast<unsigned char>(utf8[in_idx++])};
        if(b1 < 0x80) // 1 byte
        {
            ch = b1;
        }
        else if (((b1 & 0xE0) == 0xC0) && in_idx < len) // 2 bytes
        {
            unsigned char b2{static_cast<unsigned char>(utf8[in_idx++])};
                if ((b2 & 0xC0) != 0x80)
                {
                    std::cerr << "2byte char doesn't follow metadata convention fuck you";
                    std::exit(1);
                }
            ch = static_cast<char32_t>(((b1 & 0x1F) << 6) | (b2 & 0x3F));
            if ((ch < 0x0080) || (ch > 0x07FF))
            {
                std::cerr << "2byte char with value out of bounds";
                std::exit(2);
            }
        }
        else if (((b1 & 0xF0) == 0xE0) && in_idx + 1 < len) // 3 bytes
        {
            unsigned char b2{static_cast<unsigned char>(utf8[in_idx++])};
            unsigned char b3{static_cast<unsigned char>(utf8[in_idx++])};
                if( ((b2 & 0xC0) != 0x80) ||
                    ((b3 & 0xC0) != 0x80) )
                {
                    std::cerr << "3byte char doesn't follow metadata convention fuck you";
                    std::exit(1);
                }
            ch = static_cast<char32_t>  (   ((b1 & 0x0F) << 12)|
                                            ((b2 & 0x3F) << 6) |
                                            (b3 & 0x3F)     );

            if ((ch < 0x0800) || (ch > 0xFFFF) || ((ch >= 0xD800) && (ch <= 0xDFFF)))
            {
                std::cerr << "3byte char with value out of bounds";
                std::exit(2);
            }
        }
        else if (((b1 & 0xF8) == 0xF0) && in_idx + 2 < len) // 4 bytes
        {
            unsigned char b2{static_cast<unsigned char>(utf8[in_idx++])};
            unsigned char b3{static_cast<unsigned char>(utf8[in_idx++])};
            unsigned char b4{static_cast<unsigned char>(utf8[in_idx++])};
                if ( ((b2 & 0xC0) != 0x80) || 
                        ((b3 & 0xC0) != 0x80) ||
                        ((b4 & 0xC0) != 0x80) )
                {
                    std::cerr << "4byte char doesn't follow metadata convention fuck you";
                    std::exit(1);
                }
                
            ch = static_cast<char32_t>  (   ((b1 & 0x07) << 18)|
                                            ((b2 & 0x3F) << 12)|
                                            ((b3 & 0x3F) << 6) |
                                            (b4 & 0x3F)     );

            if ((ch < 0x10000) || (ch > 0x10FFFF))
            {
                std::cerr << "4byte char with value out of bounds";
                std::exit(2);
            }
        }
        else
        {
            std::cerr << "Brother how the fuck did you even get here...?";
            std::exit(1);
        }
        output += ch;
    }

    return output;
}
std::string utf32_to_utf8(std::u32string_view utf32)
{
    std::size_t len = utf32.length();
    std::string output{};
    std::size_t in_idx{};

    while(in_idx < len)
    {
        int nrBytes{};
        char32_t cur{utf32[in_idx++]};
        if (cur < 0x0080){
            nrBytes = 1;
        } else if (cur < 0x0800) {
            nrBytes = 2;
        } else if ((0xD800 <= cur) && (cur <= 0xDFFF)) {
            std::cout << "Character " << static_cast<int>(cur) << " part of UTF16 reserved list.\n";
            std::exit(1);
        } else if (cur < 0x10000) {
            nrBytes = 3;
        } else if (cur < 0x110000) {
            nrBytes = 4;
        } else {
            std::cerr <<"Character " << static_cast<int>(cur) << " not representable in UTF8.\n";
            std::exit(1);
        }
        
        switch (nrBytes)
        {
        case 1:
        {
            unsigned char b1{static_cast<unsigned char>(cur)};
            output.push_back(static_cast<char>(b1));
            break;
        }
        case 2:
        {
            unsigned char b2{static_cast<unsigned char>((cur & 0x3F) | 0x80)};
            unsigned char b1{static_cast<unsigned char>(((cur >> 6) & 0x1F) | 0xC0)};
            output.push_back(static_cast<char>(b1));
            output.push_back(static_cast<char>(b2));
            break;
        }
        case 3:
        {
            unsigned char b3{static_cast<unsigned char>((cur & 0x3F) | 0x80)};
            unsigned char b2{static_cast<unsigned char>(((cur >> 6) & 0x3F) | 0x80)};
            unsigned char b1{static_cast<unsigned char>(((cur >> 12) & 0x0F) | 0xE0)};
            output.push_back(static_cast<char>(b1));
            output.push_back(static_cast<char>(b2));
            output.push_back(static_cast<char>(b3));
            break;
        }
        case 4:
        {
            unsigned char b4{static_cast<unsigned char>((cur & 0x3F) | 0x80)};
            unsigned char b3{static_cast<unsigned char>(((cur >> 6) & 0x3F) | 0x80)};
            unsigned char b2{static_cast<unsigned char>(((cur >> 12) & 0x3F) | 0x80)};
            unsigned char b1{static_cast<unsigned char>(((cur >> 18) & 0x07) | 0xF0)};
            output.push_back(static_cast<char>(b1));
            output.push_back(static_cast<char>(b2));
            output.push_back(static_cast<char>(b3));
            output.push_back(static_cast<char>(b4));
            break;
        }
        }
    }

    return output;
}

void ignoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
bool hasUnextractedInput()
{
    return (!std::cin.eof() && std::cin.peek() != '\n');
}
bool clearFailedExtraction()
{
    if (!std::cin || hasUnextractedInput())
    {
        if (std::cin.eof())
            std::exit(0);
        
        std::cin.clear();
        ignoreLine();
        return true;
    }
    return false;
}
int getInt()
{
    while (true)
    {
        std::cout << "Enter a key value between 1 and 30: ";
        int x{};
        std::cin >> x;
        if (x < 1 || x > 30) 
            std::cin.setstate(std::ios::failbit);
        if (clearFailedExtraction())
        {
            std::cout << "Oops, seems like you ARE INCOMPETENT ENOUGH TO WRITE A GOD DAMN NUMBER BETWEEN 1 AND 30. But it's ok, i forgive you. try again: ";
            continue;

        }

        return x;
    }
}
void normalize(std::u32string& str)
{
    for (char32_t& c : str)
    {
        // Accept Romanian cedilla variants and normalize them
        // to the modern comma-below characters.

        if (c == U'Ş')
            c = U'Ș';
        else if (c == U'Ţ')
            c = U'Ț';
        else if (c == U'ş')
            c = U'ș';
        else if (c == U'ţ')
            c = U'ț';

        // Convert lowercase Romanian alphabet to uppercase
        // using our alphabet ordering.

        auto pos{g_ralph_lower.find(c)};

        if (pos != std::u32string_view::npos)
            c = g_ralph[pos];
    }
}
bool validCharacters(std::string& str)
{
    std::u32string message{utf8_to_utf32(str)};

    normalize(message);
    
    auto it = std::ranges::find_if(message, [](char32_t x) {
        return (g_ralph.find(x) == std::u32string_view::npos)
            && (x != emptySpace);
    });

    if (it != message.end())
    {
        std::cout
            << utf32_to_utf8(std::u32string(1, *it)) 
            << " Is not a valid character!\n";

        return false;
    }

    str = utf32_to_utf8(message);

    return true;
}
std::string getMessage()
{
    while (true)
    {
        std::cout << "Insert your message: ";
        std::string message{};
        std::getline(std::cin >> std::ws, message);
        if (!validCharacters(message))
        {
            continue;
        }

        return message;
    }

}

//Normal caesar
std::string caesarCipher(
    std::string_view message,
    int key,
    bool decrypt = false)
{
    std::u32string text{utf8_to_utf32(message)};

    normalize(text);

    //  spaces disappear before encryption.
    std::erase(text, U' ');

    std::u32string result{};
    result.reserve(text.size());

    for (char32_t c : text)
    {
        auto pos = g_ralph.find(c);

        // This should normally already have been caught by
        // validCharacters(), but keep the function safe (protective programming hehe)
        if (pos == std::u32string_view::npos)
        {
            std::cerr << "Invalid Romanian character: "
                      << utf32_to_utf8(std::u32string(1, c))
                      << '\n';
            std::exit(1);
        }

        std::size_t newPos{};

        if (!decrypt)
        {
            newPos =
                (pos + static_cast<std::size_t>(key))
                % g_ralph.size();
        }
        else
        {
            // Adding 31 prevents unsigned underflow
            newPos =
                (pos + g_ralph.size()
                 - static_cast<std::size_t>(key))
                % g_ralph.size();
        }

        result.push_back(g_ralph[newPos]);
    }

    return utf32_to_utf8(result);
}
//caesar with permutation
std::string caesarPermutationCipher(
    std::string_view message,
    int key,
    std::string_view keyword,
    bool decrypt = false)
{
    //convert/normalize keyword
    std::u32string keyword32{utf8_to_utf32(keyword)};
    normalize(keyword32);

    //keyword must be at least 7 characters
    if (keyword32.size() < 7)
    {
        std::cerr
            << "Keyword must contain at least 7 characters.\n";
        std::exit(1);
    }

    // Keyword may contain ONLY Romanian letters
    for (char32_t c : keyword32)
    {
        if (g_ralph.find(c) == std::u32string_view::npos)
        {
            std::cerr
                << "Invalid character in keyword: "
                << utf32_to_utf8(std::u32string(1, c))
                << '\n';

            std::exit(1);
        }
    }

    //build permuted alphabet
    std::u32string permuted{};
    permuted.reserve(g_ralph.size());

    // First: distinct letters from keyword
    for (char32_t c : keyword32)
    {
        if (permuted.find(c) == std::u32string::npos)
            permuted.push_back(c);
    }

    // Then: remaining Romanian letters in natural order
    for (char32_t c : g_ralph)
    {
        if (permuted.find(c) == std::u32string::npos)
            permuted.push_back(c);
    }

    // Printing the new alphabet
    std::cout
        << "Permuted alphabet: "
        << utf32_to_utf8(permuted)
        << '\n';


    //==========encrypt/decrypt message==========
    std::u32string text{utf8_to_utf32(message)};
    normalize(text);

    std::erase(text, U' ');

    std::u32string result{};
    result.reserve(text.size());

    for (char32_t c : text)
    {
        auto pos = permuted.find(c);

        if (pos == std::u32string::npos)
        {
            std::cerr
                << "Invalid Romanian character: "
                << utf32_to_utf8(std::u32string(1, c))
                << '\n';

            std::exit(1);
        }

        std::size_t newPos{};

        if (!decrypt)
        {
            newPos =
                (pos + static_cast<std::size_t>(key))
                % permuted.size();
        }
        else
        {
            newPos =
                (pos + permuted.size()
                 - static_cast<std::size_t>(key))
                % permuted.size();
        }

        result.push_back(permuted[newPos]);
    }

    return utf32_to_utf8(result);
}

void begin(int nrKeys)
{
    std::string input{getMessage()};
    int key{getInt()};

    switch (nrKeys)
    {
    case 1:
    {
        std::cout << "Result: "
                  << caesarCipher(input, key)
                  << '\n';
        break;
    }

    case 2:
    {
        std::cout << "Enter keyword: ";
        std::string keyword{};
        std::getline(std::cin >> std::ws, keyword);

        std::cout << "Result: "
                  << caesarPermutationCipher(input, key, keyword)
                  << '\n';
        break;
    }

    default:
        std::cerr << "Number of keys must be either 1 or 2.\n";
        break;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: program.exe [nrKeys]\n";
        return 1; // important: don't continue to argv[1]
    }

    int nrKeys{};

    try
    {
        nrKeys = std::stoi(argv[1]);
    }
    catch (...)
    {
        std::cerr << "nrKeys must be 1 or 2.\n";
        return 1;
    }

    begin(nrKeys);

    return 0;
}
