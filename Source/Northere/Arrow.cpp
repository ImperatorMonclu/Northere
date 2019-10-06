#include "Arrow.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Entity.h"
#include "Shield.h"
#include "TimerManager.h"

AArrow::AArrow()
{
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent = Arrow;

	Arrow->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->SetupAttachment(RootComponent);

	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f));

	CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	SetActorEnableCollision(false);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CollisionComp);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletal(TEXT("SkeletalMesh'/Game/Assets/Meshes/Arrow/Arrow.Arrow'"));
	SkeletalMesh->SkeletalMesh = skeletal.Object;

	SkeletalMesh->SetRelativeLocation(FVector(-70.0f, 0.0f, 0.0f));
	SkeletalMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	SkeletalMesh->RelativeScale3D = FVector(1.2f, 1.2f, 1.2f);

	SkeletalMesh->bCastDynamicShadow = false;
	SkeletalMesh->CastShadow = false;
	SkeletalMesh->SetCanEverAffectNavigation(false);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->UpdatedComponent = CollisionComp;

	Movement->Velocity = FVector(0.0f, 0.0f, 0.0f);
	Movement->ProjectileGravityScale = 0.0f;

	Movement->InitialSpeed = 2000.0f;
	Movement->MaxSpeed = 3000.0f;

	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = false;

	InitialLifeSpan = 10.0f;

	EntityOwner = NULL;

	Overlap = false;
	
	OneTime = true;

	static ConstructorHelpers::FObjectFinder<USoundCue> shoot(TEXT("SoundCue'/Game/Assets/Sounds/ArrowShoot_Cue.ArrowShoot_Cue'"));
	ShootSound = shoot.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> hit(TEXT("SoundCue'/Game/Assets/Sounds/ArrowHit_Cue.ArrowHit_Cue'"));
	HitSound = hit.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> miss(TEXT("SoundCue'/Game/Assets/Sounds/ArrowMiss_Cue.ArrowMiss_Cue'"));
	MissSound = miss.Object;

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> attenuation(TEXT("SoundAttenuation'/Game/Assets/Sounds/SoundAttenuation.SoundAttenuation'"));
	AttenuationSound = attenuation.Object;

}

void AArrow::Shoot(FVector velocity, int charge)
{
	float realCharge = FMath::Pow(1.047128548, charge) + 20.0f;

	if (realCharge > 140.0f)
	{
		realCharge = 140.0f;

	}

	float streng = 3000.0f * realCharge / 140.0f;
	velocity.Normalize();

	Movement->Velocity = FVector(velocity*streng);
	Movement->ProjectileGravityScale = 1.0f;

	UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, ShootSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, AttenuationSound, nullptr, true);

	SetLifeSpan(20.0f);

	Overlap = true;

	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AArrow::EnableCollision, 0.05f, false);

}

void AArrow::EnableCollision()
{
	SetActorEnableCollision(true);

	GetWorld()->GetTimerManager().ClearTimer(Handle);
	
}

void AArrow::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && Overlap && OneTime)
	{
		if (((OtherActor->GetClass()->IsChildOf(AEntity::StaticClass())) && (OtherComp->GetName() == "SkeletalMesh")) || (OtherActor->GetClass()->IsChildOf(AShield::StaticClass())))
		{
			OneTime = false;

			float streng = FMath::Square(Movement->Velocity.Size())/ FMath::Square(3000.0f);

			Movement->StopMovementImmediately();
			Movement->Velocity = FVector(0.0f, 0.0f, 0.0f);
			Movement->ProjectileGravityScale = 0.0f;

			if ((OtherActor->GetClass()->IsChildOf(AEntity::StaticClass())))
			{
				AttachToActor(OtherActor, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));

				AEntity* e = Cast<AEntity>(OtherActor);

				if (e->GetLife() <= streng * 50)
				{
					EntityOwner->UpdateScore(100);

					EntityOwner->UpdateAmmoArrow(4);

				}

				e->UpdateLife(-streng*50);

				EntityOwner->UpdateScore((int)(streng * 50));
				
				UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, HitSound, SweepResult.Location, FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, AttenuationSound, nullptr, true);

			}
			else
			{
				AttachToActor(OtherActor->GetAttachParentActor(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));

				AShield* s = Cast<AShield>(OtherActor);

				s->EntityOwner->UpdateScore((int)(streng * 50 * 3 / 4));

				UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, MissSound, SweepResult.Location, FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, AttenuationSound, nullptr, true);

			}
		}
		else if (!(OtherActor->GetClass()->IsChildOf(AEntity::StaticClass())) && !(OtherActor->GetClass()->IsChildOf(AArrow::StaticClass())))
		{
			OneTime = false;

			Movement->StopMovementImmediately();
			Movement->Velocity = FVector(0.0f, 0.0f, 0.0f);
			Movement->ProjectileGravityScale = 0.0f;

			UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, MissSound, SweepResult.Location, FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, AttenuationSound, nullptr, true);

		}

	}

}
