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

	sunTargetLocation.Z = center->GetActorLocation().Z + vActiveHeight.Z;
	moonTargetLocation.Z = center->GetActorLocation().Z + vInactiveHeight.Z;

	sun->SetActorLocation(FVector(center->GetActorLocation().X + distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z + vActiveHeight.Z));
	moon->SetActorLocation(FVector(center->GetActorLocation().X - distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z + vInactiveHeight.Z));

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

		sunTargetLocation.Z = center->GetActorLocation().Z + vActiveHeight.Z;
		moonTargetLocation.Z = center->GetActorLocation().Z + vInactiveHeight.Z;

		sunTargetIntensity = sunActiveIntensity;
		moonTargetIntensity = moonInactiveIntensity;
		sunTargetColor = sunActiveColor;
		moonTargetColor = moonInactiveColor;
	}
	else if (dayNight == 1) {
		targetRot.Yaw = nightRot.Yaw;

		sunTargetLocation.Z = center->GetActorLocation().Z + vInactiveHeight.Z;
		moonTargetLocation.Z = center->GetActorLocation().Z + vActiveHeight.Z;

		sunTargetIntensity = sunInactiveIntensity;
		moonTargetIntensity = moonActiveIntensity;
		sunTargetColor = sunInactiveColor;
		moonTargetColor = moonActiveColor;
	}

	time = 0;
	sunStartColor = sunDirLight->GetLightColor();
	moonStartColor = moonDirLight->GetLightColor();

	/*if (dayNight == 0) {
		activeCelestial = sun;
	}
	else {
		activeCelestial = moon;
	}
	for(AActor* celestial : celestialsArray) {
		targetRot.Yaw = dayRot.Yaw;
		startRot = center->GetActorRotation();

		time = 0;
		activeTargetLocation.Z = center->GetActorLocation().Z + vActiveHeight.Z;
		inactiveTargetLocation.Z = center->GetActorLocation().Z + vInactiveHeight.Z;

		activeTargetIntensity = activeCelestial.activeIntensity;
		inactiveTargetIntensity = activeCelestial.inactiveIntensity;*/

}

void USunMoonComponent::MoveCelestials(float DeltaTime) {
	//InterpConstant
	center->SetActorRotation(FMath::RInterpConstantTo(center->GetActorRotation(), targetRot, DeltaTime, 180 / duration));

	sun->SetActorLocation(FMath::VInterpConstantTo(sun->GetActorLocation(), FVector(sun->GetActorLocation().X, sun->GetActorLocation().Y, sunTargetLocation.Z), DeltaTime, (activeHeight + abs(inactiveHeight)) / duration));
	moon->SetActorLocation(FMath::VInterpConstantTo(moon->GetActorLocation(), FVector(moon->GetActorLocation().X, moon->GetActorLocation().Y, moonTargetLocation.Z), DeltaTime, (activeHeight + abs(inactiveHeight)) / duration));
		
	sunDirLight->SetIntensity(FMath::FInterpConstantTo(sunDirLight->Intensity, sunTargetIntensity, DeltaTime, (sunActiveIntensity + sunInactiveIntensity)  / duration));
	moonDirLight->SetIntensity(FMath::FInterpConstantTo(moonDirLight->Intensity, moonTargetIntensity, DeltaTime, (moonActiveIntensity + moonInactiveIntensity)  / duration));

	//Lerp
	if (duration > time) {
		sunDirLight->SetLightColor(FMath::Lerp(sunStartColor, sunTargetColor, colorCurve->GetFloatValue(time / duration)));
		moonDirLight->SetLightColor(FMath::Lerp(moonStartColor, moonTargetColor, colorCurve->GetFloatValue(time / duration)));
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Yellow, FString::Printf(TEXT("curve is at: %f"), colorCurve->GetFloatValue(time / duration)));
		time += DeltaTime;
	}
}
