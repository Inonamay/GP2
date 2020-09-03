// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightController.h"
#include "Kismet/GameplayStatics.h"
#include "../GP2GameModeBase.h"
#include "DayNightTriggerComponent.h"
// Sets default values for this component's properties
UDayNightController::UDayNightController()
{
}



void UDayNightController::SetTime(TimeState _state)
{
	AGameModeBase* mode = UGameplayStatics::GetGameMode(this);
	if (mode) {
		AGP2GameModeBase* castTarget = Cast<AGP2GameModeBase>(mode);
		if (!castTarget) {
			return;
		}
		castTarget->SetTime(_state);
	}
}

TimeState UDayNightController::GetState()
{
	AGameModeBase* mode = UGameplayStatics::GetGameMode(this);
	if (mode) {
		AGP2GameModeBase* castTarget = Cast<AGP2GameModeBase>(mode);
		if (!castTarget) {
			return TimeState();
		}
		return castTarget->GetState();
	}
	return TimeState();
}


