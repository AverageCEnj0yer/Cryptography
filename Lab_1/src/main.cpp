#include <iostream>
#include <array>
#include <string>
#include <string_view>

//Cryptography lab turned into mini UTF encoding library project!

[[maybe_unused]] constexpr std::array g_ralph = {
    U'A', U'Ă', U'Â', U'B', U'C', U'D', U'E', U'F', U'G', U'H', U'I', 
    U'Î', U'J', U'K', U'L', U'M', U'N', U'O', U'P', U'Q', U'R', U'S', 
    U'Ș', U'T', U'Ț', U'U', U'V', U'W', U'X', U'Y', U'Z'
};

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

            if ((ch < 0x0800) || (ch > 0xFFFF) || ((ch > 0xD800) && (ch < 0xDFFF)))
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

[[maybe_unused]] constexpr auto gg_ralph = std::to_array(U"AĂÂBCDEFGHIÎJKLMNOPQRSȘTȚUVWXYZ");
int main()
{

    // std::string_view test{"😄ĂÂÎȘȚ"};
    // std::u32string result{utf8_to_utf32(test)};

    // for (const auto& i : result)
    // {
    //     const int found{(std::ranges::find(g_ralph, i) != g_ralph.end() ? 1 : 0)};
    //     if (!found)
    //     {
    //         std::cout << "Character " << static_cast<int>(i) << " is not a valid letter fuck you\n";
    //     }
    // }




    return 0;
}
