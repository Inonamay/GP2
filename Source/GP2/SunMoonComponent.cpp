// Fill out your copyright notice in the Description page of Project Settings.


#include "SunMoonComponent.h"

USunMoonComponent::USunMoonComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	if (!dayNightTriggerComponent)
		dayNightTriggerComponent = CreateDefaultSubobject<UDayNightTriggerComponent>("DayNightTriggerComponent");
}

void USunMoonComponent::BeginPlay() {
	Super::BeginPlay();
	sun->SetActorLocation(FVector(center->GetActorLocation().X + distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z + activeHeight));
	moon->SetActorLocation(FVector(center->GetActorLocation().X - distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z + inactiveHeight));
	nightAngleFromCenter = 180;
	dayAngleFromCenter = 0;

	dayNightTriggerComponent->onDay.AddDynamic(this, &USunMoonComponent::ToggleCelestials);
	dayNightTriggerComponent->onNight.AddDynamic(this, &USunMoonComponent::ToggleCelestials);

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("CelestialCenter")), celestialsArray);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Sun")), celestialsArray);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Moon")), celestialsArray);

}

void USunMoonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MoveCelestials();

}


void USunMoonComponent::ToggleCelestials(int dayNight) {
	if (dayNight == 0) {
		targetAngleFromCenter.Yaw = dayAngleFromCenter;
		dayOrNight = TestDay;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Day Current angle: %f, Target angle: %f"), center->GetActorRotation().Yaw, targetAngleFromCenter.Yaw));
	}
	else {
		targetAngleFromCenter.Yaw = nightAngleFromCenter;
		dayOrNight = TestNight;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Night Current angle: %f, Target angle: %f"), center->GetActorRotation().Yaw, targetAngleFromCenter.Yaw));
	}
}

void USunMoonComponent::MoveCelestials() {

	FRotator NewRotation = FRotator(0, rotationSpeed, 0);

	FQuat QuatRotation = FQuat(NewRotation);

	if (targetAngleFromCenter.Yaw == dayAngleFromCenter) {
		if (!FMath::IsWithin(center->GetActorRotation().Yaw, targetAngleFromCenter.Yaw - 0.1f, targetAngleFromCenter.Yaw + 0.1f)) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Current angle: %f, Target angle: %f"), center->GetActorRotation().Yaw, targetAngleFromCenter.Yaw));
			center->AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
			
		}
	}
	else if (targetAngleFromCenter.Yaw == nightAngleFromCenter) {
		if (!FMath::IsWithin(center->GetActorRotation().Yaw, targetAngleFromCenter.Yaw - 0.1f, targetAngleFromCenter.Yaw + 0.1f)) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Current angle: %f, Target angle: %f"), center->GetActorRotation().Yaw, targetAngleFromCenter.Yaw));
			center->AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
			
		}
	}
}
