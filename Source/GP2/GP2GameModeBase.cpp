// Copyright Epic Games, Inc. All Rights Reserved.


#include "GP2GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacter.h"
#include "Components/WalkableComponent.h"
#include "Interfaces/IUndoable.h"

void AGP2GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* playerChar = UGameplayStatics::GetPlayerCharacter(this, 0);
	player = Cast<APlayerCharacter>(playerChar);
	if (!player) {
		return;
	}
	timeController = player->GetTimeController();
}

void AGP2GameModeBase::RecordAction()
{
	FAction* newAction = new FAction();
	newAction->currentTile = player->currentTile;
	newAction->timeState = timeController->GetState();
	playerActions.Add(newAction);
}

void AGP2GameModeBase::AddAdditionalRedo(TArray<AActor*> actors)
{
	const int lastAction = playerActions.Num() - 1;
	const int actorsToAdd = actors.Num();
	for (size_t i = 0; i < actorsToAdd; i++)
	{
		playerActions[lastAction]->additionalActions.Add(actors[i]);
	}
	
}

void AGP2GameModeBase::Redo(int stepsAmount)
{
	const int actionIndex = playerActions.Num() - (stepsAmount + 1);
	const FAction* revertAction = playerActions[actionIndex];
	timeController->SetTime(revertAction->timeState);
	player->currentTile = revertAction->currentTile;
	UndoPlayerMove.Broadcast(revertAction->currentTile->GetOwner());
	const int extraActors = revertAction->additionalActions.Num();
	AActor* object;
	IIUndoable* undoable;
	for (size_t i = 0; i < extraActors; i++)
	{
		object = revertAction->additionalActions[i];
		if (object->GetClass()->ImplementsInterface(UIUndoable::StaticClass())) {
			undoable = Cast<IIUndoable>(object);
			undoable->Undo();
		}
	}

}
