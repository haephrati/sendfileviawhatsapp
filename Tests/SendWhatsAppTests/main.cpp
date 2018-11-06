#include <cstdio>
#include <cstdlib>
#include "Resource.h"

static void Fail(const char* msg)
{
    std::printf("FAIL %s\n", msg);
    std::exit(1);
}

int main()
{
    if (IDD_ABOUTBOX != 100)
        Fail("about");
    if (IDD_SG_SendWhatsApp_DIALOG != 102)
        Fail("dialog");
    if (IDR_MAINFRAME != 128)
        Fail("mainframe");
    if (IDI_ICON2 != 133)
        Fail("icon");
    std::printf("OK SendWhatsAppTests\n");
    return 0;
}
