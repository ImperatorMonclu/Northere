#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Axe.generated.h"

UCLASS()
class NORTHERE_API AAxe : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollisionComp;

	class UStaticMesh* AxeStatic;

public:
	AAxe();

	void Generated(USceneComponent* parent);

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool Overlap;

private:

	class USoundCue* HitSound;

	class USoundCue* BlockSound;

	class USoundCue* MissSound;

	class USoundAttenuation* AttenuationSound;

};
