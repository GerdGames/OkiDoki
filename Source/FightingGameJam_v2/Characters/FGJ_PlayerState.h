// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FGJ_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTINGGAMEJAM_V2_API AFGJ_PlayerState : public APlayerState
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Position;
};
