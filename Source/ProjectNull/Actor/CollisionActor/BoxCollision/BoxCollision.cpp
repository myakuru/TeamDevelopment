// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxCollision.h"

// Sets default values
ABoxCollision::ABoxCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoxCollision::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

