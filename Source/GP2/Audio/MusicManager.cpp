// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
// Sets default values for this component's properties
AMusicManager::AMusicManager()
{

	// ...
}


void AMusicManager::AddLayer()
{
	addLayer++;
}

void AMusicManager::PlayAll()
{
	int a = activeSounds.Num();
	for (size_t i = 0; i < a; i++)
	{
		activeSounds[i]->Stop();
	}
	activeSounds.Empty();
	a = musicLayers.Num();
	for (size_t i = 0; i < a; i++)
	{
		activeSounds.Add(UGameplayStatics::SpawnSound2D(this, musicLayers[i], 1, 1, time, nullptr, false, false));
	}
}

void AMusicManager::PlayMultiple(int amount)
{
	addLayer += amount;
}

void AMusicManager::UpdateTiming()
{
	time += 0.1f;
	if (addLayer > 0) {
		if (layersActive < musicLayers.Num()) {
			activeSounds.Add(UGameplayStatics::SpawnSound2D(this, musicLayers[layersActive], 1, 1, time, nullptr, false, false));
			activeSounds[layersActive]->Stop();
			activeSounds[layersActive]->FadeIn(1, 1, time);
			layersActive++;
		}
		addLayer--;
	}
	
}

// Called when the game starts
void AMusicManager::BeginPlay()
{
	Super::BeginPlay();
	if (musicLayers.Num() > 0) {
		GetWorldTimerManager().SetTimer(timeLine, this, &AMusicManager::UpdateTiming, 0.1f, true);
	}
	// ...
	
}



