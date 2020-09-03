// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/DayNightController.h"
#include "GP2GameModeBase.generated.h"
class UWalkableComponent;
class APlayerCharacter;
USTRUCT()
struct FAction {
	GENERATED_BODY()
public:
	TimeState timeState;
	UWalkableComponent* currentTile;
	TArray<AActor*> additionalActions;
};
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUndoPlayerMove, AActor*, tile);
UCLASS()
class GP2_API AGP2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	TArray<FAction*> playerActions;
	virtual void BeginPlay() override;
	UFUNCTION(Blueprintcallable)
		void RecordAction();
	UFUNCTION(Blueprintcallable)
		void AddAdditionalRedo(TArray<AActor*> actors);
	UFUNCTION(Blueprintcallable)
		void Redo(int stepsAmount);
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnUndoPlayerMove UndoPlayerMove;
private:
	UDayNightController* timeController;
	APlayerCharacter* player;
};
