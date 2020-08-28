// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkableComponent.h"

// Sets default values for this component's properties
UWalkableComponent::UWalkableComponent()
{

}


// Called when the game starts
void UWalkableComponent::BeginPlay()
{
	Super::BeginPlay();
	for (size_t i = 0; i < connectedWalkableActors.Num(); i++)
	{
		UWalkableComponent* walkable = connectedWalkableActors[i]->FindComponentByClass<UWalkableComponent>();
		if (walkable) {
			if (!connectedWalkables.Contains(walkable)) {
				connectedWalkables.Add(walkable);
			}
			
		}
	}
	
	// ...
	
}
