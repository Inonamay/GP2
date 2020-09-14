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
	if (tracks.Num() == 0) {
		return;
	}
	if (!layerTrack) {
		layerTrack = UGameplayStatics::SpawnSound2D(this, layer, 1, 1, 0, nullptr, false, false);
		
	}
	if (activeTrack) {
		activeTrack->FadeOut(fadeTime, 0);
		trackToDestroy = activeTrack;
	}
	track = FMath::Clamp(track, 0, tracks.Num() - 1);
	activeTrack = UGameplayStatics::SpawnSound2D(this, tracks[track], 1, 1, 0, nullptr, false, false);
	activeTrack->Stop();
	activeTrack->FadeIn(fadeTime, 1, 0);
	layerTrack->Stop();
	layerTrack->FadeIn(fadeTime, 1, 0);
	layerTrack->SetVolumeMultiplier(layerVolume);
	
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


// Called when the game starts
void AMusicManager::BeginPlay()
{
	Super::BeginPlay();
	
}



