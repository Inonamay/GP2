// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Components/WalkableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
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

void APlayerCharacter::MoveToMapLocation(UWalkableComponent* location)
{
	this->SetActorLocation(location->GetOwner()->GetActorLocation());
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
	
}
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (codeClickToMove) {
		PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &APlayerCharacter::CheckForWalkable);
	}
	
	inputComponent = PlayerInputComponent;

}
void APlayerCharacter::CheckForWalkable()
{
	FHitResult HitRes;
	float x;
	float y;
	APlayerController* input = UGameplayStatics::GetPlayerController(this, 0);
	
	if (!input) {
		return;
	}
	input->GetMousePosition(x, y);
	FVector StartLocation;
	FVector direction;
	FVector EndLocation;
	input->DeprojectMousePositionToWorld(StartLocation, direction);
	EndLocation = StartLocation + direction * 1000;

	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FCollisionQueryParams CollisionParameters(FName(TEXT("TraceGround")), false, this);
	//FVector StartLocation = camManager->GetCameraLocation();
	//FVector EndLocation = camManager->GetCameraLocation() + camManager->GetActorForwardVector() * 20000;
	GetWorld()->LineTraceSingleByChannel(HitRes, StartLocation , EndLocation , ECollisionChannel::ECC_WorldStatic, CollisionParameters);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 2, 5, 1.f);
	//GetWorld()->LineTraceSingleByChannel(HitRes, camManager->GetCameraLocation(), camManager->GetCameraLocation() + camManager->GetActorForwardVector() * 100, ECC_Visibility);
	if (HitRes.Actor.IsValid()) {
		UWalkableComponent* walkable = HitRes.GetActor()->FindComponentByClass<UWalkableComponent>();
		if (walkable) {
			MoveToMapLocation(walkable);
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input


