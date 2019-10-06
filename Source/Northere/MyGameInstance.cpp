#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	ColorCharacter.Add(FLinearColor(0.0f, 0.0f, 0.5f, 1.0f));
	ColorCharacter.Add(FLinearColor(0.5f, 0.0f, 0.0f, 1.0f));
	ColorCharacter.Add(FLinearColor(0.0f, 0.5f, 0.0f, 1.0f));
	ColorCharacter.Add(FLinearColor(0.5f, 0.5f, 0.0f, 1.0f));
	ColorCharacter.Add(FLinearColor(0.3f, 0.1f, 0.0f, 1.0f));

	NameCharacter = "Player";
	Presentation = true;

	NamesAndScore.Empty();

}

TArray<FLinearColor> UMyGameInstance::GetColorCharacter()
{
	return ColorCharacter;

}

FString UMyGameInstance::GetNameCharacter()
{
	return NameCharacter;

}

bool UMyGameInstance::GetPresentation()
{
	return Presentation;

}

void UMyGameInstance::SetColorCharacter(TArray<FLinearColor> nColorCharacter)
{
	ColorCharacter = nColorCharacter;

}

void UMyGameInstance::SetNameCharacter(FString nNameCharacter)
{
	if (nNameCharacter != "")
	{
		NameCharacter = nNameCharacter;

	}

}

void UMyGameInstance::SetPresentation(bool nPresentation)
{
	Presentation = nPresentation;

}

void UMyGameInstance::SaveEntityScore(FString SaveText)
{
	int i = SaveText.Find(";");
	int value = FCString::Atoi(*SaveText.Mid(i + 1, SaveText.Len() - 1));
	int j = 0;

	if (NamesAndScore.Num() > 0)
	{
		for (FString aux : NamesAndScore)
		{
			i = aux.Find(";");
			int valueaux = FCString::Atoi(*aux.Mid(i + 1, aux.Len() - 1));

			if (value >= valueaux)
			{
				NamesAndScore.Insert(SaveText, j);
				value = -1;

			}

			j++;

			if (j >= NamesAndScore.Num() && value != -1)
			{
				NamesAndScore.Insert(SaveText, j);

			}

		}

	}
	else
	{
		NamesAndScore.Add(SaveText);

	}

}

TArray<FText> UMyGameInstance::LoadEntitiesScore()
{
	TArray<FText> SaveTextListAux;

	for (FString aux : NamesAndScore)
	{
		int i = aux.Find(";");

		FText auxName = FText::FromString(aux.Mid(0, i));
		SaveTextListAux.Add(auxName);

		FText auxScore = FText::FromString(aux.Mid(i + 1, aux.Len() - 1));
		SaveTextListAux.Add(auxScore);

	}

	return SaveTextListAux;

}

void UMyGameInstance::ClearEntityScore()
{
	NamesAndScore.Empty();

}
