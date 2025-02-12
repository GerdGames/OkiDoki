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
    return 255 == (~input1.ToInt() | input2.ToInt());
}