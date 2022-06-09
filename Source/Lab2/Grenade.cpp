// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	ProjectileMovement = CreateDefaultSubobject< UProjectileMovementComponent>("Projectile Movement");
	Sphere->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);
	ProjectileMovement->UpdatedComponent  = Sphere;
	ProjectileMovement->InitialSpeed  = 3000.f;
	ProjectileMovement->MaxSpeed  = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity  = true;
	ProjectileMovement->bShouldBounce  = true;
	// Die after 3 seconds by default
	InitialLifeSpan  = 3.0f;

	Mesh = CreateDefaultSubobject< UStaticMeshComponent>("MeshComponent ");
	Mesh->SetupAttachment(Sphere);



	RadialForce  = CreateDefaultSubobject< URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(RootComponent);
	RadialForce->Radius = 269;
	RadialForce->ForceStrength = 10000;
	RadialForce->ImpulseStrength = 1000;
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

}

void AGrenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AGrenade::Explode()
{
	RadialForce->FireImpulse();
}

