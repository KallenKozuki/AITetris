// Fill out your copyright notice in the Description page of Project Settings.


#include "KKBlock.h"

// Sets default values
AKKBlock::AKKBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>("Block");
	RootComponent = BlockMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(TEXT("block'/Game/Meshes/block.block'"));
	if (Finder.Succeeded())
	{
		BlockMesh->SetStaticMesh(Finder.Object);
	}
}

// Called when the game starts or when spawned
void AKKBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKKBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

