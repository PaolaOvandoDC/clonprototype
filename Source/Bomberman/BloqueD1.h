// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bloque.h"
#include "clonprototipe.h"
#include "BloqueD1.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMAN_API ABloqueD1 : public ABloque , public Iclonprototipe
{
	GENERATED_BODY()
public:
	ABloqueD1();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override; //funcion de movimento
	virtual AActor* Clonar(FVector Pos, float Dis) override;
private:
	FVector PosicionInicial;
	float DistanciaObjetivo = 1000.0f;
};
