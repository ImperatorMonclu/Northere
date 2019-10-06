#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class NORTHERE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	UFUNCTION(BlueprintPure)
	TArray<FLinearColor> GetColorCharacter();

	UFUNCTION(BlueprintPure)
		FString GetNameCharacter();

	UFUNCTION(BlueprintPure)
		bool GetPresentation();

	UFUNCTION(BlueprintCallable)
		void SetColorCharacter(TArray<FLinearColor> ColorCharacter);

	UFUNCTION(BlueprintCallable)
		void SetNameCharacter(FString NameCharacter);

	UFUNCTION(BlueprintCallable)
		void SetPresentation(bool Presentation);

	UFUNCTION(BlueprintCallable)
	void SaveEntityScore(FString SaveText);

	UFUNCTION(BlueprintPure)
	TArray<FText> LoadEntitiesScore();

	UFUNCTION(BlueprintCallable)
		void ClearEntityScore();

private:

	bool Presentation;

	TArray<FLinearColor> ColorCharacter;

	FString NameCharacter;

	TArray<FString> NamesAndScore;

};
