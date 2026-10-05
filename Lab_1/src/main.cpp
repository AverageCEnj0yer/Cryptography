#include <algorithm>
#include <cstdio>
#include <ios>
#include <iostream>
#include <array>
#include <limits>
#include <string>
#include <string_view>

//Cryptography lab turned into mini UTF encoding library project!

// [[maybe_unused]] constexpr std::array gg_ralph = {
//     U'A', U'Ă', U'Â', U'B', U'C', U'D', U'E', U'F', U'G', U'H', U'I', 
//     U'Î', U'J', U'K', U'L', U'M', U'N', U'O', U'P', U'Q', U'R', U'S', 
//     U'Ș', U'T', U'Ț', U'U', U'V', U'W', U'X', U'Y', U'Z'
// };

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


int main()
{

    std::string input{getMessage()};

    std::cout << input;
    [[maybe_unused]] int key{getInt()};
    return 0;
}
