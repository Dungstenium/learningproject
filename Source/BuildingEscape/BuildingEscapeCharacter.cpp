// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BuildingEscapeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Renderer/Public/MeshDrawShaderBindings.h"
#include "GameFramework/Controller.h"
#include "Components/AudioComponent.h"
#include "MovableWardrobe.h"
#include "MoneyManager.h"
#include "Math/UnrealMathUtility.h" 
#include "Math/Vector.h" 
#include "Math/UnrealMathUtility.h" 
#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h" 
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ABuildingEscapeCharacter

ABuildingEscapeCharacter::ABuildingEscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	Timer = 0;
	bHasGranade = false;
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	ActorParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Effects"));
	ActorParticle->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UParticleSystem> 
		Fire(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
	
	if (Fire.Succeeded())
	{
		ActorParticle->SetTemplate(Fire.Object);
	}

	ActorParticle->bAutoActivate = false;

	BurnCounterText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BurnCountdown"));
	BurnCounterText->SetTextRenderColor(FColor::Red);
	BurnCounterText->SetupAttachment(RootComponent);
	BurnCounterText->SetRelativeLocationAndRotation(FVector(0.f, -30.f, 80.f), FRotator(0.f, 180.f, 0.f));
	BurnCounterText->bAutoActivate = false;
	BurnCounterText->bVisible = false;
	BurnCounterText->SetText(FString::SanitizeFloat(Timer));

	ConstructorHelpers::FObjectFinder<UAnimMontage> 
		DebuffAnimMontage(TEXT("AnimMontage'/Game/GenericNPCAnimPack/Animations/DebuffsMontage.DebuffsMontage'"));
	
	if (DebuffAnimMontage.Succeeded())
	{
		DebuffMontage = DebuffAnimMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>
		RollAnimMontage(TEXT("AnimMontage'/Game/RollsAndDodges/Animations/RollMontage.RollMontage'"));

	if (RollAnimMontage.Succeeded())
	{
		RollMontage = RollAnimMontage.Object;
	}

	BurningSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BurnSound"));
	BurningSound->SetupAttachment(RootComponent);
	BurningSound->bAutoActivate = false;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABuildingEscapeCharacter::EnterPickUpRange);

	//MoneyManager = CreateDefaultSubobject<UMoneyManager>(TEXT("Money Handler"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABuildingEscapeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &ABuildingEscapeCharacter::Roll);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABuildingEscapeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABuildingEscapeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABuildingEscapeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABuildingEscapeCharacter::LookUpAtRate);

	//// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ABuildingEscapeCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ABuildingEscapeCharacter::TouchStopped);

	// VR headset functionality
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ABuildingEscapeCharacter::OnResetVR);
}

void ABuildingEscapeCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABuildingEscapeCharacter::OnOverlapEnd);

}

void ABuildingEscapeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (ActorParticle->bAutoActivate == true)
	{
		StopBurning(DeltaTime);
	}
}
void ABuildingEscapeCharacter::StopBurning(float DeltaTime)
{
	Timer += DeltaTime;
	Timer = FMath::Clamp(Timer, 0.f, 5.f);

	BurnCounterText->Activate(true);
	BurnCounterText->bVisible = true;
	BurnCounterText->SetText(FString::SanitizeFloat(Timer));

	if (Timer<= 0.1f)
	{
		PlayAnimMontage(DebuffMontage, 1.f, "start01");
		BurningSound->Activate(true);
		BurningSound->PitchModulationMax = 1.2;
		BurningSound->PitchModulationMin = 0.9;
		BurningSound->bStopWhenOwnerDestroyed = false;
		BurningSound->Play(0.0f);
	}
	else if (Timer >= 2.5f && Timer <= 2.6f)
	{
		BurningSound->Activate(true);
		BurningSound->PitchModulationMax = 1.2;
		BurningSound->PitchModulationMin = 0.9;
		BurningSound->bStopWhenOwnerDestroyed = false;
		BurningSound->Play(0.0f);
	}

	if (Timer >= 5.f)
	{
		ActorParticle->Deactivate();
		ActorParticle->bAutoActivate = false;

		BurnCounterText->bVisible = false;
		Timer = 0;
	}
}

void ABuildingEscapeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

//void ABuildingEscapeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
//{
//		Jump();
////}
//
//void ABuildingEscapeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
//{
//		StopJumping();
//}

