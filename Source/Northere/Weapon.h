#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class NORTHERE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AWeapon();

	class AEntity* EntityOwner;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Weapon;

};
