// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <FightingGameJam_v2/PlayerController/PlayerInputStruct.h>
#include "InputStructFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTINGGAMEJAM_V2_API UInputStructFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	static bool Equals(FUPlayerInputStruct input1, FUPlayerInputStruct input2);

	UFUNCTION(BlueprintCallable, Category = "Input")
	static bool Contains(FUPlayerInputStruct input1, FUPlayerInputStruct input2);

};
