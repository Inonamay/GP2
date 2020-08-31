// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathVectorCommon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DayNightTriggerComponent.h"
#include "Math/Range.h"
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
	float nightAngleFromCenter;
	float dayAngleFromCenter;
	UPROPERTY(EditAnywhere)
	float activeHeight = 0;
	UPROPERTY(EditAnywhere)
	float inactiveHeight = 0;

	FVector lookDirection;
	FRotator celestialRotation;

	float sunActiveIntensity;
	float sunInactiveIntensity;

	float moonActiveIntensity;
	float moonInactiveIntensity;

	UPROPERTY(EditAnywhere)
	float rotationSpeed = 1;

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

	FRotator currentAngleFromCenter;
	FRotator targetAngleFromCenter;
	float sunCurrentIntensity;
	float moonCurrentIntensity;

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
	void MoveCelestials();

};
