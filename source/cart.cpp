#include <string>
#include <sstream>
#include <map>

#include "cart.h"
#include "filehelpers.h"

#include "utils.h"

#include "logger.h"

#include "cartPatcher.h"
#include "utf8-util.h"

#include "FakoBios.h"

//#include "tests/test_base.h"
//#if _TEST
//#include "tests/cart_test.h"
//#endif


std::map<char32_t, uint8_t> emoji = {
    {0x025ae, 0x10}, {0x025a0, 0x11}, {0x025a1, 0x12}, {0x02059, 0x13}, {0x02058, 0x14},
    {0x02016, 0x15}, {0x025c0, 0x16}, {0x025b6, 0x17}, {0x0300c, 0x18}, {0x0300d, 0x19},
    {0x000a5, 0x1a}, {0x02022, 0x1b}, {0x03001, 0x1c}, {0x03002, 0x1d}, {0x0309b, 0x1e},
    {0x0309c, 0x1f}, {0x00020, 0x20}, {0x00021, 0x21}, {0x00022, 0x22}, {0x00023, 0x23},
    {0x00024, 0x24}, {0x00025, 0x25}, {0x00026, 0x26}, {0x00027, 0x27}, {0x00028, 0x28},
    {0x00029, 0x29}, {0x0002a, 0x2a}, {0x0002b, 0x2b}, {0x0002c, 0x2c}, {0x0002d, 0x2d},
    {0x0002e, 0x2e}, {0x0002f, 0x2f}, {0x00030, 0x30}, {0x00031, 0x31}, {0x00032, 0x32},
    {0x00033, 0x33}, {0x00034, 0x34}, {0x00035, 0x35}, {0x00036, 0x36}, {0x00037, 0x37},
    {0x00038, 0x38}, {0x00039, 0x39}, {0x0003a, 0x3a}, {0x0003b, 0x3b}, {0x0003c, 0x3c},
    {0x0003d, 0x3d}, {0x0003e, 0x3e}, {0x0003f, 0x3f}, {0x00040, 0x40}, {0x1d622, 0x41},
    {0x1d623, 0x42}, {0x1d624, 0x43}, {0x1d625, 0x44}, {0x1d626, 0x45}, {0x1d627, 0x46},
    {0x1d628, 0x47}, {0x1d629, 0x48}, {0x1d62a, 0x49}, {0x1d62b, 0x4a}, {0x1d62c, 0x4b},
    {0x1d62d, 0x4c}, {0x1d62e, 0x4d}, {0x1d62f, 0x4e}, {0x1d630, 0x4f}, {0x1d631, 0x50},
    {0x1d632, 0x51}, {0x1d633, 0x52}, {0x1d634, 0x53}, {0x1d635, 0x54}, {0x1d636, 0x55},
    {0x1d637, 0x56}, {0x1d638, 0x57}, {0x1d639, 0x58}, {0x1d63a, 0x59}, {0x1d63b, 0x5a},
    {0x0005b, 0x5b}, {0x0005c, 0x5c}, {0x0005d, 0x5d}, {0x0005e, 0x5e}, {0x0005f, 0x5f},
    {0x00060, 0x60}, {0x00061, 0x61}, {0x00062, 0x62}, {0x00063, 0x63}, {0x00064, 0x64},
    {0x00065, 0x65}, {0x00066, 0x66}, {0x00067, 0x67}, {0x00068, 0x68}, {0x00069, 0x69},
    {0x0006a, 0x6a}, {0x0006b, 0x6b}, {0x0006c, 0x6c}, {0x0006d, 0x6d}, {0x0006e, 0x6e},
    {0x0006f, 0x6f}, {0x00070, 0x70}, {0x00071, 0x71}, {0x00072, 0x72}, {0x00073, 0x73},
    {0x00074, 0x74}, {0x00075, 0x75}, {0x00076, 0x76}, {0x00077, 0x77}, {0x00078, 0x78},
    {0x00079, 0x79}, {0x0007a, 0x7a}, {0x0007b, 0x7b}, {0x0007c, 0x7c}, {0x0007d, 0x7d},
    {0x0007e, 0x7e}, {0x025cb, 0x7f}, {0x02588, 0x80}, {0x02592, 0x81}, {0x1f431, 0x82},
    {0x02b07, 0x83}, {0x02591, 0x84}, {0x0273d, 0x85}, {0x025cf, 0x86}, {0x02665, 0x87},
    {0x02609, 0x88}, {0x0c6c3, 0x89}, {0x02302, 0x8a}, {0x02b05, 0x8b}, {0x1f610, 0x8c},
    {0x0266a, 0x8d}, {0x1f17e, 0x8e}, {0x025c6, 0x8f}, {0x02026, 0x90}, {0x027a1, 0x91},
    {0x02605, 0x92}, {0x029d7, 0x93}, {0x02b06, 0x94}, {0x002c7, 0x95}, {0x02227, 0x96},
    {0x0274e, 0x97}, {0x025a4, 0x98}, {0x025a5, 0x99}, {0x03042, 0x9a}, {0x03044, 0x9b},
    {0x03046, 0x9c}, {0x03048, 0x9d}, {0x0304a, 0x9e}, {0x0304b, 0x9f}, {0x0304d, 0xa0},
    {0x0304f, 0xa1}, {0x03051, 0xa2}, {0x03053, 0xa3}, {0x03055, 0xa4}, {0x03057, 0xa5},
    {0x03059, 0xa6}, {0x0305b, 0xa7}, {0x0305d, 0xa8}, {0x0305f, 0xa9}, {0x03061, 0xaa},
    {0x03064, 0xab}, {0x03066, 0xac}, {0x03068, 0xad}, {0x0306a, 0xae}, {0x0306b, 0xaf},
    {0x0306c, 0xb0}, {0x0306d, 0xb1}, {0x0306e, 0xb2}, {0x0306f, 0xb3}, {0x03072, 0xb4},
    {0x03075, 0xb5}, {0x03078, 0xb6}, {0x0307b, 0xb7}, {0x0307e, 0xb8}, {0x0307f, 0xb9},
    {0x03080, 0xba}, {0x03081, 0xbb}, {0x03082, 0xbc}, {0x03084, 0xbd}, {0x03086, 0xbe},
    {0x03088, 0xbf}, {0x03089, 0xc0}, {0x0308a, 0xc1}, {0x0308b, 0xc2}, {0x0308c, 0xc3},
    {0x0308d, 0xc4}, {0x0308f, 0xc5}, {0x03092, 0xc6}, {0x03093, 0xc7}, {0x03063, 0xc8},
    {0x03083, 0xc9}, {0x03085, 0xca}, {0x03087, 0xcb}, {0x030a2, 0xcc}, {0x030a4, 0xcd},
    {0x030a6, 0xce}, {0x030a8, 0xcf}, {0x030aa, 0xd0}, {0x030ab, 0xd1}, {0x030ad, 0xd2},
    {0x030af, 0xd3}, {0x030b1, 0xd4}, {0x030b3, 0xd5}, {0x030b5, 0xd6}, {0x030b7, 0xd7},
    {0x030b9, 0xd8}, {0x030bb, 0xd9}, {0x030bd, 0xda}, {0x030bf, 0xdb}, {0x030c1, 0xdc},
    {0x030c4, 0xdd}, {0x030c6, 0xde}, {0x030c8, 0xdf}, {0x030ca, 0xe0}, {0x030cb, 0xe1},
    {0x030cc, 0xe2}, {0x030cd, 0xe3}, {0x030ce, 0xe4}, {0x030cf, 0xe5}, {0x030d2, 0xe6},
    {0x030d5, 0xe7}, {0x030d8, 0xe8}, {0x030db, 0xe9}, {0x030de, 0xea}, {0x030df, 0xeb},
    {0x030e0, 0xec}, {0x030e1, 0xed}, {0x030e2, 0xee}, {0x030e4, 0xef}, {0x030e6, 0xf0},
    {0x030e8, 0xf1}, {0x030e9, 0xf2}, {0x030ea, 0xf3}, {0x030eb, 0xf4}, {0x030ec, 0xf5},
    {0x030ed, 0xf6}, {0x030ef, 0xf7}, {0x030f2, 0xf8}, {0x030f3, 0xf9}, {0x030c3, 0xfa},
    {0x030e3, 0xfb}, {0x030e5, 0xfc}, {0x030e7, 0xfd}, {0x025dc, 0xfe}, {0x025dd, 0xff},
};

