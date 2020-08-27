// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!dayNightComponent)
		dayNightComponent = CreateDefaultSubobject<UDayNightController>("DayNightController");

}
void APlayerCharacter::ChangeTimeOfDay(bool toggle, TimeState state)
{
	if (toggle) {
		dayNightComponent->ToggleDayNight();
	}
	else {
		dayNightComponent->SetTime(state);
	}
}

bool APlayerCharacter::ChangeActionPoints(int amount)
{
	if (currentActionPoints + amount < 0) {
		return false;
	}
	int clamped = FMath::Clamp(currentActionPoints + amount, 0, maxActionPoints);
	onPointChange.Broadcast(currentActionPoints, clamped);
	currentActionPoints = clamped;
	return true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

