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
	USoundCue* layer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
		TArray<USoundCue*> tracks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
		float fadeTime = 1;
	UFUNCTION(Blueprintcallable)
		void SwitchTrack(int track);
	UFUNCTION(Blueprintcallable)
		void AddLayer();
	UFUNCTION(Blueprintcallable)
		void RemoveLayer();
protected:
	float layerVolume = 0;
	TArray<UAudioComponent*> activeSounds;
	UAudioComponent* activeTrack;
	UAudioComponent* layerTrack;
	UAudioComponent* trackToDestroy;
	virtual void BeginPlay() override;
	float time;
	int layersActive = 0;
	float musicDuration;
	int addLayer = 0;
	FTimerHandle timeLine;
public:	
	// Called every frame

		
};