std::string convert_emojis(const std::string& lua) {
	std::string res;
	for (char32_t codepoint : utf8::CodepointIterator(lua)) {
		if (codepoint < 0x80) {
			res.push_back(uint8_t(codepoint));
		} else {
			auto i = emoji.find(codepoint);
			if (i != emoji.end()) {
				res.push_back(i->second);
			}
		}
	}
	return res;
}

//tac08 based cart parsing and stripping of emoji
Cart::Cart(std::string filename){
    Filename = filename;\
    Logger::Write("getting file contents\n");

    std::string cartStr;

    if (filename == "__FAKE08-BIOS.p8") {
        cartStr = fake08BiosP8;
    }
    else {
        cartStr = get_file_contents(filename.c_str());
    }
    Logger::Write("Got file contents... parsing cart\n");

    fullCartText = cartStr;

    std::istringstream s(cartStr);
    std::string line;
    std::string currSec = "";
    
    while (std::getline(s, line)) {
		line = utils::trimright(line, " \n\r");
        line = convert_emojis(line);

        if (line.length() > 2 && line[0] == '_' && line[1] == '_') {
            currSec = line;
        }
        else if (currSec == "__lua__"){
            LuaString += line + "\n";
        }
        else if (currSec == "__gfx__"){
            SpriteSheetString += line + "\n";
        }
        else if (currSec == "__gff__"){
            SpriteFlagsString += line + "\n";
        }
        else if (currSec == "__map__"){
            MapString += line + "\n";
        }
        else if (currSec == "__sfx__"){
             SfxString += line + "\n";
        }
        else if (currSec == "__music__"){
             MusicString += line + "\n";
        }
	}

    const char * patched = getPatchedLua(LuaString.c_str());

    LuaString = patched;
    
    
    #if _TEST
    //run tests to make sure cart is parsed correctly
    //verifyFullCartText(cartStr);

    //verifyCart(this);

    #endif
}
