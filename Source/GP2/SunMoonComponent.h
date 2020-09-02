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
#include "Components/DirectionalLightComponent.h"
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

	UPROPERTY(EditAnywhere)
	float duration = 2;

	AActor* sunLightActor;
	AActor* moonLightActor;

	UDirectionalLightComponent* sunDirLight;
	UDirectionalLightComponent* moonDirLight;

	UPROPERTY(EditAnywhere)
	float sunActiveIntensity = 10;
	UPROPERTY(EditAnywhere)
	float sunInactiveIntensity = 0;
	UPROPERTY(EditAnywhere)
	float moonActiveIntensity = 10;
	UPROPERTY(EditAnywhere)
	float moonInactiveIntensity = 0;

	bool toggle;

	UPROPERTY(EditAnywhere)
	AActor* center;
	APawn* playerCharacter;
	AActor* sun;
	AActor* moon;

	TEnumAsByte<DayNight> dayOrNight;

private:

	TArray<AActor*> celestialsArray;
	TArray<AActor*> lightArray;
	TArray<UDirectionalLightComponent*> dirLightArray;

	float sunCurrentIntensity;
	float moonCurrentIntensity;
	float sunTargetIntensity;
	float moonTargetIntensity;

	float dayAngle = 0;
	float nightAngle = 180;
	float time = 0;

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
