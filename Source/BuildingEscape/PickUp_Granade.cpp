// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp_Granade.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
APickUp_Granade::APickUp_Granade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GranadeTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	GranadeTrigger->SetCollisionProfileName("Trigger");
	GranadeTrigger->InitBoxExtent(FVector(30.f, 30.f, 30.f));
	GranadeTrigger->SetVisibility(false);
	RootComponent = GranadeTrigger;

	GranadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Granade"));
	GranadeMesh->SetupAttachment(GranadeTrigger);
	GranadeMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	PickUpSound = CreateDefaultSubobject<UAudioComponent>(TEXT("PickUpSound"));
	PickUpSound->SetupAttachment(GranadeTrigger);
	PickUpSound->bAutoActivate = false;

	GranadeTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickUp_Granade::OnPickUpRange);
}

// Called when the game starts or when spawned
void APickUp_Granade::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickUp_Granade::OnPickUpRange(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		PickUpSound->Activate(true);
		PickUpSound->bStopWhenOwnerDestroyed = false;
		PickUpSound->Play(0.f);
		Destroy();
	}
}

// Called every frame
void APickUp_Granade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

