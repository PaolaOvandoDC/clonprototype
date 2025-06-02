// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "bomba.generated.h"
class UStaticMeshComponent;
UCLASS()
class BOMBERMAN_API Abomba : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Abomba();
	void Explotar(); //esta funcion activa la explosion
	UPROPERTY(EditAnywhere, Category = "Efectos")
	UParticleSystem* ExplosionEffect;
	float TiempoTranscurrido;
protected:

	// Componente de malla estática
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UStaticMeshComponent* Malla;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool bExploto = false; //esto indica si se exploto
private:
	
	void DestruirBloques();
	bool bEsBombaInicial = true; // Solo la primera bomba podrá generar otras bombas
	void GenerarBombasAleatorias(FVector PosicionBomba);
};
