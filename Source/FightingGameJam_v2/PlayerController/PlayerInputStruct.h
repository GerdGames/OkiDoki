// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "PlayerInputStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FIGHTINGGAMEJAM_V2_API FUPlayerInputStruct
{
	GENERATED_BODY()

	bool getValue();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	bool UP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	bool DWN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	bool LFT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
	bool RGT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool LP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool LK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool HK;

	int inputNum = 8;

	int ToInt();
	void FromInt(int input);

	/**/
};
