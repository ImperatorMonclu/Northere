#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Shield.generated.h"

UCLASS()
class NORTHERE_API AShield : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CollisionComp1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CollisionComp2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CollisionComp3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CollisionComp4;

	class UStaticMesh* ShieldStatic;

public:	
	AShield();

	void Generated(USceneComponent* parent);

	void Action();

	void Undo();

};
