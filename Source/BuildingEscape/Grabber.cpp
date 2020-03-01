// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/PlayerCameraManager.h" 
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h" 
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector LineTraceEnd = PlayerPosition + PlayerViewPointRotation.Vector() * Reach;
	
	DrawDebugLine(GetWorld(), PlayerPosition, LineTraceEnd, FColor::Red, false, 0.0f, 0, 3.0f);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(TEXT(""), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerPosition,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	if (Hit.Actor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.Actor->GetName())
	}
}

