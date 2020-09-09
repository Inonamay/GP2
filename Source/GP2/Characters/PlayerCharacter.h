// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Components/DayNightController.h"
#include "../GP2GameModeBase.h"
#include "PlayerCharacter.generated.h"
UENUM(Blueprinttype)
enum ActionError {
	NoPath, NotEnoughPoints
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionPointChange, int, previousActionpoints, int, currentActionpoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionError, ActionError, errorType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPathFound, const TArray<AActor*>&, path, int, actionPointCost);

class UWalkableComponent;
UCLASS()
class GP2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UDayNightController* dayNightComponent;
public:
	// Sets default values for this character's properties
	AGP2GameModeBase* gameMode;
	APlayerCharacter();
#pragma region Pathfinding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		AActor* startingTile;
		UWalkableComponent* currentTile;
    UFUNCTION(Blueprintcallable)
	    void SetCurrentTile(AActor* tile);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		bool codeClickToMove = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
		bool moveAutomaticly = false;
	UFUNCTION(Blueprintcallable)
		void MoveToPath(AActor* actor);
	void MoveToMapLocation(TArray<UWalkableComponent*> location);
	UFUNCTION(Blueprintcallable)
		void GeneratePathToCurrentClickable();
	UFUNCTION(Blueprintcallable)
		void GeneratePathToWalkable(AActor* tile);
	UFUNCTION(BlueprintPure)
		UWalkableComponent* GetCurrentTile() { return currentTile; }
#pragma endregion
#pragma region Action points
	UFUNCTION(Blueprintpure)
		int GetActionPoints() { return currentActionPoints; }
	UFUNCTION(Blueprintcallable)
    bool DoAction(int pointsCost);
	UFUNCTION(Blueprintcallable)
		void ReplenishActionPoints(int amount);
#pragma endregion
	UFUNCTION(Blueprintcallable)
		void ChangeTimeOfDay(bool toggle, TimeState state);

	UFUNCTION(BlueprintPure)
		TimeState GetCurrentState();
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int maxActionPoints = 11;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		int currentActionPoints = 11;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnActionPointChange onPointChange;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnActionError actionFailed;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FPathFound onFoundPath;
	UInputComponent* inputComponent;
private:
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
