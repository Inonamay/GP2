// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightTriggerComponent.h"
#include "../Characters/PlayerCharacter.h"
#include "../GP2GameModeBase.h"
#include "Kismet/GameplayStatics.h"
// Sets default values for this component's properties
UDayNightTriggerComponent::UDayNightTriggerComponent()
{
	
}
void UDayNightTriggerComponent::ChangeTime(TimeState state) {
	if (state == Day) {
		onDay.Broadcast(0);
	}
	else {
		onNight.Broadcast(1);
	}
}
// Called when the game starts
void UDayNightTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
    AGameModeBase* mode = UGameplayStatics::GetGameMode(GetOwner());
		if (mode) {
			AGP2GameModeBase* castTarget = Cast<AGP2GameModeBase>(mode);
			if (!castTarget) {
				return;
			}
			castTarget->AddTriggerComponent(this);
		}
		else {
			return;
		}

	// ...
	
}



