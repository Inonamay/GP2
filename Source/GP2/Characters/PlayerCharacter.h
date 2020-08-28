// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Components/DayNightController.h"
#include "PlayerCharacter.generated.h"
UENUM(Blueprinttype)
enum ActionError {
	NoPath, NotEnoughPoints
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionPointChange, int, previousActionpoints, int, currentActionpoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionError, ActionError, errorType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPathFound, TArray<AActor*>, path);

class UWalkableComponent;
UCLASS()
class GP2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UDayNightController* dayNightComponent;
public:
	// Sets default values for this character's properties
	APlayerCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		AActor* startingTile;
		UWalkableComponent* currentTile;
	UFUNCTION(BlueprintPure)
		TimeState GetCurrentState() { return dayNightComponent->GetState(); }
	UFUNCTION(Blueprintcallable)
		void ChangeTimeOfDay(bool toggle, TimeState state);
	UFUNCTION(BlueprintPure)
	UDayNightController* GetTimeController() { return dayNightComponent; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		bool codeClickToMove = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		bool moveAutomaticly = false;
	UFUNCTION(Blueprintcallable)
		void MoveToMapLocation(TArray<UWalkableComponent*> location);
	UFUNCTION(Blueprintpure)
		int GetActionPoints() { return currentActionPoints; }
	UFUNCTION(Blueprintcallable)
    bool DoAction(int pointsCost);
	UFUNCTION(Blueprintcallable)
		void ReplenishActionPoints(int amount);
	UFUNCTION(Blueprintcallable)
		void CheckForWalkable();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int maxActionPoints = 11;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
		int currentActionPoints = 11;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnActionPointChange onPointChange;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnActionError actionFailed;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FPathFound onFoundPath;
	UInputComponent* inputComponent;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
