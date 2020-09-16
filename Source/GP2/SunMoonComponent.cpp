#include "SunMoonComponent.h"

USunMoonComponent::USunMoonComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	if (!dayNightTriggerComponent)
		dayNightTriggerComponent = CreateDefaultSubobject<UDayNightTriggerComponent>("DayNightTriggerComponent");

	
}

void USunMoonComponent::BeginPlay() {
	Super::BeginPlay();

	vSunActiveHeight.Z = sunActiveHeight;
	vMoonActiveHeight.Z = moonActiveHeight;
	vInactiveHeight.Z = inactiveHeight;

	dayRot = FRotator(0, dayAngle, 0);
	nightRot = FRotator(0, nightAngle, 0);

	dayNightTriggerComponent->onDay.AddDynamic(this, &USunMoonComponent::ToggleCelestials);
	dayNightTriggerComponent->onNight.AddDynamic(this, &USunMoonComponent::ToggleCelestials);

	GetOwner()->GetAttachedActors(celestialsArray);

	for (AActor* celestial : celestialsArray) {
		if (celestial->GetName() == "Sun")
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

	sunTargetLocation.Z = center->GetActorLocation().Z + vSunActiveHeight.Z;
	moonTargetLocation.Z = center->GetActorLocation().Z + vInactiveHeight.Z;

	sun->SetActorLocation(FVector(center->GetActorLocation().X + distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z + vSunActiveHeight.Z));
	moon->SetActorLocation(FVector(center->GetActorLocation().X - distanceFromCenter, center->GetActorLocation().Y, center->GetActorLocation().Z + vInactiveHeight.Z));

	sun->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(sun->GetActorLocation(), center->GetActorLocation()));
	moon->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(moon->GetActorLocation(), center->GetActorLocation()));

	sunActiveIntensity = sunDirLight->Intensity;
	moonActiveIntensity = moonDirLight->Intensity;

	sunDirLight->SetIntensity(sunActiveIntensity);
	moonDirLight->SetIntensity(moonInactiveIntensity);

	sunActiveColor = sunDirLight->GetLightColor();
	moonActiveColor = moonDirLight->GetLightColor();
	moonDirLight->SetLightColor(moonInactiveColor);
}

void USunMoonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MoveCelestials(DeltaTime, bMoveCelestials);
}


void USunMoonComponent::ToggleCelestials(int dayNight) {
	if (dayNight == 0) {
		targetRot.Yaw = dayRot.Yaw;

		sunTargetLocation.Z = center->GetActorLocation().Z + vSunActiveHeight.Z;
		moonTargetLocation.Z = center->GetActorLocation().Z + vInactiveHeight.Z;

		sunTargetIntensity = sunActiveIntensity;
		moonTargetIntensity = moonInactiveIntensity;
		sunTargetColor = sunActiveColor;
		moonTargetColor = moonInactiveColor;

		dayOrNight = DayNight::CelestialDay;
	}
	else if (dayNight == 1) {
		targetRot.Yaw = nightRot.Yaw;

		sunTargetLocation.Z = center->GetActorLocation().Z + vInactiveHeight.Z;
		moonTargetLocation.Z = center->GetActorLocation().Z + vMoonActiveHeight.Z;

		sunTargetIntensity = sunInactiveIntensity;
		moonTargetIntensity = moonActiveIntensity;
		sunTargetColor = sunInactiveColor;
		moonTargetColor = moonActiveColor;

		dayOrNight = DayNight::CelestialNight;
	}

	time = 0;
	sunStartLocation = sun->GetActorLocation();
	moonStartLocation = moon->GetActorLocation();
	sunStartColor = sunDirLight->GetLightColor();
	moonStartColor = moonDirLight->GetLightColor();
	sunStartIntensity = sunDirLight->Intensity;
	moonStartIntensity = moonDirLight->Intensity;
	bMoveCelestials = true;
}

void USunMoonComponent::MoveCelestials(float DeltaTime, bool move) {
	
	if (move) {
		if (dayOrNight == 0 || dayOrNight == 1) {
			if (duration > time) {
				center->SetActorRotation(FMath::RInterpConstantTo(center->GetActorRotation(), targetRot, DeltaTime, 180 / duration));
				// Height interpolation
				if (heightCurve != nullptr) {
					sun->SetActorLocation(FMath::Lerp(FVector(sun->GetActorLocation().X, sun->GetActorLocation().Y, sunStartLocation.Z), FVector(sun->GetActorLocation().X, sun->GetActorLocation().Y, sunTargetLocation.Z), heightCurve->GetFloatValue(time / duration)));
					moon->SetActorLocation(FMath::Lerp(FVector(moon->GetActorLocation().X, moon->GetActorLocation().Y, moonStartLocation.Z), FVector(moon->GetActorLocation().X, moon->GetActorLocation().Y, moonTargetLocation.Z), heightCurve->GetFloatValue(time / duration)));
				}
				else {
					sun->SetActorLocation(FMath::VInterpConstantTo(sun->GetActorLocation(), FVector(sun->GetActorLocation().X, sun->GetActorLocation().Y, sunTargetLocation.Z), DeltaTime, (sunActiveHeight + abs(inactiveHeight)) / duration));
					moon->SetActorLocation(FMath::VInterpConstantTo(moon->GetActorLocation(), FVector(moon->GetActorLocation().X, moon->GetActorLocation().Y, moonTargetLocation.Z), DeltaTime, (moonActiveHeight + abs(inactiveHeight)) / duration));
				}

				// Intensity interpolation
				if (intensityCurve != nullptr) {
					sunDirLight->SetIntensity(FMath::Lerp(sunStartIntensity, sunTargetIntensity, intensityCurve->GetFloatValue(time / duration)));
					moonDirLight->SetIntensity(FMath::Lerp(moonStartIntensity, moonTargetIntensity, intensityCurve->GetFloatValue(time / duration)));
				}
				else {
					sunDirLight->SetIntensity(FMath::FInterpConstantTo(sunDirLight->Intensity, sunTargetIntensity, DeltaTime, (sunActiveIntensity + abs(sunInactiveIntensity)) / duration));
					moonDirLight->SetIntensity(FMath::FInterpConstantTo(moonDirLight->Intensity, moonTargetIntensity, DeltaTime, (moonActiveIntensity + abs(moonInactiveIntensity)) / duration));
				}

				// Color interpolation
				if (colorCurve != nullptr) {
					sunDirLight->SetLightColor(FMath::Lerp(sunStartColor, sunTargetColor, colorCurve->GetFloatValue(time / duration)));
					moonDirLight->SetLightColor(FMath::Lerp(moonStartColor, moonTargetColor, colorCurve->GetFloatValue(time / duration)));
				}
				else {
					sunDirLight->SetLightColor(FMath::Lerp(sunStartColor, sunTargetColor, time / duration));
					moonDirLight->SetLightColor(FMath::Lerp(moonStartColor, moonTargetColor, time / duration));
				}
				time += DeltaTime;
			}
			move = false;
		}
	}
}
