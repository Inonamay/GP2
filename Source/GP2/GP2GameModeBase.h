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
	UFUNCTION(BlueprintPure)
	TimeState GetState() { return state; }
	UFUNCTION(Blueprintcallable)
	void ToggleDayNight();
	UFUNCTION(Blueprintcallable)
		void SetTime(TimeState _state);
	void UpdateTriggerComponents();
	void AddTriggerComponent(UDayNightTriggerComponent* triggerToAdd);
private:
	TimeState state;
	TArray<UDayNightTriggerComponent*> triggers;
	APlayerCharacter* player;
};
