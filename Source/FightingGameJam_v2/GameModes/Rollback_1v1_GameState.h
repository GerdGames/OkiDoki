// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "../PlayerController/RollbackPlayerController.h"
#include "..\..\..\Plugins\GGPOUE5-master\Source\GGPOUE4\Public\include\ggponet.h"
#include <FightingGameJam_v2\Characters\BaseCharacter.h>
#include "FGJ_GameState.h"
#include <GGPOGameInstance.h>
#include "Rollback_1v1_GameState.generated.h"



/**
 * 
 */
UCLASS()
class FIGHTINGGAMEJAM_V2_API ARollback_1v1_GameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	ARollbackPlayerController *LocalController;
	FUPlayerInputStruct LocalInput;
	bool Active = false;
	int startCount = 0;
	
public:
	ARollback_1v1_GameState();

	UFUNCTION(BlueprintCallable, Category = "GGPO")
	void StartConnection(UClass* playerClass);

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	bool StartSession(int player, int port, int opPort, const char* opIp);

	/*template<class T>
	T* SpawnActor(FVector const& Location, FRotator const& Rotation, AActor* Owner, APawn* Instigator, bool bNoCollisionFail);*/

};

void FGJ_AdvanceFrame(int inputs[], int disconnect_flags);

void UpdateGameStateFromWorld();

void UpdateWorldFromGameState();