// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Todo.h"

// Sets default values
APlayer_Todo::APlayer_Todo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayer_Todo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer_Todo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayer_Todo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

