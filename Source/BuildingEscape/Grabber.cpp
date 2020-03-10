#include "Grabber.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/InputComponent.h" 
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h" 
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::StartPushingFurniture);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::StopPushingFurniture);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Grabber component of the object %s couldn't find the InputComponent"), *GetOwner()->GetName())
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("The Grabber component of the object %s couldn't find the PhysicsHandle"), *GetOwner()->GetName())
	}
}

void UGrabber::StartPushingFurniture()
{
	UE_LOG(LogTemp, Error, TEXT("Start Pushing working"))
	
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector LineTraceEnd = PlayerPosition + PlayerViewPointRotation.Vector() * Reach;

	FHitResult GrabResult = GetFirstPhysicsBodyInReach();

	if (GrabResult.GetActor())
	{
			UPrimitiveComponent* ComponentToGrab = GrabResult.GetComponent();

			//PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, LineTraceEnd);
			PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, LineTraceEnd, GrabResult.GetActor()->GetActorRotation());
			PhysicsHandle->SetLinearDamping(Damping);
			PhysicsHandle->SetLinearStiffness(Stiffness);
			PhysicsHandle->SetInterpolationSpeed(Speed);
	}
}

void UGrabber::StopPushingFurniture()
{
	UE_LOG(LogTemp, Error, TEXT("Stop Pushing working"))

	PhysicsHandle->ReleaseComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotation);

		FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		FVector LineTraceEnd = PlayerPosition + PlayerViewPointRotation.Vector() * Reach;

		PhysicsHandle->SetTargetLocation(LineTraceEnd);			
	}

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector LineTraceEnd = PlayerPosition + PlayerViewPointRotation.Vector() * Reach;

	DrawDebugLine(GetWorld(), PlayerPosition, LineTraceEnd, FColor::Red, false, 4.0f, 0, 3.0f);

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

	return Hit;
}
