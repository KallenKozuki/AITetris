// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKBlock.generated.h"

UCLASS()
class AITETRIS_API AKKBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKKBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BlockMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
