#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "BotEnemy.generated.h"

UCLASS()
class NORTHERE_API ABotEnemy : public AEntity
{
	GENERATED_BODY()

public:
	ABotEnemy();

	UFUNCTION(BlueprintCallable)
	void Aim();

	UFUNCTION(BlueprintCallable)
	void Block();

	UFUNCTION(BlueprintCallable)
	void StopBlock();

	UFUNCTION(BlueprintCallable)
	void Shoot(float velocity, float distance, float height);

	UFUNCTION(BlueprintCallable)
	void Melee();

	UFUNCTION(BlueprintPure)
	int WeaponType();

protected:

	virtual void BeginPlay() override;

private:

	void ChargeArrow();

	void StopAim();

	void DischargeArrow();

	void StopShoot();

	void StopMelee();

};
