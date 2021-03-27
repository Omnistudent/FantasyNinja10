// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "chunkActor.generated.h"



UCLASS()
class FANTASYNINJA_API AchunkActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray <UMaterialInterface *> Materials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray <UMaterialInterface *> MaterialsLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface * leavesMaterialRounded;

	//UPROPERTY()
	//	AVoxelAdministration * voxelAdministration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 randomSeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 voxelSize = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 chunkLineElements = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 chunkXindex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 chunkYindex = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	//	bool bCreateCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float xMult = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float yMult = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float zMult = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float weight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float freq = 1;

	UPROPERTY()
		int32 chunkTotalElements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		int32 chunkZElements;

	UPROPERTY()
		int32 chunkLineElementsExt;

	UPROPERTY()
		int32 chunkLineElementsP2;

	UPROPERTY()
		int32 chunkLineElementsP2Ext;

	UPROPERTY()
		int32 voxelSizeHalf;

	UPROPERTY()
		TArray <int32> chunkFields;

	UPROPERTY()
		UProceduralMeshComponent* proceduralComponent;

	// added noise multipliers to voxelactor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		float noisemultiplier1 = 16.0; //was 16

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		float noisemultiplier2 = 16.0; //was 16

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		float noisemultiplier3 = 16.0; //was 16

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
		float noisemultiplier4 = 16.0; //was 16

public:
	// Sets default values for this actor's properties
	AchunkActor();
public:

	virtual void OnConstruction(const FTransform & Transform) override;
	void GenerateChunk();		// generate voxel data from noise
	void UpdateMesh();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:



	TArray <int32> calculateNoise();
};
