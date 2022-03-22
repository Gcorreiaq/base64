#include <iostream>
#include <cinttypes>
#include <string.h>

char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int encode(std::string message, std::string *dst)
{
    int str_size = message.length();
    int pad = str_size % 3;
    int a = 0;
    do {
        *dst += b64[(message[3*a] >> 2) & 0x3f];
        if (pad == 1 && (3*a + pad) == str_size) {
            *dst += b64[(message[3*a] & 0x3) << 4];
            break;
        }
        else *dst += b64[((message[3*a] & 0x3) << 4) | (message[3*a + 1] >> 4) & 0xf];
        if (pad == 2 && (3*a + pad) == str_size) {
            *dst += b64[(message[3*a + 1] & 0xf) << 2];
            break;
        }
        *dst += b64[((message[3*a + 1] & 0xf) << 2) | (message[3*a + 2] >> 6) & 0x3];
        *dst += b64[message[3*a + 2] & 0x3f];
        a++;
    } while (a < (str_size / 3) + (pad & 3)) ;
    if (pad) for (int i = 3 - pad;i > 0;i--) *dst += '=';
    return dst->length();
}
int decode(std::string message, std::string *dst)
{
    int str_size = message.length();
    int a = 0;
    int d = 0;
    do {
        d = 4*a;
        for (int i = 0;i < 4;i++)
            for (int b = 0;b < 65;b++) 
                if (message[d+i] == b64[b])
                {
                    message[d+i] = b;
                    break;
                }
        *dst += (((message[d] << 2) & 0xfc) | (message[d+1] >> 4) & 0x3);
        if (message[d+2] == '=' && ((d + 4) == str_size)) break;
        *dst += (((message[d+1] << 4) & 0xf0) | (message[d+2] >> 2) & 0xf);
        if (message[d+3] == '=' && ((d + 4) == str_size)) break;
        *dst += (((message[d+2] << 6) & 0xc0) | message[d+3] & 0x3f);
        a++;
    } while (a < (str_size / 4));
    return dst->length();
}

int main(int argc, char **argv)
{   
    std::string message{0};
    std::string encoded{0};
    if (argc < 2)
    {
        std::cout << "Usage: base64.exe <option>\n" << "-d: Decode a string\n" << "-e: Encode a string\n";
        return 0;
    }
    std::cout << "Enter a message: ";
    std::getline(std::cin >> std::ws, message);
    if (!strcmp(argv[1], "-d")) decode(message, &encoded);
    else encode(message, &encoded);
    std::cout << encoded;
    return 0;
}