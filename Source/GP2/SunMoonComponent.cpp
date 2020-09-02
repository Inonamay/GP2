#include "SunMoonComponent.h"

USunMoonComponent::USunMoonComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	if (!dayNightTriggerComponent)
		dayNightTriggerComponent = CreateDefaultSubobject<UDayNightTriggerComponent>("DayNightTriggerComponent");
}

void USunMoonComponent::BeginPlay() {
	Super::BeginPlay();
	
	vActiveHeight.Z = activeHeight;
	vInactiveHeight.Z = inactiveHeight;

	dayRot = FRotator(0, dayAngle, 0);
	nightRot = FRotator(0, nightAngle, 0);


	dayNightTriggerComponent->onDay.AddDynamic(this, &USunMoonComponent::ToggleCelestials);
	dayNightTriggerComponent->onNight.AddDynamic(this, &USunMoonComponent::ToggleCelestials);

	GetOwner()->GetAttachedActors(celestialsArray);

	for (AActor* celestial : celestialsArray) {
		if(celestial->GetName() == "Sun")
			sun = celestial;
		if (celestial->GetName() == "Moon")
			moon = celestial;
	}

	if (sun != nullptr && moon != nullptr) {
		sun->GetAttachedActors(lightArray, false);
		moon->GetAttachedActors(lightArray, false);
	}

	for (AActor* lightSource : lightArray) {
		if (lightSource->GetName() == "SunLight") {
			sunLightActor = lightSource;
		}
		if (lightSource->GetName() == "MoonLight") {
			moonLightActor = lightSource;
		}
	}

	if (sunLightActor != nullptr && moonLightActor != nullptr) {
		sunDirLight = sunLightActor->FindComponentByClass<UDirectionalLightComponent>();
		moonDirLight = moonLightActor->FindComponentByClass<UDirectionalLightComponent>();
	}

	sun->SetActorLocation(FVector(center->GetActorLocation().X + distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z));
	moon->SetActorLocation(FVector(center->GetActorLocation().X - distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z));

	sunTargetLocation.Z = sun->GetActorLocation().Z + vActiveHeight.Z;
	moonTargetLocation.Z = moon->GetActorLocation().Z + vInactiveHeight.Z;

	sun->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(sun->GetActorLocation(), center->GetActorLocation()));
	moon->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(moon->GetActorLocation(), center->GetActorLocation()));

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

		sunTargetIntensity = sunActiveIntensity;
		moonTargetIntensity = moonInactiveIntensity;
	}
	else if (dayNight == 1) {
		targetRot.Yaw = nightRot.Yaw;
		startRot = center->GetActorRotation();
		dayOrNight = TestNight;
		time = 0;

		sunTargetLocation.Z = sun->GetActorLocation().Z + vInactiveHeight.Z * 2;
		moonTargetLocation.Z = moon->GetActorLocation().Z + vActiveHeight.Z * 2;

		sunTargetIntensity = sunInactiveIntensity;
		moonTargetIntensity = moonActiveIntensity;
	}

}

void USunMoonComponent::MoveCelestials(float DeltaTime) {

	if (dayOrNight == TestDay || dayOrNight == TestNight ) {
		center->SetActorRotation(FMath::RInterpConstantTo(center->GetActorRotation(), targetRot, DeltaTime, 360 / duration));

		sun->SetActorLocation(FMath::VInterpConstantTo(sun->GetActorLocation(), FVector(sun->GetActorLocation().X, sun->GetActorLocation().Y, sunTargetLocation.Z), DeltaTime, (activeHeight + -inactiveHeight) * 2 / duration));
		moon->SetActorLocation(FMath::VInterpConstantTo(moon->GetActorLocation(), FVector(moon->GetActorLocation().X, moon->GetActorLocation().Y, moonTargetLocation.Z), DeltaTime, (activeHeight + -inactiveHeight) * 2 / duration));
		
		sunDirLight->SetIntensity(FMath::FInterpConstantTo(sunDirLight->Intensity, sunTargetIntensity, DeltaTime, (sunActiveIntensity + sunInactiveIntensity) * 2 / duration));
		moonDirLight->SetIntensity(FMath::FInterpConstantTo(moonDirLight->Intensity, moonTargetIntensity, DeltaTime, (moonActiveIntensity + moonInactiveIntensity) * 2 / duration));
	}
}
