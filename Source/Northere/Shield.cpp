#include "Shield.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Entity.h"

AShield::AShield()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bowStatic(TEXT("StaticMesh'/Game/Assets/Meshes/Shield/Shield.Shield'"));
	ShieldStatic = bowStatic.Object;

	CollisionComp1 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComp1"));
	CollisionComp1->SetupAttachment(RootComponent);

	CollisionComp1->InitCapsuleSize(20.0f, 50.0f);
	CollisionComp1->SetRelativeLocation(FVector(0.0f, 15.0f, 0.0f));
	CollisionComp1->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	CollisionComp1->SetNotifyRigidBodyCollision(true);
	CollisionComp1->BodyInstance.SetCollisionProfileName("Projectile");

	CollisionComp1->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComp1->CanCharacterStepUpOn = ECB_No;
	
	CollisionComp2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComp2"));
	CollisionComp2->SetupAttachment(RootComponent);

	CollisionComp2->InitCapsuleSize(20.0f, 50.0f);
	CollisionComp2->SetRelativeLocation(FVector(0.0f, 15.0f, 0.0f));
	CollisionComp2->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	CollisionComp2->SetNotifyRigidBodyCollision(true);
	CollisionComp2->BodyInstance.SetCollisionProfileName("Projectile");

	CollisionComp2->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComp2->CanCharacterStepUpOn = ECB_No;

	CollisionComp3 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComp3"));
	CollisionComp3->SetupAttachment(RootComponent);

	CollisionComp3->InitCapsuleSize(20.0f, 50.0f);
	CollisionComp3->SetRelativeLocation(FVector(0.0f, 15.0f, 0.0f));
	CollisionComp3->SetRelativeRotation(FRotator(45.0f, 0.0f, 0.0f));

	CollisionComp3->SetNotifyRigidBodyCollision(true);
	CollisionComp3->BodyInstance.SetCollisionProfileName("Projectile");

	CollisionComp3->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComp3->CanCharacterStepUpOn = ECB_No;

	CollisionComp4 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComp4"));
	CollisionComp4->SetupAttachment(RootComponent);

	CollisionComp4->InitCapsuleSize(20.0f, 50.0f);
	CollisionComp4->SetRelativeLocation(FVector(0.0f, 15.0f, 0.0f));
	CollisionComp4->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	CollisionComp4->SetNotifyRigidBodyCollision(true);
	CollisionComp4->BodyInstance.SetCollisionProfileName("Projectile");

	CollisionComp4->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComp4->CanCharacterStepUpOn = ECB_No;

}

void AShield::Generated(USceneComponent* parent)
{
	EntityOwner = Cast<AEntity>(parent->GetOwner());
	
	Weapon->SetRelativeLocation(parent->GetSocketLocation("LeftHandIndex1SocketShield"));
	Weapon->SetRelativeRotation(parent->GetSocketRotation("LeftHandIndex1SocketShield"));

	Weapon->AttachToComponent(parent, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), "LeftHandIndex1SocketShield");
	
	Weapon->SetStaticMesh(ShieldStatic);

	Weapon->BodyInstance.SetCollisionProfileName("Projectile");

}

void AShield::Action()
{
	CollisionComp1->SetCapsuleSize(30.0f, 90.0f, false);

	CollisionComp2->SetCapsuleSize(30.0f, 90.0f, false);

	CollisionComp3->SetCapsuleSize(30.0f, 90.0f, false);

	CollisionComp4->SetCapsuleSize(30.0f, 90.0f, false);

}

void AShield::Undo()
{
	CollisionComp1->SetCapsuleSize(20.0f, 50.0f, false);

	CollisionComp2->SetCapsuleSize(20.0f, 50.0f, false);

	CollisionComp3->SetCapsuleSize(20.0f, 50.0f, false);

	CollisionComp4->SetCapsuleSize(20.0f, 50.0f, false);

}
