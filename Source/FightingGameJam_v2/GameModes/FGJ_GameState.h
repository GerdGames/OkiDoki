// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include <FightingGameJam_v2\Characters\FGJ_PlayerState.h>
#include <FightingGameJam_v2\Characters\FGJ_Player_StateStruct.h>
#include "FGJ_GameState.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTINGGAMEJAM_V2_API UFGJ_GameState : public UUserDefinedStruct
{
	GENERATED_BODY()

public:
	FFGJ_Player_StateStruct Player1State;
	FFGJ_Player_StateStruct Player2State;

};
