// Fill out your copyright notice in the Description page of Project Settings.


#include "MoneyManager.h"

// Sets default values for this component's properties
UMoneyManager::UMoneyManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	MoneyAmount = { 100 };
	// ...
}


// Called when the game starts
void UMoneyManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMoneyManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UMoneyManager::GetMoney() const
{
	return MoneyAmount;
}
void UMoneyManager::SetMoney(float Value)
{
	MoneyAmount = Value;
}
void UMoneyManager::AddMoney(float Quantity)
{
	MoneyAmount += Quantity;
}
void UMoneyManager::SpendMoney(float Quantity)
{
	MoneyAmount -= Quantity;
}
