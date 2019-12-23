// Fill out your copyright notice in the Description page of Project Settings.


#include "BurnTrigger.h"



ABurnTrigger::ABurnTrigger() 
{
	BurningArea = CreateDefaultSubobject<ATriggerBox>(TEXT("Trigger Area"));

}

void ABurnTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABurnTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
