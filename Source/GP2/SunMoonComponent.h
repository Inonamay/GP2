// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DayNightTriggerComponent.h"
#include "Math/Range.h"
#include "Misc/App.h"
#include "SunMoonComponent.generated.h"

UENUM()
enum DayNight {
	TestDay UMETA(DisplayName = "Day State"),
	TestNight UMETA(DisplayName = "Day State"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP2_API USunMoonComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
	UDayNightTriggerComponent* dayNightTriggerComponent;


public:
	
	UPROPERTY(EditAnywhere)
	float distanceFromCenter = 500;
	
	UPROPERTY(EditAnywhere)
	float activeHeight = 100;
	UPROPERTY(EditAnywhere)
	float inactiveHeight = -100;

	FVector lookDirection;
	FRotator celestialRotation;

	float time = 0;
	UPROPERTY(EditAnywhere)
	float duration = 2;


	float sunActiveIntensity;
	float sunInactiveIntensity;
	float moonActiveIntensity;
	float moonInactiveIntensity;

	UPROPERTY(EditAnywhere)
	int transitionTime = 10;
	UPROPERTY(EditAnywhere)
	int interpSpeed = 100;

	UPROPERTY(EditAnywhere)
	int celestialVerticalSpeed;

	bool toggle;

	APawn* playerCharacter;
	UPROPERTY(EditAnywhere)
	AActor* center;
	UPROPERTY(EditAnywhere)
	AActor* sun;
	UPROPERTY(EditAnywhere)
	AActor* moon;

	TEnumAsByte<DayNight> dayOrNight;

private:

	TArray<AActor*> celestialsArray;

	float sunCurrentIntensity;
	float moonCurrentIntensity;

	float dayAngle = 0;
	float nightAngle = 180;

	FRotator startRot;
	FRotator targetRot;
	FRotator dayRot;
	FRotator nightRot;

	FVector sunCurrentLocation;
	FVector sunTargetLocation;
	FVector moonTargetLocation;

	FVector vActiveHeight;
	FVector vInactiveHeight;

public:	
	// Sets default values for this component's properties
	USunMoonComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void ToggleCelestials(int dayNight);
	void MoveCelestials(float DeltaTime);

};
