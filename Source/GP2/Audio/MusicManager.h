// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "MusicManager.generated.h"
class USoundCue;
class UAudioComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP2_API UMusicManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMusicManager();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	TArray<USoundCue*> musicLayers;
	UFUNCTION(Blueprintcallable)
		void AddLayer();
protected:
	// Called when the game starts
	void UpdateTiming(const USoundWave* wave, const float percent);
	TArray<UAudioComponent*> activeSounds;
	virtual void BeginPlay() override;
	float percentage;
	int layersActive = 0;

public:	
	// Called every frame

		
};
