// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "MusicManager.generated.h"
class USoundCue;
class UAudioComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP2_API AMusicManager : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AMusicManager();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	TArray<USoundCue*> musicLayers;
	UFUNCTION(Blueprintcallable)
		void AddLayer();
	UFUNCTION(Blueprintcallable)
		void PlayAll();
	UFUNCTION(Blueprintcallable)
		void PlayMultiple(int amount);
protected:
	// Called when the game starts
	void UpdateTiming();
	TArray<UAudioComponent*> activeSounds;
	virtual void BeginPlay() override;
	float time;
	int layersActive = 0;
	float musicDuration;
	int addLayer = 0;
	FTimerHandle timeLine;
public:	
	// Called every frame

		
};
