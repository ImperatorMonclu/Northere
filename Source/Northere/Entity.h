#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.generated.h"

UCLASS()
class NORTHERE_API AEntity : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* FootstepSound;

public:
	AEntity();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SkeletalMesh;

	class AArrow* CurrentArrow;

	void Tick(float Delta) override;

	UFUNCTION(BlueprintCallable)
		void WalkForward(float Value);

	UFUNCTION(BlueprintCallable)
		void WalkRight(float Value);

	UFUNCTION(BlueprintCallable)
		void Run();

	UFUNCTION(BlueprintCallable)
		void StopRun();

	UFUNCTION(BlueprintCallable)
		void JumpAnim();

	UFUNCTION(BlueprintCallable)
		void ChangeWeapon();

	void UpdateLife(int Life);

	UFUNCTION(BlueprintPure)
		float GetSpeed();

	UFUNCTION(BlueprintPure)
		float GetTurn();

	UFUNCTION(BlueprintPure)
		bool GetIsAimBow();

	UFUNCTION(BlueprintPure)
		bool GetIsShootBow();

	UFUNCTION(BlueprintPure)
		bool GetIsMelee();

	UFUNCTION(BlueprintPure)
		bool GetIsBlock();

	UFUNCTION(BlueprintPure)
		bool GetIsChangeWeapon();

	UFUNCTION(BlueprintPure)
		bool GetIsJump();

	UFUNCTION(BlueprintPure)
		bool GetIsMeleeHit();

	UFUNCTION(BlueprintPure)
		int GetLife();

	UFUNCTION(BlueprintPure)
		int GetNumLifes();

	UFUNCTION(BlueprintPure)
		int GetCooldown();

	UFUNCTION(BlueprintPure)
		int GetScore();

	UFUNCTION(BlueprintPure)
		FString GetNameEntity();

	UFUNCTION(BlueprintPure)
		int GetAmmoArrow();

	UFUNCTION(BlueprintPure)
		FLinearColor GetFlagColor();

	UFUNCTION(BlueprintPure)
		FVector GetSpawnLocation();

	UFUNCTION(BlueprintCallable)
		void SetFlagColor(FLinearColor FlagColor);

	UFUNCTION(BlueprintCallable)
		void SetSpawnLocation(FVector SpawnLocation);

	UFUNCTION(BlueprintCallable)
		void SetLife(int Life);

	UFUNCTION(BlueprintCallable)
		void SetNumLifes(int NumLifes);

	UFUNCTION(BlueprintCallable)
		void UpdateScore(int Score);

	UFUNCTION(BlueprintCallable)
	void SetNameEntity(FString NameEntity);

	void SetIsMelee(bool IsMelee);

	void SetIsMeleeHit(bool IsMeleeHit);

	void UpdateAmmoArrow(int AmmoArrow);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* BowSound;

	class AWeapon* Weapon1;

	class AWeapon* Weapon2;

	TSubclassOf<class AArrow> ArrowClass;

	void SetSpeed(float Speed);

	void SetTurn(float Turn);

	void SetIsAimBow(bool IsAimBow);

	void SetIsShootBow(bool IsShootBow);

	void SetIsBlock(bool IsBlock);

	void SetIsChangeWeapon(bool IsChangeWeapon);

	void SetIsJump(bool IsJump);

	int CoolDown;

	float BaseTurnRate;

	float BaseLookUpRate;

	bool IsAimBow;

	bool IsJump;

	bool IsRun;

	bool IsShootBow;

	bool IsBlock;

	bool IsMelee;

	int Life;

	int NCharge;

	bool IsTimeActive;

	int TimeMelee;

	int AmmoArrow;

	class USoundCue* DrawSound;

	FTimerHandle Handle;

	FTimerHandle Handle2;

private:

	int Score;

	float Speed;

	float Turn;

	bool IsChangeWeapon;

	bool IsMeleeHit;

	float Time;

	float AccelerationRun;

	int NumLifes;

	FString NameEntity;

	void StopJump();

	void StopChangeWeapon();

	void StopMeleeHit();

	void StopRed();

	void Respawn();

	FLinearColor FlagColor;

	FVector SpawnLocation;

	class USoundCue* WalkSound;

	class USoundCue* RunSound;

	bool IsFootstepSound;

};
