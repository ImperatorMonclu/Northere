#include "Bow.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Entity.h"

ABow::ABow()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bowStatic(TEXT("StaticMesh'/Game/Assets/Meshes/Bow/Bow.Bow'"));
	BowStatic = bowStatic.Object;
	
}

void ABow::Generated(USceneComponent* parent, bool bot)
{
	EntityOwner = Cast<AEntity>(parent->GetOwner());

	if (bot)
	{
		Weapon->SetRelativeLocation(parent->GetSocketLocation("LeftHandIndex1SocketBowBot"));
		Weapon->SetRelativeRotation(parent->GetSocketRotation("LeftHandIndex1SocketBowBot"));
		
		Weapon->AttachToComponent(parent, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), "LeftHandIndex1SocketBowBot");
	
	}
	else
	{
		Weapon->SetRelativeLocation(parent->GetSocketLocation("LeftHandIndex1SocketBow"));
		Weapon->SetRelativeRotation(parent->GetSocketRotation("LeftHandIndex1SocketBow"));
		
		Weapon->AttachToComponent(parent, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), "LeftHandIndex1SocketBow");
	
	}

	Weapon->SetStaticMesh(BowStatic);

}
