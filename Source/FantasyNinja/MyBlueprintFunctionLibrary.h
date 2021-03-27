// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimplexNoiseLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYNINJA_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable, Category = "MyCategory")
		static int HelloUnreal(int xx, int yy, int randomSeed, int chunkLineElementsExt, int chunkZElements, int chunkXindex, int chunkYindex, float firstmultiplier, float noisemultiplier2, float noisemultiplier3, float noisemultiplier4, int chunkLineElements);



};