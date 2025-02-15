// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/InputStructFunctionLibrary.h"

bool UInputStructFunctionLibrary::Equals(FUPlayerInputStruct input1, FUPlayerInputStruct input2)
{
    return (input1.ToInt() == input2.ToInt());
}

bool UInputStructFunctionLibrary::Contains(FUPlayerInputStruct input1, FUPlayerInputStruct input2)
{
    //Would be an if then logic gate, which converts to not A or B https://en.wikipedia.org/wiki/Material_conditional
    //If it's in this input struct, but not in the other, return false
    //If it's in this input struct and in the other return true
    //If its not in this input struct, always return true

    //if all inputs are true, the result should equal 255
    //return 256 == (~input1.ToInt() | input2.ToInt());

    //Doing a slightly dumber method
    
    //check if input one is on, if so check if input two is on

    int int1 = input1.ToInt();
    int int2 = input2.ToInt();

    //loop over bits in struct
    for (int i = 0; i < input1.inputNum; i++)
    {
        int bit1 = int1 & 1;
        int bit2 = int2 & 1;

        //check if input one is on
        if (bit1 == 1)
        {
            //if so check if input two is on
            //if so keep checking
            if (bit2 == 1)
            {
                continue;
            }
            //else stop there and return false, input 1 does not contain input 2
            else {
                return false;
            }
        }

        int1 >>= 1;
        int2 >>= 1;
    }

    //if you made it through each bit, input2 matches the input requirement of input1
    return true;
}

int UInputStructFunctionLibrary::ToInt(FUPlayerInputStruct input)
{
    return input.ToInt();
}

