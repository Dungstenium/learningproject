// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUp_Granade.generated.h"

UCLASS()
class BUILDINGESCAPE_API APickUp_Granade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUp_Granade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Granade)
	class UStaticMeshComponent* GranadeMesh;

	UPROPERTY(VisibleAnywhere, Category = Granade)
	class UBoxComponent* GranadeTrigger;

	UPROPERTY(VisibleAnywhere, Category = Granade)
	class UAudioComponent* PickUpSound;

	UFUNCTION()
	void OnPickUpRange(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
