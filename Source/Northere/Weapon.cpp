#include "Weapon.h"
#include "Components/StaticMeshComponent.h"

AWeapon::AWeapon()
{
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	RootComponent = Weapon;

	Weapon->RelativeScale3D = FVector(1.2f, 1.2f, 1.2f);

	Weapon->bCastDynamicShadow = false;
	Weapon->CastShadow = false;
	Weapon->SetGenerateOverlapEvents(true);

	Weapon->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	Weapon->CanCharacterStepUpOn = ECB_No;
	Weapon->SetCanEverAffectNavigation(false);
	Weapon->BodyInstance.SetCollisionProfileName("NoCollision");
	
	EntityOwner = NULL;

}
