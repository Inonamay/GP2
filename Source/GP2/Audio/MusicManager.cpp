// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
// Sets default values for this component's properties
UMusicManager::UMusicManager()
{

	// ...
}


void UMusicManager::AddLayer()
{
	if (activeSounds.Num() == 0) {
		return;
	}
	if (layersActive < musicLayers.Num()) {
		activeSounds.Add(UGameplayStatics::SpawnSound2D(this, musicLayers[layersActive], 1, 1, 0, nullptr, false, false));
		activeSounds[layersActive]->Stop();
		float delay = musicLayers[layersActive]->Duration * percentage;
		activeSounds[layersActive]->FadeIn(1, 1, delay);
		layersActive++;
	}
}

void UMusicManager::UpdateTiming(const USoundWave* wave, const float percent)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), percent));
	percentage = percent;
}

// Called when the game starts
void UMusicManager::BeginPlay()
{
	Super::BeginPlay();
	if (musicLayers.Num() > 0) {
		activeSounds.Add(UGameplayStatics::SpawnSound2D(this, musicLayers[0], 1, 1, 0, nullptr, false, false));
		activeSounds[0]->Stop();
		activeSounds[0]->OnAudioPlaybackPercent.AddDynamic(this, &UMusicManager::UpdateTiming);
		activeSounds[0]->Play();
		layersActive = 1;
	}
	// ...
	
}


