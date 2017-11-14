// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector Direction = GlobalTargetLocation - GlobalStartLocation;
		FVector Location = GetActorLocation();
		
		float JourneyLength = Direction.Size();
		float JourneyTravelled = (Location - GlobalStartLocation).Size();

		if (JourneyTravelled > JourneyLength)
		{
			FVector Swap = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = Swap;
		}

		FVector DirectionNormal = Direction.GetSafeNormal();

		Location += (Speed * DeltaTime * DirectionNormal);

		SetActorLocation(Location);
	}
}