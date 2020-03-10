#include "Engine/StaticMesh.h" 


#include "GameFramework/Actor.h" 
#include "MovableFurniture.h"

AMovableFurniture::AMovableFurniture()
{
    OnActorBeginOverlap.AddDynamic(this, &AMovableFurniture::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AMovableFurniture::OnOverlapEnd);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    StaticMesh->SetupAttachment(RootComponent);
}

void AMovableFurniture::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    UE_LOG(LogTemp, Warning, TEXT("touched"))

    // check if Actors do not equal nullptr and that 
    if (OtherActor && (OtherActor != this))
    {
        UE_LOG(LogTemp, Warning, TEXT("touched"))
    }
}

void AMovableFurniture::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
    if (OtherActor && (OtherActor != this))
    {

    }
}

void AMovableFurniture::BeginPlay()
{
	Super::BeginPlay();


}
