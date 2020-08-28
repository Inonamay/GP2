// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Components/WalkableComponent.h"
/**
 * 
 */
struct Node
{
public:
	int stepsTaken;
	UWalkableComponent* currentPosition;
	Node* previous;

};
class GP2_API Pathfinder
{

public:
	Pathfinder();
	static TArray<UWalkableComponent*> FindPath(UWalkableComponent* start, UWalkableComponent* end);
	~Pathfinder();
};
