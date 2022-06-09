// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpCentre.h"

// Sets default values
APickUpCentre::APickUpCentre()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickUpCentre::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpCentre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

