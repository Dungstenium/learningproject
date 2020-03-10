// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	

	UGrabber();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FHitResult GetFirstPhysicsBodyInReach() const;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	float Reach{ 200.0f };
	UPROPERTY(EditAnywhere)
		float Damping{ 1.0f };
	UPROPERTY(EditAnywhere)
		float Stiffness{ 1.0f };
	UPROPERTY(EditAnywhere)
		float Speed{ 1.0f };



	class UPhysicsHandleComponent* PhysicsHandle{ nullptr };

	class UInputComponent* InputComponent{ nullptr };

	void SetupInputComponent();
	void FindPhysicsHandle();

	void StartPushingFurniture();
	void StopPushingFurniture();
};
