// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_Granade.h"

// Sets default values
APickUp_Granade::APickUp_Granade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickUp_Granade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUp_Granade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

