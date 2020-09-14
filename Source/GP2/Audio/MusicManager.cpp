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


void AMusicManager::SwitchTrack(int track)
{
	if (activeSounds.Num() == 0) {
		return;
	}
	if (tracks.Num() == 0) {
		return;
	}
	if (activeTrack) {
		activeTrack->SetVolumeMultiplier(0);
	}
	track = FMath::Clamp(track, 0, activeSounds.Num() - 1);
	activeTrack = activeSounds[track];
	activeTrack->SetVolumeMultiplier(1);
	
}

void AMusicManager::AddLayer()
{
	layerVolume = 1;
	layerTrack->SetVolumeMultiplier(layerVolume);
}

void AMusicManager::RemoveLayer()
{
	layerVolume = 0;
	layerTrack->SetVolumeMultiplier(layerVolume);
}

void AMusicManager::StartMusic()
{
	int trackNumber = tracks.Num();
	if (trackNumber == 0) {
		return;
	}
	if (activeSounds.Num() == 0) {
		for (size_t i = 0; i < trackNumber; i++)
		{
			activeSounds.Add(UGameplayStatics::SpawnSound2D(this, tracks[trackNumber], 0, 1, 0, nullptr, false, false));
		}
		layerTrack = UGameplayStatics::SpawnSound2D(this, layer, 0, 1, 0, nullptr, false, false);
	}
	else {
		if (activeTrack) {
			activeTrack->SetVolumeMultiplier(0);
		}
		for (size_t i = 0; i < activeSounds.Num(); i++)
		{
			activeSounds[i]->Play();
			activeSounds[i]->SetVolumeMultiplier(0);

		}
		if (layerTrack) {
			layerTrack->Play();
		}
	}
	activeTrack = activeSounds[0];
	activeTrack->SetVolumeMultiplier(1);
	
}

void AMusicManager::StopMusic()
{
	if (activeTrack) {
		activeTrack->SetVolumeMultiplier(0);
	}
	for (size_t i = 0; i < activeSounds.Num(); i++)
	{
		activeSounds[i]->Stop();
	}
	if (layerTrack) {
		layerTrack->Stop();
	}
	
}


// Called when the game starts
void AMusicManager::BeginPlay()
{
	Super::BeginPlay();
	
}



