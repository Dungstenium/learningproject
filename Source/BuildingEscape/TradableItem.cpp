// Fill out your copyright notice in the Description page of Project Settings.


#include "TradableItem.h"
#include "MoneyManager.h"
#include "Components/BoxComponent.h"

ATradableItem::ATradableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	BuyPrice = { 0.0f };
	SellPrice = { 0.0f };

	GrabArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Grip Range"));
	RootComponent = GrabArea;
	GrabArea->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	GrabArea->SetCollisionProfileName(TEXT("Trigger"));
}

