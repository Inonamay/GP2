// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightController.h"
#include "DayNightTriggerComponent.h"
// Sets default values for this component's properties
UDayNightController::UDayNightController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UDayNightController::AddTriggerComponent(UDayNightTriggerComponent* triggerToAdd)
{
	if (!triggers.Contains(triggerToAdd)) {
		triggers.Add(triggerToAdd);
	}
	
}
void UDayNightController::ToggleDayNight()
{
	if (state == Day) {
		state = Night;
	}
	else {
		state = Day;
	}
	UpdateTriggerComponents();
}
void UDayNightController::UpdateTriggerComponents() {
	for (size_t i = 0; i < triggers.Num(); i++)
	{
		triggers[i]->ChangeTime(state);
	}
}
void UDayNightController::SetTime(TimeState _state)
{
	state = _state;
	UpdateTriggerComponents();
}


