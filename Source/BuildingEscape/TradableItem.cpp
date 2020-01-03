// Fill out your copyright notice in the Description page of Project Settings.


#include "TradableItem.h"

ATradableItem::ATradableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	BuyPrice = { 0.0f };
	SellPrice = { 0.0f };
}
