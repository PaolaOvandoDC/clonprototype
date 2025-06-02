// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "iBombaFacade.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UiBombaFacade : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BOMBERMAN_API IiBombaFacade
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void nivelfacil() = 0;
	//virtual void nivelmedio(FVector posicion) = 0;
	//virtual void niveldificil(FVector posicionBomba, int32 cantidadBombas) = 0;


};

