// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bloque.h"
#include "clonprototipe.h"
#include "BloqueD2.generated.h"
/**
 * 
 */
UCLASS()
class BOMBERMAN_API ABloqueD2 : public ABloque, public Iclonprototipe	
{
	GENERATED_BODY()
public:
	ABloqueD2();

protected:
	virtual void BeginPlay() override;
public:

	// Declarar un mapa de bloques como un array bidimensional
	
	TArray<TArray<int32>> aMapaBloques;

	float XInicial = -600.00f;
	float YInicial = -1000.00f;
	float ZInicial = 0.0f;
	float AnchoBloque = 100.0f;
	float LargoBloque = 100.0f;
	//ABloque* BloqueActual = nullptr;
	// Declarar un array de punteros a objetos de tipo BloqueMadera
	TArray<ABloque*> aBloques;

	FTimerHandle tHDestruirBloques;

	void SpawnBloque(FVector posicion, int32 tipoBloque);
	//void DestruirBloque();
	// Método llamado cada frame
	virtual void Tick(float DeltaTime) override;
	virtual AActor* Clonar(FVector Pos, float Dis) override;
	void GenerarLaberintoSimetrico();
	FVector PosicionInicial;
	float DistanciaObjetivo = 1000.0f;
	// Indica si es un clon (para evitar doble generación)
	bool bEsClon = false;
};
