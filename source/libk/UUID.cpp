#include "UUID.h"

GUID GenerateNullGUID1() {
    GUID WantedGUID{};

    for (int i = 0; i < GUIDSize; ++i) {
        WantedGUID.Internal[i] = 0;
    }

    return WantedGUID;
}

GUID GenerateNullGUID() {
    GUID WantedGUID{};

    for (int i = 0; i < GUIDSize; ++i) {
        WantedGUID.Internal[i] = 0;
    }

    return WantedGUID;
}

void GenerateGUID(GUID* GUID) {

    srand ((uintptr_t)GUID);
    int t = 0;
    const char *szTemp = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
    const char *szHex = "0123456789ABCDEF-";
    size_t nLen = strlen (szTemp);

    for (size_t t=0; t<nLen+1; t++)
    {
        int r = rand () % 16;
        char c = ' ';

        switch (szTemp[t])
        {
            case 'x' : { c = szHex [r]; } break;
            case 'y' : { c = szHex [r & 0x03 | 0x08]; } break;
            case '-' : { c = '-'; } break;
            case '4' : { c = '4'; } break;
        }

        ((char*)GUID)[t] = ( t < nLen ) ? c : 0x00;
    }
}