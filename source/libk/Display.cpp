#include "Display.hpp"

Display Console;

Display::Display()
{
    this->Buffer = (u1 *)0xb8000;

    this->X = 0;
    this->Y = 0;

    for (int x = 0; x < (80 * 25) * 2; x++) {
        Buffer[x++] = 0;
    }
}

void Display::Write(const char* Message) {

    for (size_t Counter = 0; Counter < strlen(Message); ++Counter) {

        if (Message[Counter] == '\n') {
            NextLine();
        }

        else {

            X++;
            if (X > 80) {
                NextLine();
            }

            Write(X,Y,Message[Counter]);
        }

    }
}

void Display::Write(Object XF, Object YF, char Value) {
    this->Buffer[((XF-1) + (YF * 80)) * 2] = Value;
}

char Display::Get(Object XF, Object YF) {
    return this->Buffer[((XF-1) + (YF * 80)) * 2];
}

void Display::NextLine() {
    this->X = 0;
    this->Y++;
    if (Y > 25) {

        ClearLine(0);
        Y--;

        for (int YF = 0; YF < 25; ++YF) {
            for (int XF = 0; XF < 80; ++XF) {
                Write(XF,YF, Get(XF,YF+1));
            }
        }

        ClearLine(25);

    }
}

void Display::ClearLine(Object YF) {

    for (size_t Counter = 0; Counter < 80; ++Counter) {
        Write(Counter,YF,0);
    }

}

void Display::Write(GUID Address) {

    for (int Counter = 0; Counter < 40; ++Counter) {

        Write((char)Address.Internal[Counter]);

    }
}

void Display::Write(char Letter) {

    if (Letter == '\n') {
        NextLine();
    }

    else {

        X++;

        if (X > 80) {

            NextLine();

        }

        Write(X,Y,Letter);

     }
}
