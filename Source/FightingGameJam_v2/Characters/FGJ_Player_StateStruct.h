#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FGJ_Player_StateStruct.generated.h"

USTRUCT(BlueprintType)
struct FIGHTINGGAMEJAM_V2_API FFGJ_Player_StateStruct
{
	GENERATED_BODY()

	bool getValue();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	FVector2D Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Velocity")
	FVector2D Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual State")
	bool FacingRight;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual State")
	UAnimInstance *AnimInstance;*/
};