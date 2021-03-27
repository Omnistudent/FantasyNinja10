// No licence stuff

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintFunctionLibraryGetDir.generated.h"

/**
 * 
 */



UCLASS()
class FANTASYNINJA_API UBlueprintFunctionLibraryGetDir : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintPure, Category = Game)
	static TArray<FString> GetAllSaveGameSlotNames();
	
};
