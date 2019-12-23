// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h" 
#include "Runtime/Renderer/Public/MeshDrawShaderBindings.h"


// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Outline = CreateDefaultSubobject<USphereComponent>(TEXT("Outline"));
	Outline->InitSphereRadius(100.f);
	RootComponent = Outline;
	Outline->SetSimulatePhysics(true);
	Outline->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	

	TheMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Esfera"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>
		Esfera(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));
	
	if (Esfera.Succeeded())
	{
		TheMesh->SetStaticMesh(Esfera.Object);
		TheMesh->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	}
	
	TheMesh->SetupAttachment(RootComponent);

	ParticulaFlamejante = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke"));
	ConstructorHelpers::FObjectFinder<UParticleSystem>
		Fumaca(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Smoke.P_Smoke'"));

	if (Fumaca.Succeeded())
	{
		ParticulaFlamejante->SetTemplate(Fumaca.Object);
	}

	ParticulaFlamejante->SetupAttachment(RootComponent);
	ParticulaFlamejante->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

