#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Bow.generated.h"

UCLASS()
class NORTHERE_API ABow : public AWeapon
{
	GENERATED_BODY()

	class UStaticMesh* BowStatic;

public:	
	ABow();

	void Generated(USceneComponent* parent, bool bot);

};
