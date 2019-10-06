#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "MyCharacter.generated.h"

UCLASS()
class NORTHERE_API AMyCharacter : public AEntity
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	AMyCharacter();

	UFUNCTION(BlueprintPure)
		FRotator GetRotate();

	void TurnXAxis(float Value);

	void TurnYAxis(float Value);

	void BlockOrAim();

	void ReduceView();

	void StopBlockOrAim();

	void IncreaseView();
	
	void MeleeOrShoot();

	void StopShoot();

	void StopMelee();

protected:

	virtual void BeginPlay() override;

	void SetRotate(FRotator nRotate);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	FRotator Rotate;
	
	int NView;

};
