// Fill out your copyright notice in the Description page of Project Settings.

#include "BurningTrigger.h"
#include "Components/BoxComponent.h"
#include "Runtime/Renderer/Public/MeshDrawShaderBindings.h"
#include "UObject/ConstructorHelpers.h" 
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABurningTrigger::ABurningTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BurningArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	BurningArea->InitBoxExtent(FVector(100.f,100.f,100.f));
	BurningArea->SetCollisionProfileName(TEXT("Trigger"));
	BurningArea->SetupAttachment(RootComponent);
	BurningArea->OnComponentBeginOverlap.AddDynamic(this, &ABurningTrigger::FireBuildUp);

	FireBurning = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Effects"));
	FireBurning->SetupAttachment(BurningArea);
	FireBurning->SetRelativeLocation(FVector(0, 0, -80));
	FireBurning->SetRelativeScale3D(FVector(3, 3, 3));
	ConstructorHelpers::FObjectFinder<UParticleSystem>
		Fire(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));

	if (Fire.Succeeded())
	{
		FireBurning->SetTemplate(Fire.Object);
	}
	FireBurning->bAutoActivate = true;

	FloatingText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	FloatingText->Activate(true);
	FloatingText->SetText(TEXT("Burning Area"));
	FloatingText->SetHorizontalAlignment(EHTA_Center);
	FloatingText->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	FloatingText->SetupAttachment(BurningArea);
	FloatingText->SetTextRenderColor(FColor::Yellow);
	FloatingText->SetXScale(2.f);
	FloatingText->SetYScale(2.f);

	if (FloatingText || FireBurning || BurningArea == nullptr)
	{
		return;
	}
}

// Called when the game starts or when spawned
void ABurningTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABurningTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABurningTrigger::FireBuildUp(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Burnable")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		//OtherActor->GetComponents<UParticleSystemComponent>()
		Cast<UParticleSystemComponent>(OtherActor->GetComponentByClass
			(UParticleSystemComponent::StaticClass()))->Activate(true);
		Cast<UParticleSystemComponent>(OtherActor->GetComponentByClass
		(UParticleSystemComponent::StaticClass()))->bAutoActivate = true;
	}
}

