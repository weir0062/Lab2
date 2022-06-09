// Copyright Epic Games, Inc. All Rights Reserved.

#include "Lab2Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TextRenderComponent.h"
#include "Grenade.h"
#include "PickUpCentre.h"
//////////////////////////////////////////////////////////////////////////
// ALab2Character




//
//
//2) Implement the following :
//
//Add ability to throw grenades when G key is pressed.Grenades must be affected by gravityand should stick / attach to all intersecting objects.
//Game should start with total of 5 grenades.Grenade counter should decrease when single grenade is deployed.Grenade throwing functionality must be limited if grenade counter is 0.
//Upon explosion grenade should emit explosion particle and apply radial damage to the player if within the blast radius.
//Add custom HUDand display number of remaining grenades. (Use UUserWidget class as a parent) Additionally, use TextComponentand display grenade count above player.
//Add grenade pickup base.When player touches the pickup base number of grenades should be set to its maximum value of 5 grenades.


ALab2Character::ALab2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	NumGrenades = 5;


	TextComponent = CreateDefaultSubobject<UTextRenderComponent>("Text");
	TextComponent->SetRelativeLocation(TextComponent->GetRelativeLocation() + GetActorUpVector() * 100);
	TextComponent->SetTextRenderColor(FColor::Purple);
	TextComponent->SetupAttachment(RootComponent);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALab2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ALab2Character::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ALab2Character::MoveRight);

	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &ALab2Character::ThrowGrenade);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALab2Character::Interact);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ALab2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ALab2Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALab2Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALab2Character::TouchStopped);
}

void ALab2Character::Tick(float DeltaTime)
{



	if (TextComponent)
	{
		FString value = FString::Printf(TEXT("%d"), NumGrenades);
		TextComponent->SetText(FText::FromString(value));
	}

}


void ALab2Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ALab2Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ALab2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALab2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALab2Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALab2Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}



void ALab2Character::ThrowGrenade()
{
	if (NumGrenades > 0)
	{

	UWorld* world = GetWorld();
	FVector offset = ((GetActorUpVector() + 50) + (GetActorRightVector() - 70) + (GetActorForwardVector() + 125));
	FVector StartPos = GetActorLocation() + offset;
	
	FRotator StartRot = GetActorRotation();
	world->SpawnActor(Grenade, &StartPos, &StartRot);
	NumGrenades--;
	}
}

void ALab2Character::Interact()
{

	FHitResult  Hit;
	FVector Start = GetActorLocation();
	FVector End = (GetActorForwardVector() * 10000) + Start;
	FCollisionQueryParams CollisionParams;



	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams))
	{
		AActor* actor = Hit.GetActor();



		if (actor->IsA<APickUpCentre>() || actor->ActorHasTag("PuzzlePiece"))
		{

			APickUpCentre* pickup = Cast<APickUpCentre>(actor);
			pickup->SetNewGrenades(NumGrenades);
		}

	}
}
