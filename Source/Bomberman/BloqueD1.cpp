// Fill out your copyright notice in the Description page of Project Settings.


#include "BloqueD1.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ABloqueD1::ABloqueD1()
{
    PrimaryActorTick.bCanEverTick = true;
    if (MallaBloque)
    {
        static ConstructorHelpers::FObjectFinder<UMaterial> MaterialBase(TEXT("Material'/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor'"));
        if (MaterialBase.Succeeded())
        {
            MallaBloque->SetMaterial(0, MaterialBase.Object); // Asignar el material al slot 0
        }
    }
}

void ABloqueD1::BeginPlay()
{
    Super::BeginPlay();
    PosicionInicial = GetActorLocation(); // Guarda la posición inicial al comenzar
	
}

void ABloqueD1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector PosicionActual = GetActorLocation();
    float DistanciaRecorrida = PosicionActual.X - PosicionInicial.X;

    if (DistanciaRecorrida < DistanciaObjetivo)
    {
        PosicionActual.X += 50.0f * DeltaTime;
        SetActorLocation(PosicionActual);
    }
}
AActor* ABloqueD1::Clonar(FVector Pos, float Dis)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;

    FActorSpawnParameters Params;
    AActor* clon = Mundo->SpawnActor<ABloqueD1>(GetClass(), Pos, FRotator::ZeroRotator);

    if (clon)
    {
        ABloqueD1* BloqueClon = Cast<ABloqueD1>(clon);
        if (BloqueClon)
        {
            BloqueClon->SetActorLocation(Pos);
           // BloqueClon->SetActorScale3D(FVector(Dis));
            BloqueClon->PosicionInicial = Pos;
            // **Desactivar colisiones para evitar bloqueos**
            BloqueClon->SetActorEnableCollision(false);

            FString Mensaje = FString::Printf(TEXT("Clon creado en: X=%f, Y=%f, Z=%f"), Pos.X, Pos.Y, Pos.Z);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Mensaje);
        }
    }

    return clon;
}