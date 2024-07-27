//#include "C:\Users\trevo\OneDrive\Documents\Unreal Projects\SwagFighterJam\Intermediate\Build\Win64\x64\SwagFighterJamEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.h"
#include "PlayerInputStruct.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

bool FUPlayerInputStruct::getValue()
{
    return false;
}

int FUPlayerInputStruct::ToInt()
{
    int input = 0;
    
    if (UP) {
        input |= (1 << 0);
    }
    if (DWN) {
        input |= (1 << 1);
    }
    if (LFT) {
        input |= (1 << 2);
    }
    if (RGT) {
        input |= (1 << 3);
    }
    if (LP) {
        input |= (1 << 4);
    }
    

    return input;
}

void FUPlayerInputStruct::FromInt(int input)
{
    UP = CHECK_BIT(input, 0);
    DWN = CHECK_BIT(input, 1);
    LFT = CHECK_BIT(input, 2);
    RGT = CHECK_BIT(input, 3);
    LP = CHECK_BIT(input, 4);
}
