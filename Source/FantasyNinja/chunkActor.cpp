// Fill out your copyright notice in the Description page of Project Settings.


#include "chunkActor.h"

#include "SimplexNoiseLibrary.h"


#include "EngineUtils.h"


const int32 bTriangles[] = { 2, 1, 0, 0, 3, 2 };
const FVector2D bUVs[] = { FVector2D(0.000000, 0.000000), FVector2D(0.000000, 1.000000), FVector2D(1.000000, 1.000000), FVector2D(1.000000, 0.000000) };
const FVector bNormals0[] = { FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1) };
const FVector bNormals1[] = { FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1) };
const FVector bNormals2[] = { FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0) };
const FVector bNormals3[] = { FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0) };
const FVector bNormals4[] = { FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0) };
const FVector bNormals5[] = { FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0) };
const FVector bMask[] = { FVector(0.000000, 0.000000, 1.000000), FVector(0.000000, 0.000000, -1.000000), FVector(0.000000, 1.000000, 0.000000), FVector(0.000000, -1.000000, 0.000000), FVector(1.000000, 0.000000, 0.000000), FVector(-1.000000, 0.000000, 0.000000) };


// Sets default values
AchunkActor::AchunkActor()
{
 	 //Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}


// Called when the game starts or when spawned
void AchunkActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AchunkActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AchunkActor::OnConstruction(const FTransform & Transform)
{
	USimplexNoiseLibrary::setNoiseSeed(randomSeed); // random seed for noise
	chunkZElements = 64;
	chunkLineElementsExt = chunkLineElements + 2;
	chunkTotalElements = chunkLineElements * chunkLineElements * chunkZElements;
	chunkLineElementsP2 = chunkLineElements * chunkLineElements;
	chunkLineElementsP2Ext = chunkLineElementsExt * chunkLineElementsExt;
	voxelSizeHalf = voxelSize / 2;

	FString string = "Voxel_" + FString::FromInt(chunkXindex) + "_" + FString::FromInt(chunkYindex);
	FName name = FName(*string);
	proceduralComponent = NewObject<UProceduralMeshComponent>(this, name);
	proceduralComponent->RegisterComponent();

	RootComponent = proceduralComponent;
	RootComponent->SetWorldTransform(Transform); // set position of root component (bubble actor)
	Super::OnConstruction(Transform);

	GenerateChunk();		// generate voxel data from noise
	UpdateMesh();			// generate mesh sides
}


void AchunkActor::GenerateChunk()
{
	FRandomStream RandomStream = FRandomStream(randomSeed);
	TArray<FIntVector> treeCenters;
	TArray <int32> tempLeaves;

	chunkFields.SetNumUninitialized(chunkTotalElements);

	TArray <int32> noise = calculateNoise();
	GEngine->AddOnScreenDebugMessage(-1, 200, FColor::Green, FString::Printf(TEXT("Hello %s"), *GetActorLocation().ToString()));
	for (int32 x = 0; x < chunkLineElements; x++)
	{
		for (int32 y = 0; y < chunkLineElements; y++)
		{
			for (int32 z = 0; z < chunkZElements; z++)
			{
				int32 index = x + (y * chunkLineElements) + (z * chunkLineElementsP2);

				//chunkFields[index] = (z <30) ? 1 : 0;

				//if (z == 31 + noise[x + y * chunkLineElements] && RandomStream.FRand() < 0.01) treeCenters.Add(FIntVector(x, y, z));

				if (z == 30 + noise[x + y * chunkLineElementsExt]) chunkFields[index] = 11;
				else if (z == 29 + noise[x + y * chunkLineElementsExt]) chunkFields[index] = 12;
				else if (z < 29 + noise[x + y * chunkLineElementsExt]) chunkFields[index] = 13;
				else chunkFields[index] = 0;
				
				//int32 index = x + (y * chunkLineElements) + (z * chunkLineElementsP2);
				//chunkFields[index] = (z == 29 + noise[x + y * chunkLineElements]) ? 1 : 0;
				//if (z == 28 + noise[x + y * chunkLineElements]) chunkFields[index] = 2;
				//if (z < 28 + noise[x + y * chunkLineElements]) chunkFields[index] = 3;
				//if (RandomStream.FRand() < 0.01 && z == 30 + noise[x + y * chunkLineElements]) treeCenters.Add(FIntVector(x, y, z));
			}
		}
	}
}









void AchunkActor::UpdateMesh()
{
	TArray<FVector> Vertices ;
	TArray<int32> Triangles ;
	TArray<FVector> Normals ;
	TArray<FVector2D> UVs ;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;
	
	int32 elementID = 0;



	//int32 el_num = 0;


	for (int32 x = 0; x < chunkLineElements; x++)
	{
		for (int32 y = 0; y < chunkLineElements; y++)
		{
			for (int32 z = 0; z < chunkZElements; z++)
			{
				int32 index = x  + (chunkLineElements * y) + (chunkLineElementsP2 * z);
				int32 meshIndex = chunkFields[index];

				
				if (meshIndex > 0)
				{
					meshIndex--;

					
					
					
					
					// add faces, verticies, UVS and Normals
					int triangle_num = 0;
					for (int i = 0; i < 6; i++)
					{
						int newIndex = index + bMask[i].X + (bMask[i].Y * chunkLineElements) + (bMask[i].Z * chunkLineElementsP2);

						bool flag = false;
						if (meshIndex >= 20) flag = true;
						else if ((x + bMask[i].X < chunkLineElements) && (x + bMask[i].X >= 0) && (y + bMask[i].Y < chunkLineElements) && (y + bMask[i].Y >= 0))
						{
							if (newIndex < chunkFields.Num() && newIndex >= 0)
								if (chunkFields[newIndex] <= 1) flag = true;	// if see through or none
						}
						else flag = true;

						if (flag)
						{
							Triangles.Add(bTriangles[0] + triangle_num + elementID);
							Triangles.Add(bTriangles[1] + triangle_num + elementID);
							Triangles.Add(bTriangles[2] + triangle_num + elementID);
							Triangles.Add(bTriangles[3] + triangle_num + elementID);
							Triangles.Add(bTriangles[4] + triangle_num + elementID);
							Triangles.Add(bTriangles[5] + triangle_num + elementID);
							triangle_num += 4;	// add 4 verticies to next triangle

							switch (i)
							{
							case 0: {
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));

								Normals.Append(bNormals0, ARRAY_COUNT(bNormals0));
								break;
							}
							case 1: {
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));

								Normals.Append(bNormals1, ARRAY_COUNT(bNormals1));
								break;
							}
							case 2: {
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));

								Normals.Append(bNormals2, ARRAY_COUNT(bNormals2));
								break;
							}
							case 3: {
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Normals.Append(bNormals3, ARRAY_COUNT(bNormals3));
								break;
							}
							case 4: {
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Normals.Append(bNormals4, ARRAY_COUNT(bNormals4));
								break;
							}
							case 5: {
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), -voxelSizeHalf + (z * voxelSize)));
								Vertices.Add(FVector(-voxelSizeHalf + (x * voxelSize), -voxelSizeHalf + (y * voxelSize), voxelSizeHalf + (z * voxelSize)));
								Normals.Append(bNormals5, ARRAY_COUNT(bNormals5));
								break;
							}
							}

							UVs.Append(bUVs, ARRAY_COUNT(bUVs));
							//FColor color = FColor::MakeRandomColor();
							//FColor color = FColor(RandomStream.FRand() * 256, RandomStream.FRand() * 256, RandomStream.FRand() * 256, rand() % 5);
							FColor color = FColor(255, 255, 255, i);
							VertexColors.Add(color); VertexColors.Add(color); VertexColors.Add(color); VertexColors.Add(color);
						}
					}

					//el_num += triangle_num;
					//meshSections[meshIndex].elementID += triangle_num;
					elementID += triangle_num;
				}
				
			}
		}
	}
	proceduralComponent->ClearAllMeshSections();
	proceduralComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);


}




