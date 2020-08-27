// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WalkableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP2_API UWalkableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWalkableComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walkable")
	bool blocked;
	int Cost() { return pointsCost; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walkable")
		int pointsCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walkable")
		TArray<UWalkableComponent*> connectedWalkables;
public:	

		
};
