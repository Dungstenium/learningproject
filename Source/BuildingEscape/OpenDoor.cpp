// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/TriggerVolume.h"
#include "GameFramework/PlayerController.h" 
#include "Engine/World.h" 
#include "GameFramework//Actor.h"
#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	DoorStartingLocation = GetOwner()->GetActorLocation();
	DoorActualLocation = GetOwner()->GetActorLocation();

	if (ActorThatOpens == NULL)
	{
		ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (DoorTrigger->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		bPlayerLeftTrigger = false;
		TimeSincePlayerLeftTrigger = 0;
	}
	else
	{
		bPlayerLeftTrigger = true;
		TimeSincePlayerLeftTrigger += DeltaTime;

		if (bPlayerLeftTrigger && TimeSincePlayerLeftTrigger >= DoorDelayToClose)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	if (bShouldMoveX && DoorActualLocation.X != DoorStartingLocation.X - DoorMoveDistance)
	{
		DoorActualLocation.X = FMath::FInterpConstantTo(DoorActualLocation.X,
			DoorStartingLocation.X - DoorMoveDistance, DeltaTime, DoorOpenSpeed);
	}
	else if (bShouldMoveY && DoorActualLocation.Y != DoorStartingLocation.Y - DoorMoveDistance)
	{
		DoorActualLocation.Y = FMath::FInterpConstantTo(DoorActualLocation.Y,
			DoorStartingLocation.Y - DoorMoveDistance, DeltaTime, DoorOpenSpeed);
	}
	else if (bShouldMoveZ && DoorActualLocation.Z != DoorStartingLocation.Z - DoorMoveDistance)
	{
		DoorActualLocation.Z = FMath::FInterpConstantTo(DoorActualLocation.Z,
			DoorStartingLocation.Z - DoorMoveDistance, DeltaTime, DoorOpenSpeed);
	}

	GetOwner()->SetActorLocation(DoorActualLocation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	if (bShouldMoveX)
	{
		DoorActualLocation.X = FMath::FInterpConstantTo(DoorActualLocation.X,
			DoorStartingLocation.X, DeltaTime, DoorOpenSpeed);
	}
	else if (bShouldMoveY)
	{
		DoorActualLocation.Y = FMath::FInterpConstantTo(DoorActualLocation.Y,
			DoorStartingLocation.Y, DeltaTime, DoorOpenSpeed);
	}
	else if (bShouldMoveZ)
	{
		DoorActualLocation.Z = FMath::FInterpConstantTo(DoorActualLocation.Z,
			DoorStartingLocation.Z, DeltaTime, DoorOpenSpeed);
	}

	GetOwner()->SetActorLocation(DoorActualLocation);
}

