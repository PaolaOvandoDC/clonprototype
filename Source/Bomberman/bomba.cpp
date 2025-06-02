// Fill out your copyright notice in the Description page of Project Settings.


#include "bomba.h"
#include <Kismet/GameplayStatics.h>
#include "Particles/ParticleSystem.h" // Para trabajar con efectos de partículas
#include "Particles/ParticleSystemComponent.h" // Para efectos avanzados de partículas
#include "Components/StaticMeshComponent.h"
#include "Bloque.h"


// Sets default values
Abomba::Abomba()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
    //RootComponent = MeshComp;
    Malla->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjetoMalla(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionAsset(TEXT("/Game/StarterContent/Particles/P_Explosion"));
    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialBase(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile'"));
    if (ObjetoMalla.Succeeded())
    {
        Malla->SetStaticMesh(ObjetoMalla.Object);

        Malla->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    }
    // Cargar efecto de explosión
    if (ExplosionAsset.Succeeded())
    {
        ExplosionEffect = ExplosionAsset.Object;
    }
    TiempoTranscurrido = 0.0f;


    if (MaterialBase.Succeeded())
    {
        Malla->SetMaterial(0, MaterialBase.Object); // Asignar el material al slot 0
    }
}

void Abomba::Explotar()
{
    FVector Posicion = GetActorLocation();

    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, Posicion);
    }
   
    DestruirBloques(); // Aplicar explosión en área de 2 cuadros
    if (bEsBombaInicial)
    {
        // Generar nuevas bombas aleatoriamente
        GenerarBombasAleatorias(GetActorLocation());
    }

    Destroy(); // Elimina el bloque tras explotar
}
void Abomba::DestruirBloques()
{
    FVector Posicion = GetActorLocation();
    float RadioExplosion = 200.0f; // Ajusta según el tamaño de los cuadros

    // Buscar TODOS los bloques que sean descendientes de ABloque
    TArray<AActor*> ActoresCercanos;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABloque::StaticClass(), ActoresCercanos);
//revisa que bloque estan dentro del area de eplosion 
    for (AActor* Actor : ActoresCercanos)
    {
        if (FVector::Dist(Actor->GetActorLocation(), Posicion) <= RadioExplosion) // Explosión en 2 cuadros
        {
            Actor->Destroy(); // Destruir cualquier bloque destructible
        }
    }
}
//genera bpmbas aleatorias dentro de un rango determinado 
void Abomba::GenerarBombasAleatorias(FVector PosicionBomba)
{
    // se ejecuta un ciclo dos veces por que se crearan dos bombas
    for (int i = 0; i < 4; i++) // Generar 3 bombas
    {
        if (!bEsBombaInicial)return; // Solo la bomba inicial genera nuevas bombas
        //bEsBombaInicial = false; // La bomba que explota ya no debe generar más bombas
        float OffsetX = FMath::RandRange(-400.0f, 400.0f); // Movimiento aleatorio en X
        float OffsetY = FMath::RandRange(-400.0f, 400.0f); // Movimiento aleatorio en Y
        //e genera un desplazamiento aleatorio en X e Y dentro del rango [-200, 200].
       // Luego, se suma este desplazamiento a la posición original de la bomba :
        FVector NuevaPosicion = PosicionBomba + FVector(OffsetX, OffsetY, 0.0f); // Nueva posición aleatoria

        Abomba* NuevaBomba = GetWorld()->SpawnActor<Abomba>(GetClass(), NuevaPosicion, FRotator::ZeroRotator);

        if (NuevaBomba)
        {
            NuevaBomba->bEsBombaInicial = false; // Las bombas nuevas NO generarán más bombas
            UE_LOG(LogTemp, Warning, TEXT("¡Bomba generada aleatoriamente en X=%f Y=%f!"), NuevaPosicion.X, NuevaPosicion.Y);
        }

    }
}
// Called when the game starts or when spawned
void Abomba::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//esto ejecuta cada fotograma del juego, su proposito es controlarr el tiempo de esplocion de la bomba
void Abomba::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (bExploto) return; // Si la bomba fue marcada como no explosiva, no hace nada
    // Si no ha explotado aún, hacemos que explote después de cierto tiempo
    TiempoTranscurrido += DeltaTime;

    if (TiempoTranscurrido >= 5.0f && !bExploto) // Explota después de 5 segundos
    {
        Explotar();
        bExploto = true;
    }
}


