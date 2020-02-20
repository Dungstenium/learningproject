// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework//Actor.h"
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	DoorStartingLocation = GetOwner()->GetActorLocation();
	DoorActualLocation = GetOwner()->GetActorLocation();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	OpenDoor(DeltaTime);
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

