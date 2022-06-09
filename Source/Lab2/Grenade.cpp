// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	Mesh = CreateDefaultSubobject< UStaticMeshComponent>("MeshComponent ");
	Mesh->SetCollisionProfileName("Block All");
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetRootComponent(Mesh);
	Mesh->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);

	ProjectileMovement = CreateDefaultSubobject< UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovement->UpdatedComponent  = Mesh;
	ProjectileMovement->InitialSpeed  = 3000.f;
	ProjectileMovement->MaxSpeed  = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity  = true;
	ProjectileMovement->bShouldBounce  = true;
	// Die after 3 seconds by default
	InitialLifeSpan  = 3.0f;




	RadialForce  = CreateDefaultSubobject< URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(Mesh);
	RadialForce->Radius = 369;
	RadialForce->ForceStrength = 10000;
	RadialForce->ImpulseStrength = 10000;
	RadialForce->bImpulseVelChange = true;
	RadialForce->bAutoActivate = false;
	RadialForce->bIgnoreOwningActor = true;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsSticking)
	{
		SetActorLocation(pos);
	}

	
}

void AGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ProjectileMovement->MaxSpeed = 0.0f;
	pos= GetActorLocation();
	IsSticking = true;
	float Delay = 2.f;
	bool Looping = false;
	FTimerHandle MyTimer;
	FTimerManager& Timer = GetWorldTimerManager();
	Timer.SetTimer(MyTimer, this, &AGrenade::Explode, Delay, Looping);
}

void AGrenade::Explode()
{

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Boom");
	RadialForce->FireImpulse();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, GetActorLocation());
	Destroy();
	Mesh->SetVisibility(false);
}

