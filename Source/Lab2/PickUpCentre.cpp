// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpCentre.h"

// Sets default values
APickUpCentre::APickUpCentre()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject< UStaticMeshComponent>("MeshComponent ");
	Mesh->SetCollisionProfileName("Block All");
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetRootComponent(Mesh);
}

void APickUpCentre::SetNewGrenades(int& Grenades)
{
	Grenades = 5;
}

// Called when the game starts or when spawned
void APickUpCentre::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("PickupCentre");
}

// Called every frame
void APickUpCentre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

