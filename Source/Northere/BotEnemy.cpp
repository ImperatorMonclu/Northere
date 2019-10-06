#include "BotEnemy.h"
#include "Bow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Arrow.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Shield.h"
#include "Axe.h"

ABotEnemy::ABotEnemy()
{}

void ABotEnemy::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<class ABow> bowClass = TSubclassOf<ABow>(ABow::StaticClass());
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABow* bow = GetWorld()->SpawnActor<ABow>(bowClass, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), ActorSpawnParams);

	bow->Generated(SkeletalMesh, true);

	Weapon1 = bow;
	Weapon2 = bow;

}

void ABotEnemy::Aim()
{
	IsRun = false;
	IsAimBow = true;
	IsShootBow = false;

	if (NCharge <= 0)
	{
		ArrowClass = TSubclassOf<AArrow>(AArrow::StaticClass());

		if (ArrowClass != NULL)
		{
			UWorld* const World = GetWorld();

			if (World != NULL)
			{
				const FRotator SpawnRotation = SkeletalMesh->GetSocketRotation("LeftHandIndex1SocketArrowBot");
				const FVector SpawnLocation = SkeletalMesh->GetSocketLocation("LeftHandIndex1SocketArrowBot");

				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				CurrentArrow = World->SpawnActor<AArrow>(ArrowClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

				CurrentArrow->AttachToComponent(SkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), "LeftHandIndex1SocketArrowBot");

				if (CurrentArrow != nullptr)
				{
					CurrentArrow->EntityOwner = this;

				}

			}
		}
	}

	IsTimeActive = true;

	BowSound->SetSound(DrawSound);
	BowSound->Play(0.0f);

	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABotEnemy::ChargeArrow, 0.01f, false);

}

void ABotEnemy::ChargeArrow()
{
	if (NCharge < 140)
	{
		NCharge++;

		if (CurrentArrow != NULL)
		{
			CurrentArrow->SetActorRelativeLocation(FVector(0.0f, -NCharge / 2, 0.0f));

		}

		GetWorld()->GetTimerManager().ClearTimer(Handle);

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABotEnemy::ChargeArrow, 0.005f, false);

	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);

	}

}

void ABotEnemy::StopAim()
{
	BowSound->Stop();

	IsAimBow = false;

	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABotEnemy::DischargeArrow, 0.01f, false);

}

void ABotEnemy::DischargeArrow()
{
	if (NCharge > 0)
	{
		NCharge -= 2;

		if (CurrentArrow != NULL)
		{
			CurrentArrow->SetActorRelativeLocation(FVector(0.0f, -NCharge / 2, 0.0f));

			if (NCharge <= 0)
			{
				CurrentArrow->Destroy();

			}
		}

		GetWorld()->GetTimerManager().ClearTimer(Handle);

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABotEnemy::DischargeArrow, 0.005f, false);

	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);

	}

}

void ABotEnemy::Block()
{
	if (Weapon1 != NULL)
	{
		IsBlock = true;

		AShield* s = Cast<AShield>(Weapon1);
		s->Action();

	}

}

void ABotEnemy::StopBlock()
{
	if (Weapon1 != NULL)
	{
		IsBlock = false;

		AShield* s = Cast<AShield>(Weapon1);
		s->Undo();

	}

}

void ABotEnemy::Shoot(float velocity, float distance, float height)
{
	IsShootBow = true;

	IsAimBow = false;

	CoolDown = 1;
	IsTimeActive = true;
		
	float angle = atan((FMath::Pow(velocity, 2) - FMath::Pow(FMath::Pow(velocity, 4) - 980.0f * (980.0f * FMath::Pow(distance, 2) + 2 * height*FMath::Pow(velocity, 2)), 0.5f)) / (980.0f * distance));

	FVector vectorAux = GetActorRotation().Vector();

	if (CurrentArrow != NULL)
	{
		CurrentArrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));

		CurrentArrow->Shoot(FVector(vectorAux.X, vectorAux.Y, angle), NCharge);

		AmmoArrow--;

	}

	NCharge = 0;

	BowSound->Stop();

	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABotEnemy::StopShoot, 0.2f, false);

}

void ABotEnemy::StopShoot()
{
	IsAimBow = false;
	IsShootBow = false;

	StopAim();

	GetWorld()->GetTimerManager().ClearTimer(Handle);

}

void ABotEnemy::Melee()
{
	if (Weapon2 != NULL)
	{
		IsMelee = true;

		AAxe* a = Cast<AAxe>(Weapon2);

		a->Overlap = true;

		TimeMelee = 2;

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ABotEnemy::StopMelee, 0.2f, false);

	}

}

void ABotEnemy::StopMelee()
{
	IsMelee = false;

	CoolDown = 1;
	IsTimeActive = true;

	GetWorld()->GetTimerManager().ClearTimer(Handle);

}

int ABotEnemy::WeaponType()
{
	int type = 0;

	if (Weapon1 != NULL)
	{
		if (Weapon1->GetClass()->IsChildOf(ABow::StaticClass()))
		{
			type = 1;

		}
		else
		{
			type = 2;

		}
	}

	return type;

}
