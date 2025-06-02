// Fill out your copyright notice in the Description page of Project Settings.


#include "BloqueD2.h"
#include "BloqueAcero.h"    
#include "BloqueMadera.h"   
#include "BloqueConcreto.h" 
#include "BloqueLadrillo.h" 
#include "Engine/World.h"   
#include "Engine/Engine.h"  
#include "Math/UnrealMathUtility.h"

ABloqueD2::ABloqueD2()
{
    PrimaryActorTick.bCanEverTick = true; // Habilitar la función Tick() 
    if (MallaBloque)
    {
        static ConstructorHelpers::FObjectFinder<UMaterial> MaterialBase(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Copper.M_Metal_Copper'"));
        if (MaterialBase.Succeeded())
        {
            MallaBloque->SetMaterial(0, MaterialBase.Object); // Asignar el material al slot 0
        }
    }
    // Ejemplo de inicialización de la matriz (puedes ajustar el tamaño)
    int32 filas = 10;
    int32 columnas = 10;
    aMapaBloques.SetNum(filas);
    for (int32 i = 0; i < filas; ++i)
    {
        aMapaBloques[i].SetNum(columnas);
    }
    
}
void ABloqueD2::GenerarLaberintoSimetrico()
{
    int32 numFilas = aMapaBloques.Num();
    if (numFilas == 0) return;
    int32 numColumnas = aMapaBloques[0].Num();
    int32 mitadColumnas = numColumnas / 2;

    // 1. Generar aleatoriamente la mitad derecha
    for (int32 fila = 0; fila < numFilas; ++fila)
    {
        for (int32 columna = mitadColumnas; columna < numColumnas; ++columna)
        {
            aMapaBloques[fila][columna] = FMath::RandRange(0, 4); // 0: vacío, 1-4: tipos de bloque
        }
    }

    // 2. Clonar la mitad derecha en la izquierda (espejo)
    for (int32 fila = 0; fila < numFilas; ++fila)
    {
        for (int32 columna = 0; columna < mitadColumnas; ++columna)
        {
            int32 columnaEspejo = numColumnas - columna - 1;
            aMapaBloques[fila][columna] = aMapaBloques[fila][columnaEspejo];
        }
    }
}

void ABloqueD2::BeginPlay()
{
    Super::BeginPlay();
    PosicionInicial = GetActorLocation();

    int32 MaxAltura = 3;
    float AlturaBloque = 100.0f;
    int32 numFilas = aMapaBloques.Num();
    int32 numColumnas = aMapaBloques[0].Num();
    int32 mitadColumnas = numColumnas / 2;

    if (!bEsClon)
    {
        GenerarLaberintoSimetrico();
        // Prototipo: genera todo el laberinto
        for (int32 fila = 0; fila < numFilas; ++fila)
        {
            for (int32 columna = 0; columna < numColumnas; ++columna)
            {
                int32 valor = aMapaBloques[fila][columna];
                if (valor != 0)
                {
                    for (int32 altura = 0; altura < MaxAltura; ++altura)
                    {
                        FVector posicionBloque = FVector(XInicial + columna * AnchoBloque, YInicial + fila * LargoBloque, ZInicial + altura * AlturaBloque);
                        SpawnBloque(posicionBloque, valor);
                    }
                }
            }
        }
    }
    else
    {
        // Clon: solo la mitad izquierda
        for (int32 fila = 0; fila < numFilas; ++fila)
        {
            for (int32 columna = 0; columna < mitadColumnas; ++columna)
            {
                int32 valor = aMapaBloques[fila][columna];
                if (valor != 0)
                {
                    for (int32 altura = 0; altura < MaxAltura; ++altura)
                    {
                        FVector posicionBloque = FVector(XInicial + columna * AnchoBloque, YInicial + fila * LargoBloque, ZInicial + altura * AlturaBloque);
                        SpawnBloque(posicionBloque, valor);
                    }
                }
            }
        }
    }
}

void ABloqueD2::SpawnBloque(FVector posicionBloque, int32 tipoBloque)
{

    //ABloque* BloqueGenerado = nullptr;
    ABloque* BloqueGenerado = nullptr;
    // Elegir tipo de bloque basado en el valor
    if (tipoBloque == 4)
    {
        BloqueGenerado = GetWorld()->SpawnActor<ABloqueAcero>(ABloqueAcero::StaticClass(), posicionBloque, FRotator(0.0f, 0.0f, 0.0f));
    }
    else if (tipoBloque == 3)
    {
        BloqueGenerado = GetWorld()->SpawnActor<ABloqueConcreto>(ABloqueConcreto::StaticClass(), posicionBloque, FRotator(0.0f, 0.0f, 0.0f));
    }
    else if (tipoBloque == 2)
    {
        BloqueGenerado = GetWorld()->SpawnActor<ABloqueLadrillo>(ABloqueLadrillo::StaticClass(), posicionBloque, FRotator(0.0f, 0.0f, 0.0f));
    }
    else if (tipoBloque == 1)
    {
        BloqueGenerado = GetWorld()->SpawnActor<ABloqueMadera>(ABloqueMadera::StaticClass(), posicionBloque, FRotator(0.0f, 0.0f, 0.0f));
    }
    // Agregar el bloque al TArray si fue generado
    if (BloqueGenerado)
    {
        aBloques.Add(BloqueGenerado);
    }

}
void ABloqueD2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector PosicionActual = GetActorLocation();
    float DistanciaRecorrida = PosicionActual.Y - PosicionInicial.Y;

    if (DistanciaRecorrida < DistanciaObjetivo)
    {
        PosicionActual.Y += 50.0f * DeltaTime;
        SetActorLocation(PosicionActual);
    }
}

AActor* ABloqueD2::Clonar(FVector Pos, float Dis)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;
    AActor* clon = Mundo->SpawnActor<ABloqueD2>(GetClass(), Pos, FRotator::ZeroRotator);
    if (clon)
    {
        ABloqueD2* BloqueClon = Cast<ABloqueD2>(clon);
        if (BloqueClon)
        {
            // Copiar la mitad derecha en la mitad izquierda del clon (Prototype)
            int32 numFilas = aMapaBloques.Num();
            if (numFilas == 0) return clon;
            int32 numColumnas = aMapaBloques[0].Num();
            int32 mitadColumnas = numColumnas / 2;

            BloqueClon->aMapaBloques.SetNum(numFilas);
            for (int32 fila = 0; fila < numFilas; ++fila)
            {
                BloqueClon->aMapaBloques[fila].SetNum(numColumnas);
                // Copia la mitad derecha
                for (int32 columna = mitadColumnas; columna < numColumnas; ++columna)
                {
                    BloqueClon->aMapaBloques[fila][columna] = aMapaBloques[fila][columna];
                }
                // Espeja la mitad derecha en la izquierda
                for (int32 columna = 0; columna < mitadColumnas; ++columna)
                {
                    int32 columnaEspejo = numColumnas - columna - 1;
                    BloqueClon->aMapaBloques[fila][columna] = BloqueClon->aMapaBloques[fila][columnaEspejo];
                }
            }

            BloqueClon->bEsClon = true;
            BloqueClon->SetActorLocation(Pos);
            BloqueClon->PosicionInicial = Pos;
            BloqueClon->SetActorEnableCollision(false);

            FString Mensaje = FString::Printf(TEXT("Clon (mitad izquierda) creado en: X=%f, Y=%f, Z=%f"), Pos.X, Pos.Y, Pos.Z);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Mensaje);
            return clon;
        }
    }
    return nullptr;
}

