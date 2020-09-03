// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DayNightController.h"
#include "DayNightTriggerComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeTime, int, time);
class APlayerCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP2_API UDayNightTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDayNightTriggerComponent();
	void ChangeTime(TimeState state);
protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnChangeTime onDay;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnChangeTime onNight;
};
