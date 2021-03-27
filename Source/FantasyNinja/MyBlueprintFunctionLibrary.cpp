// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

int UMyBlueprintFunctionLibrary::HelloUnreal(int xx, int yy, int randomSeed, int chunkLineElementsExt, int chunkZElements, int chunkXindex, int chunkYindex, float firstmultiplier, float noisemultiplier2, float noisemultiplier3, float noisemultiplier4, int chunkLineElements)
{
	int foo = xx + yy;
	FRandomStream RandomStream = FRandomStream(randomSeed);


	TArray<int32> noises;
	noises.Reserve(chunkLineElementsExt * chunkLineElementsExt);
	for (int32 y = -1; y <= chunkLineElements; y++)
	{
		for (int32 x = -1; x <= chunkLineElements; x++)
		{

			float noiseValue =
				USimplexNoiseLibrary::SimplexNoise2D((chunkXindex*chunkLineElements + x) * 0.01f, (chunkYindex*chunkLineElements + y) * 0.01f) *  firstmultiplier +
				USimplexNoiseLibrary::SimplexNoise2D((chunkXindex*chunkLineElements + x) * 0.01f, (chunkYindex*chunkLineElements + y) * 0.01f) * noisemultiplier2 +
				USimplexNoiseLibrary::SimplexNoise2D((chunkXindex*chunkLineElements + x) * 0.004f, (chunkYindex*chunkLineElements + y) * 0.004f) * noisemultiplier3 +
				FMath::Clamp(USimplexNoiseLibrary::SimplexNoise2D((chunkXindex*chunkLineElements + x) * 0.05f, (chunkYindex*chunkLineElements + y) * 0.05f), 0.0f, 5.0f) * noisemultiplier4; // clamp 0-5
				noises.Add(FMath::FloorToInt(noiseValue));


		}
	}



	foo = noises[xx + yy * chunkLineElementsExt];
	return foo;
}



