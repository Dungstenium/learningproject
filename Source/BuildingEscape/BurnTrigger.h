// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "BurnTrigger.generated.h"

/**
 * 
 */
UCLASS()
class BUILDINGESCAPE_API ABurnTrigger : public ATriggerBox
{
	GENERATED_BODY()


public:
	ABurnTrigger();

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class ATriggerBox* BurningArea;
	//class UBoxComponent* BurningArea;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
