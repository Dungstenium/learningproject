// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class BUILDINGESCAPE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category = Item)
	class UStaticMeshComponent* Mesh;
};
