// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightController.h"
#include "DayNightTriggerComponent.h"
// Sets default values for this component's properties
UDayNightController::UDayNightController()
{
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