TArray<int32> AchunkActor::calculateNoise()
{
	TArray<int32> noises;
	noises.Reserve(chunkLineElementsExt * chunkLineElementsExt);
	for (int32 y = -1; y <= chunkLineElements; y++)
	{
		for (int32 x = -1; x <= chunkLineElements; x++)
		{
			float noiseValue =
				USimplexNoiseLibrary::SimplexNoise2D((chunkXindex*chunkLineElements + x) * 0.01f, (chunkYindex*chunkLineElements + y) * 0.01f) * 4 +
				USimplexNoiseLibrary::SimplexNoise2D((chunkXindex*chunkLineElements + x) * 0.01f, (chunkYindex*chunkLineElements + y) * 0.01f) * 8 +
				USimplexNoiseLibrary::SimplexNoise2D((chunkXindex*chunkLineElements + x) * 0.004f, (chunkYindex*chunkLineElements + y) * 0.004f) * 16 +
				FMath::Clamp(USimplexNoiseLibrary::SimplexNoise2D((chunkXindex*chunkLineElements + x) * 0.05f, (chunkYindex*chunkLineElements + y) * 0.05f), 0.0f, 5.0f) * 4; // clamp 0-5
			noises.Add(FMath::FloorToInt(noiseValue));
		}
	}
	return noises;
}

