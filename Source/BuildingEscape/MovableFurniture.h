// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MovableFurniture.generated.h"

UCLASS()
class BUILDINGESCAPE_API AMovableFurniture : public ATriggerBox
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

protected:

	virtual void BeginPlay() override;

public:

	AMovableFurniture();

	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

};
