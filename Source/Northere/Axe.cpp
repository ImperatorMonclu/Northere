#include "Axe.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Entity.h"
#include "Shield.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AAxe::AAxe()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> bowStatic(TEXT("StaticMesh'/Game/Assets/Meshes/Axe/Axe.Axe'"));
	AxeStatic = bowStatic.Object;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->SetupAttachment(RootComponent);

	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->SetRelativeLocation(FVector(-5.0f, 10.0f, 55.0f));

	CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnOverlapBegin);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	static ConstructorHelpers::FObjectFinder<USoundCue> hit(TEXT("SoundCue'/Game/Assets/Sounds/MeleeHit_Cue.MeleeHit_Cue'"));
	HitSound = hit.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> block(TEXT("SoundCue'/Game/Assets/Sounds/MeleeBlock_Cue.MeleeBlock_Cue'"));
	BlockSound = block.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> miss(TEXT("SoundCue'/Game/Assets/Sounds/MeleeMiss_Cue.MeleeMiss_Cue'"));
	MissSound = miss.Object;

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> attenuation(TEXT("SoundAttenuation'/Game/Assets/Sounds/SoundAttenuation.SoundAttenuation'"));
	AttenuationSound = attenuation.Object;

	Overlap = false;
	
}

void AAxe::Generated(USceneComponent* parent)
{
	EntityOwner = Cast<AEntity>(parent->GetOwner());

	Weapon->SetRelativeLocation(parent->GetSocketLocation("RightHandIndex1SocketAxe"));
	Weapon->SetRelativeRotation(parent->GetSocketRotation("RightHandIndex1SocketAxe"));
	
	Weapon->AttachToComponent(parent, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), "RightHandIndex1SocketAxe");
	
	Weapon->SetStaticMesh(AxeStatic);

}

void AAxe::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Overlap && (OtherActor != NULL) && (OtherActor != this))
	{
		if ((OtherActor->GetClass()->IsChildOf(AEntity::StaticClass())) && (OtherComp->GetName() == "SkeletalMesh"))
		{
			Overlap = false;

			AEntity* e = Cast<AEntity>(OtherActor);

			if (e->GetLife() <= 40)
			{
				EntityOwner->UpdateScore(100);

				EntityOwner->UpdateAmmoArrow(4);

			}

			e->UpdateLife(-40);

			EntityOwner->UpdateScore(40);

			UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, HitSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, AttenuationSound, nullptr, true);

		}
		else if (OtherActor->GetClass()->IsChildOf(AShield::StaticClass()))
		{
			Overlap = false;

			AShield* s = Cast<AShield>(OtherActor);

			s->EntityOwner->UpdateScore(30);

			UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, BlockSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, AttenuationSound, nullptr, true);

			EntityOwner->SetIsMelee(false);

			EntityOwner->SetIsMeleeHit(true);

		}
		else if (!(OtherActor->GetClass()->IsChildOf(AEntity::StaticClass())))
		{
			Overlap = false;

			UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, MissSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, AttenuationSound, nullptr, true);

		}

	}

}
