// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "TradableItem.generated.h"

/**
 * 
 */
UCLASS()
class BUILDINGESCAPE_API ATradableItem : public AItem
{
	GENERATED_BODY()
	
public:
	ATradableItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float BuyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float SellPrice;
};
