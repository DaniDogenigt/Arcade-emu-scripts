#include "src/FutabaVFD8/FutabaVFD8.h"

futabavfd8::VFD vfd(8,7);

// static void vfd_set_brightness(unsigned char level);


void setup()
{
    vfd.Init();
    //vfd_set_brightness(0x80);
}

void loop()
{
    // vfd.LPrintf_P(100, PSTR("The quick brown fox jumps over the lazy dog %d%d%d%d%d"), 12, 34, 56, 78, 90);

    unsigned int ts = 100;
    // byte str_len = 48;
    // char chars[] = "                1234567812345678                ";
    String spaces = "                ";
    String msg = "";
    String myString = "fucking works!";
    msg += spaces;
    msg += myString;
    msg += spaces;
    
    byte str_len = msg.length() + 1;
    byte buffer[str_len];
    msg.toCharArray(buffer, str_len);

    
    for (int i = 0; i < (str_len - 16); ++i) {
        // send command
        // vfd.Write(0, 16, (const byte*)chars+i);
        vfd.Write(0, 16, (const byte*)buffer+i);
        // wait for next iteration
        delay(ts);
    }


    delay(1000);
}
