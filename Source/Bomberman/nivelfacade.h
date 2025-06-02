// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "iBombaFacade.h"	
#include "nivelfacade.generated.h"
UCLASS()
class BOMBERMAN_API Anivelfacade : public AActor ,public IiBombaFacade
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Anivelfacade();
	virtual void nivelfacil() override;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
