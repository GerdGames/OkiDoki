// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Rollback_1v1_GameState.h"
#include "Rollback_1v1_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTINGGAMEJAM_V2_API ARollback_1v1_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARollback_1v1_GameMode();

	ABaseCharacter* P1Character, *P2Character;

	virtual void BeginPlay() override;
};
