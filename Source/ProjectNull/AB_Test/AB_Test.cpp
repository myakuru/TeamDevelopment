// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_Test.h"

// Sets default values
AAB_Test::AAB_Test()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAB_Test::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Hello World!"));
}

// Called every frame
void AAB_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

