// Fill out your copyright notice in the Description page of Project Settings.


#include "RollbackPlayerController.h"

void ARollbackPlayerController::BeginPlay()
{
	Super::BeginPlay();

	currentInput = FUPlayerInputStruct();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(inputMapping, 0);
	}
}

void ARollbackPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);

	if (Input) {
		Input->BindAction(inputMoveLeft, ETriggerEvent::Triggered, this, &ARollbackPlayerController::MoveLeft);
		Input->BindAction(inputMoveRight, ETriggerEvent::Triggered, this, &ARollbackPlayerController::MoveRight);
		Input->BindAction(inputMoveDown, ETriggerEvent::Triggered, this, &ARollbackPlayerController::MoveDown);
		Input->BindAction(inputMoveUp, ETriggerEvent::Triggered, this, &ARollbackPlayerController::MoveUp);
		Input->BindAction(inputLightPunch, ETriggerEvent::Triggered, this, &ARollbackPlayerController::LightPunch);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Inputs Set for Controller %i"), this->GetInputIndex()));
		
	}
	
}

void ARollbackPlayerController::MoveLeft(const FInputActionValue& Value)
{
	currentInput.LFT = Value.Get<bool>();

	/*if (currentInput.LFT) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Left: On");
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Left: Off");
	}*/
	
}

void ARollbackPlayerController::MoveRight(const FInputActionValue& Value)
{
	currentInput.RGT = Value.Get<bool>();
	
	/*if (currentInput.RGT) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Right: On");
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Right: Off");
	}*/
}

void ARollbackPlayerController::MoveDown(const FInputActionValue& Value)
{
	currentInput.DWN = Value.Get<bool>();

	/*if (currentInput.DWN) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Down: On");
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Down: Off");
	}*/
}

void ARollbackPlayerController::MoveUp(const FInputActionValue& Value)
{
	currentInput.UP = Value.Get<bool>();

	/*if (currentInput.UP) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Up: On");
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Up: Off");
	}*/
}

void ARollbackPlayerController::LightPunch(const FInputActionValue& Value)
{
	currentInput.LP = Value.Get<bool>();

	/*if (currentInput.LP) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Light Punch: On");
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Light Punch: Off");
	}*/
}
