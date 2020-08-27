// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightTriggerComponent.h"
#include "../Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
// Sets default values for this component's properties
UDayNightTriggerComponent::UDayNightTriggerComponent()
{
	
}
APlayerCharacter* UDayNightTriggerComponent::player;
void UDayNightTriggerComponent::ChangeTime(TimeState state) {
	if (state == Day) {
		onDay.Broadcast(0);
	}
	else {
		onNight.Broadcast(1);
	}
}
void UDayNightTriggerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (player != nullptr) {
		player = nullptr;
	}
}
// Called when the game starts
void UDayNightTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	if (player == nullptr) {
		ACharacter* inWorldPlayer = UGameplayStatics::GetPlayerCharacter(GetOwner(), 0);
		if (inWorldPlayer) {
			APlayerCharacter* castTarget = Cast<APlayerCharacter>(inWorldPlayer);
			if (!castTarget) {
				return;
			}
			player = castTarget;
		}
		else {
			return;
		}
	}
	player->GetTimeController()->AddTriggerComponent(this);

	// ...
	
}



