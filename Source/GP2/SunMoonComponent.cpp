#include "SunMoonComponent.h"

USunMoonComponent::USunMoonComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	if (!dayNightTriggerComponent)
		dayNightTriggerComponent = CreateDefaultSubobject<UDayNightTriggerComponent>("DayNightTriggerComponent");
}

void USunMoonComponent::BeginPlay() {
	Super::BeginPlay();
	sun->SetActorLocation(FVector(center->GetActorLocation().X + distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z));
	moon->SetActorLocation(FVector(center->GetActorLocation().X - distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z));

	vActiveHeight.Z = activeHeight;
	vInactiveHeight.Z = inactiveHeight;

	dayRot = FRotator(0, dayAngle, 0);
	nightRot = FRotator(0, nightAngle, 0);

	sunTargetLocation.Z = sun->GetActorLocation().Z + vActiveHeight.Z;
	moonTargetLocation.Z = moon->GetActorLocation().Z + vInactiveHeight.Z;

	dayNightTriggerComponent->onDay.AddDynamic(this, &USunMoonComponent::ToggleCelestials);
	dayNightTriggerComponent->onNight.AddDynamic(this, &USunMoonComponent::ToggleCelestials);

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("CelestialCenter")), celestialsArray);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Sun")), celestialsArray);
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Moon")), celestialsArray);
		
	sun->SetActorRelativeRotation(FRotator(0, 180, 0));

}

void USunMoonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MoveCelestials(DeltaTime);
}


void USunMoonComponent::ToggleCelestials(int dayNight) {
	if (dayNight == 0) {
		targetRot.Yaw = dayRot.Yaw;
		startRot = center->GetActorRotation();
		dayOrNight = TestDay;
		time = 0;
		sunTargetLocation.Z = sun->GetActorLocation().Z + vActiveHeight.Z * 2;
		moonTargetLocation.Z = moon->GetActorLocation().Z + vInactiveHeight.Z * 2;
	}
	else if (dayNight == 1) {
		targetRot.Yaw = nightRot.Yaw;
		startRot = center->GetActorRotation();
		dayOrNight = TestNight;
		time = 0;
		sunTargetLocation.Z = sun->GetActorLocation().Z + vInactiveHeight.Z * 2;
		moonTargetLocation.Z = moon->GetActorLocation().Z + vActiveHeight.Z * 2;
	}

}

void USunMoonComponent::MoveCelestials(float DeltaTime) {

	if (dayOrNight == TestDay || dayOrNight == TestNight ) {
		center->SetActorRotation(FMath::RInterpConstantTo(center->GetActorRotation(), targetRot, DeltaTime, 360 / duration));

		sun->SetActorLocation(FMath::VInterpConstantTo(sun->GetActorLocation(), FVector(sun->GetActorLocation().X, sun->GetActorLocation().Y, sunTargetLocation.Z), DeltaTime, (activeHeight + -inactiveHeight) * 2 / duration));
		moon->SetActorLocation(FMath::VInterpConstantTo(moon->GetActorLocation(), FVector(moon->GetActorLocation().X, moon->GetActorLocation().Y, moonTargetLocation.Z), DeltaTime, (activeHeight + -inactiveHeight) * 2 / duration));
	}
}
