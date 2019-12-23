// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "BurningTrigger.generated.h"

UCLASS()
class BUILDINGESCAPE_API ABurningTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABurningTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Burning Area")
	class UBoxComponent* BurningArea;

	UPROPERTY(VisibleAnywhere, Category = "Burning Area")
	class UParticleSystemComponent* FireBurning;

	UPROPERTY(VisibleAnywhere, Category = "Text Pop-Up")
	class UTextRenderComponent* FloatingText;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void FireBuildUp(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	//UFUNCTION()
	//void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	//	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
