// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../PlayerController/PlayerInputStruct.h"
#include "FGJ_PlayerState.h"
#include "FGJ_Player_StateStruct.h"
#include "BaseCharacter.generated.h"


UCLASS()
class FIGHTINGGAMEJAM_V2_API ABaseCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseCharacter();

	//The opposing character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Opponent")
	ABaseCharacter* opponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//called to handle one frame of input
	UFUNCTION(BlueprintImplementableEvent)
	void HandleInput(FUPlayerInputStruct currentInput);

	//called to set state
	UFUNCTION(BlueprintImplementableEvent)
	void SetState(FFGJ_Player_StateStruct newState);

	//called to get Character State
	UFUNCTION(BlueprintImplementableEvent)
	void GetState(FFGJ_Player_StateStruct& curState);

};