void ABuildingEscapeCharacter::EnterPickUpRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Interactable"))
	{
		if (!bHasGranade)
		{
			PlayAnimMontage(DebuffMontage, 1.f, "start02");
			bHasGranade = true;
		}
	}
	if (OtherActor->IsA<AMovableWardrobe>())
	{
		PlayAnimMontage(DebuffMontage, 1.f, "start03");
	}
}

void ABuildingEscapeCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMovableWardrobe>())
	{
		StopAnimMontage(DebuffMontage);
	}
}

void ABuildingEscapeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABuildingEscapeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABuildingEscapeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		UPawnMovementComponent* MovementComponent = GetMovementComponent();
		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//ActualSpeed = FMath::Lerp(0.0f, 400.0f,
		//	FMath::Clamp(increaserH += GetWorld()->GetDeltaSeconds() / 2, 0.0f, 1.0f));

		////MovementComponent->Velocity = Direction * ActualSpeed * Value;
		//GetRootComponent()->ComponentVelocity = Direction * ActualSpeed * Value;

		AddMovementInput(Direction, Value);

	}

	XMoveDir = Value;
}

void ABuildingEscapeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		UPawnMovementComponent* MovementComponent = GetMovementComponent();
	
		//ActualSpeed = FMath::Lerp(0.0f, 400.0f, 
		//	FMath::Clamp(increaserH += GetWorld()->GetDeltaSeconds() / 2, 0.0f,1.0f));

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement in that direction
		AddMovementInput(Direction, Value);
		//MovementComponent->Velocity = Direction * ActualSpeed * Value;
		//GetRootComponent()->ComponentVelocity = Direction * ActualSpeed * Value;

	}

	YMoveDir = Value;
}

void ABuildingEscapeCharacter::Roll()
{
	PlayAnimMontage(RollMontage, 1.f, "start01");


	if ((Controller != NULL))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		UPawnMovementComponent* MovementComponent = GetMovementComponent();
		// get forward vector
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		Direction *= 50000;
		//GetRootComponent()->ComponentVelocity = Direction;
		//MovementComponent->AddInputVector(Direction, true);
		AddMovementInput(Direction, 1.0f);
	}
	//UE_LOG(LogTemp, Warning, TEXT("%f, %f"), XMoveDir, YMoveDir)
	//FVector2D moveDir = FVector2D(XMoveDir, YMoveDir);
	// = moveDir.GetSafeNormal();
	//if (moveDir.X >= 0.9f && (moveDir.Y <= 0.1f && moveDir.Y >= -0.1f))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Nah Man1"))
	//}
	//else if ((moveDir.X <= 0.1f && moveDir.X >= -0.1f) && moveDir.Y <= -0.9)
	//{
	//	PlayAnimMontage(RollMontage, 1.f, "start03");
	//	UE_LOG(LogTemp, Warning, TEXT("Nah Ma2"))

	//}
	//else if (moveDir.X <= -0.9f && (moveDir.Y <= 0.1f && moveDir.Y >= -0.1f))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Nah Man3"))

	//	PlayAnimMontage(RollMontage, 1.f, "start05");
	//}
	//else if ((moveDir.X <= 0.1f && moveDir.X >= -0.1f) && moveDir.Y >= 0.9f)
	//{
	//	
	//	UE_LOG(LogTemp, Warning, TEXT("Nah Man4"))
	//		PlayAnimMontage(RollMontage, 1.f, "start07");
	//}
	//else if (moveDir.X >= -0.5f && moveDir.Y >= 0.5f)
	//{
	//	PlayAnimMontage(RollMontage, 1.f, "start02");
	//	UE_LOG(LogTemp, Warning, TEXT("Nah Man5"))

	//}
	//else if (moveDir.X <= -0.5f && moveDir.Y <= -0.5f)
	//{
	//	PlayAnimMontage(RollMontage, 1.f, "start04");
	//	UE_LOG(LogTemp, Warning, TEXT("Nah Man6"))

	//}
	//else if (moveDir.X <= -0.5f && moveDir.Y >= 0.5f)
	//{
	//	PlayAnimMontage(RollMontage, 1.f, "start06");
	//	UE_LOG(LogTemp, Warning, TEXT("Nah Man7"))

	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Nah Man8"))
	//	PlayAnimMontage(RollMontage, 1.f, "start08");
	//}
}
