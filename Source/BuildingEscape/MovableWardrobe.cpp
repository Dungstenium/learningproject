// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableWardrobe.h"
#include "Engine/StaticMesh.h" 
#include "Components/BoxComponent.h" 

// Sets default values
AMovableWardrobe::AMovableWardrobe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    //OnActorBeginOverlap.AddDynamic(this, &AMovableWardrobe::OnOverlapBegin);
    //OnActorEndOverlap.AddDynamic(this, &AMovableWardrobe::OnOverlapEnd);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    
    RootComponent = StaticMesh;

    Trigger = CreateDefaultSubobject<UBoxComponent>("Tigger Box");
    Trigger->SetCollisionProfileName(TEXT("Trigger"));
    Trigger->SetupAttachment(RootComponent);
    Trigger->SetRelativeLocation(FVector(0, 0, 0));
}

// Called when the game starts or when spawned
void AMovableWardrobe::BeginPlay()
{
	Super::BeginPlay();

    StaticMesh->BodyInstance.bLockXTranslation = true;

    Trigger->SetRelativeLocation(FVector(0, 0, 0));

    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMovableWardrobe::OnOverlapBegin);
    Trigger->OnComponentEndOverlap.AddDynamic(this, &AMovableWardrobe::OnOverlapEnd);
}

// Called every frame
void AMovableWardrobe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovableWardrobe::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // check if Actors do not equal nullptr
    if (OtherActor && (OtherActor != this))
    {
        UE_LOG(LogTemp, Warning, TEXT("We Began"));
        StaticMesh->BodyInstance.bLockXTranslation = false;
    }
}

void AMovableWardrobe::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && (OtherActor != this))
    {
        UE_LOG(LogTemp, Warning, TEXT("We Ended"));
        StaticMesh->BodyInstance.bLockXTranslation = true;
    }
}


