// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Components/WalkableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../Algorithms/Pathfinder.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!dayNightComponent)
		dayNightComponent = CreateDefaultSubobject<UDayNightController>("DayNightController");
	
	

}
void APlayerCharacter::ChangeTimeOfDay(bool toggle, TimeState state)
{
	if (toggle) {
		dayNightComponent->ToggleDayNight();
	}
	else {
		dayNightComponent->SetTime(state);
	}
}

void APlayerCharacter::MoveToMapLocation(TArray<UWalkableComponent*> path)
{
	if (path.Num() == 0) {
		actionFailed.Broadcast(NoPath);
		return;
	}
	if (moveAutomaticly) {
		this->SetActorLocation(path[path.Num() - 1]->GetOwner()->GetActorLocation());
		DoAction(Pathfinder::actionPointsSpentLast);
	}
	else {
		TArray<AActor*> actorPath;
		for (size_t i = 0; i < path.Num(); i++)
		{
			actorPath.Add(path[i]->GetOwner());
		}
		onFoundPath.Broadcast(actorPath, Pathfinder::actionPointsSpentLast);
	}
	
}

void APlayerCharacter::MoveToPath(AActor* actor)
{
	UWalkableComponent* walkable = actor->FindComponentByClass<UWalkableComponent>();
	if (walkable) {
		currentTile = walkable;
	}
}

bool APlayerCharacter::DoAction(int pointsCost)
{
	if (currentActionPoints - pointsCost < 0) {
		return false;
	}
	int clamped = FMath::Clamp(currentActionPoints - pointsCost, 0, maxActionPoints);
	onPointChange.Broadcast(currentActionPoints, clamped);
	currentActionPoints = clamped;
	return true;
}

void APlayerCharacter::ReplenishActionPoints(int amount)
{
	currentActionPoints += amount;
	if (currentActionPoints > maxActionPoints) {
		currentActionPoints = maxActionPoints;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (startingTile) {
		UWalkableComponent* walkable = startingTile->FindComponentByClass<UWalkableComponent>();
		if (walkable) {
			currentTile = walkable;
		}
	}
	if (!currentTile) {
		FHitResult HitRes;
		FVector start = GetActorLocation();
		FVector end = start - GetActorUpVector() * 200;
		FCollisionQueryParams CollisionParameters(FName(TEXT("TraceGround")), false, this);
		GetWorld()->LineTraceSingleByChannel(HitRes, start, end, ECollisionChannel::ECC_WorldStatic, CollisionParameters);
		if (HitRes.Actor.IsValid()) {
			UWalkableComponent* walkable = HitRes.GetActor()->FindComponentByClass<UWalkableComponent>();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit: " + HitRes.GetActor()->GetFName().ToString()));
			if (walkable) {
			
				currentTile = walkable;

			}
		}
	}
	
}
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (codeClickToMove) {
		PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &APlayerCharacter::CheckForWalkable);
	}
	
	inputComponent = PlayerInputComponent;

}
void APlayerCharacter::CheckForWalkable()
{
	FHitResult HitRes;
	APlayerController* input = UGameplayStatics::GetPlayerController(this, 0);
	if (!input) {
		return;
	}
	input->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitRes);
	if (HitRes.Actor.IsValid()) {
		UWalkableComponent* walkable = HitRes.GetActor()->FindComponentByClass<UWalkableComponent>();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Hit: " + HitRes.GetActor()->GetFName().ToString()));
		if (walkable) {
			if (walkable->blocked) {
				return;
			}
			if (currentTile) {
				MoveToMapLocation(Pathfinder::FindPath(currentTile, walkable, currentActionPoints));
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("There is no current tile"));
			}
			
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input


