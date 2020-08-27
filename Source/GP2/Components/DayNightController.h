// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DayNightController.generated.h"
class UDayNightTriggerComponent;
UENUM()
enum TimeState {
	Day, Night
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP2_API UDayNightController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDayNightController();
	void AddTriggerComponent(UDayNightTriggerComponent* triggerToAdd);
	void ToggleDayNight();
	UFUNCTION(Blueprintcallable)
	void SetTime(TimeState _state);
	void UpdateTriggerComponents();
	UFUNCTION(Blueprintpure)
	TimeState GetState() { return state; }
protected:
private:
	TimeState state;
	TArray<UDayNightTriggerComponent*> triggers;
public:	

		
};
