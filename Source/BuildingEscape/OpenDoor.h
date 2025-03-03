// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

private:

	UPROPERTY(EditAnywhere)
		float DoorOpenSpeed{ 45.0f };
	UPROPERTY(EditAnywhere)
		float DoorMoveDistance{ 90.0f };

	UPROPERTY(EditAnywhere)
		bool bShouldMoveX{ false };
	UPROPERTY(EditAnywhere)
		bool bShouldMoveY{ false };
	UPROPERTY(EditAnywhere)
		bool bShouldMoveZ{ false };

	UPROPERTY(EditAnywhere)
		class ATriggerVolume* DoorTrigger;

	AActor* ActorThatOpens;

	FVector DoorActualLocation;
	FVector DoorStartingLocation;

	bool bPlayerLeftTrigger{ true };
	float TimeSincePlayerLeftTrigger{ 0.0f };

	UPROPERTY(EditAnywhere)
		float DoorDelayToClose;
};
