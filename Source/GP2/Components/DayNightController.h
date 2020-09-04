// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DayNightController.generated.h"
class UDayNightTriggerComponent;
UENUM(Blueprinttype)
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
	UFUNCTION(Blueprintcallable)
	void SetTime(TimeState _state);
	UFUNCTION(Blueprintpure)
		TimeState GetState();
protected:
private:
public:	

		
};
