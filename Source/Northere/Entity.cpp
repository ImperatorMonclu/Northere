#include "Entity.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Arrow.h"
#include "TimerManager.h"
#include "Shield.h"
#include "Axe.h"
#include "Bow.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

AEntity::AEntity()
{
	GetCapsuleComponent()->InitCapsuleSize(60.0f, 120.0f);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxStepHeight = 140.0f;
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->MaxAcceleration = 700.0f;
	GetCharacterMovement()->JumpZVelocity = 650.0f;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletal(TEXT("SkeletalMesh'/Game/Assets/Meshes/Character/Character.Character'"));
	SkeletalMesh->SkeletalMesh = skeletal.Object;
	SkeletalMesh->SetupAttachment(RootComponent);

	SkeletalMesh->RelativeScale3D = FVector(1.2f, 1.2f, 1.2f);
	SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -115.0f));
	SkeletalMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SkeletalMesh->SetGenerateOverlapEvents(true);
	SkeletalMesh->bCastDynamicShadow = false;
	SkeletalMesh->CastShadow = false;

	SkeletalMesh->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	SkeletalMesh->BodyInstance.SetCollisionProfileName("BlockAll");
	SkeletalMesh->CanCharacterStepUpOn = ECB_No;
	SkeletalMesh->SetCanEverAffectNavigation(false);

	NameEntity = NULL;

	BaseTurnRate = 45.0f;
	BaseLookUpRate = 45.0f;

	Speed = 0.0f;
	Turn = 0.0f;

	IsRun = false;
	AccelerationRun = 0.5f;

	IsAimBow = false;
	IsShootBow = false;
	IsMelee = false;
	IsBlock = false;
	IsChangeWeapon = false;
	IsJump = false;
	IsMeleeHit = false;

	Life = 200;
	NumLifes = 3;
	Score = 0;
	AmmoArrow = 8;

	Weapon1 = NULL;
	Weapon2 = NULL;

	Time = 0.0f;
	CoolDown = 0;
	CurrentArrow = NULL;
	NCharge = 0;
	IsTimeActive = false;
	TimeMelee = 0;

	FlagColor = FLinearColor(0.0f, 0.0f, 0.0f);

	FootstepSound = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepSound"));
	FootstepSound->SetupAttachment(RootComponent);

	FootstepSound->SetRelativeLocation(FVector(0.0f, 0.0f, -80.0f));

	BowSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BowSound"));
	BowSound->SetupAttachment(RootComponent);

	BowSound->SetRelativeLocation(FVector(80.0f, 20.0f, 40.0f));

	BowSound->VolumeMultiplier = 0.6f;

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> attenuation(TEXT("SoundAttenuation'/Game/Assets/Sounds/SoundAttenuation.SoundAttenuation'"));
	FootstepSound->AttenuationSettings = attenuation.Object;
	BowSound->AttenuationSettings = attenuation.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> walk(TEXT("SoundCue'/Game/Assets/Sounds/Walk_Cue.Walk_Cue'"));
	WalkSound = walk.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> run(TEXT("SoundCue'/Game/Assets/Sounds/Run_Cue.Run_Cue'"));
	RunSound = run.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> draw(TEXT("SoundCue'/Game/Assets/Sounds/ArrowDraw_Cue.ArrowDraw_Cue'"));
	DrawSound = draw.Object;

	IsFootstepSound = false;

}

void AEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTimeActive)
	{
		Time += DeltaTime;

		if (Time >= 1.0f)
		{

			if (CoolDown > 0)
			{
				CoolDown--;

				if (CoolDown <= 0)
				{
					CoolDown = 0;
					IsTimeActive = false;
				}

			}

			if (TimeMelee > 0)
			{
				IsTimeActive = true;

				TimeMelee--;

				if (TimeMelee <= 0)
				{
					TimeMelee = 0;
					IsTimeActive = false;

					if (Weapon2 != NULL)
					{
						if (Weapon2->GetClass()->IsChildOf(AAxe::StaticClass()))
						{
							AAxe* a = Cast<AAxe>(Weapon2);
							a->Overlap = false;
						}
					}
				}

			}

			if (IsAimBow)
			{
				IsTimeActive = true;
				CurrentArrow->SetLifeSpan(CurrentArrow->GetLifeSpan() + 1.0f);
			}

			Time = 0.0f;

		}

		if (Life <= 0)
		{
			IsTimeActive = false;

		}

	}

}

void AEntity::WalkForward(float Value)
{
	if (Value != 0.0f && !IsMelee && !IsChangeWeapon && Life > 0)
	{
		if (Value > 0.0f)
		{
			if (IsRun)
			{
				if (!IsFootstepSound)
				{
					FootstepSound->SetSound(RunSound);
					FootstepSound->Play(0.0f);

					IsFootstepSound = true;

				}

				if (Speed < 50.0f)
				{
					Speed += 0.5f;
				}

				if (AccelerationRun < 1.0f)
				{
					AccelerationRun += 0.25f;
				}
			}
			else
			{
				if (!IsFootstepSound)
				{
					FootstepSound->SetSound(WalkSound);
					FootstepSound->Play(0.0f);

					IsFootstepSound = true;

				}

				if (Speed < 25.0f)
				{
					Speed += 1.0f;
				}
				else if (Speed > 25.0f)
				{
					Speed -= 1.0f;
				}
				if (AccelerationRun > 0.5f)
				{
					AccelerationRun -= 0.01f;
				}
			}
		}
		else
		{
			if (!IsFootstepSound)
			{
				FootstepSound->SetSound(WalkSound);
				FootstepSound->Play(0.0f);

				IsFootstepSound = true;

			}

			if (Speed > -50.0f)
			{
				Speed -= 1.0f;
			}
			if (AccelerationRun > 0.5f)
			{
				AccelerationRun = 0.5f;
			}
		}

		AddMovementInput(GetActorForwardVector(), Value*AccelerationRun);

	}
	else
	{
		FootstepSound->Stop();
		IsFootstepSound = false;

		if (Speed > 0.0f)
		{
			Speed -= 1.0f;
		}
		else if (Speed < 0.0f)
		{
			Speed += 1.0f;
		}
		if (AccelerationRun > 0.5f)
		{
			AccelerationRun = 0.5f;
		}
	}

}

void AEntity::WalkRight(float Value)
{
	if (Value != 0.0f && !IsMelee && !IsChangeWeapon && Life > 0)
	{
		if (!IsFootstepSound)
		{
			FootstepSound->SetSound(WalkSound);
			FootstepSound->Play(0.0f);

			IsFootstepSound = true;

		}

		if (Value < 0.0f)
		{
			if (Turn < 25.0f)
			{
				Turn += 0.5f;
			}
		}
		else
		{
			if (Turn > -25.0f)
			{
				Turn -= 0.5f;
			}
		}

		AddMovementInput(GetActorRightVector(), Value*0.5f);

	}
	else
	{
		if (Turn > 0.0f)
		{
			Turn -= 1.0f;
		}
		else if (Turn < 0.0f)
		{
			Turn += 1.0f;
		}
	}

}

void AEntity::Run()
{
	if (!IsAimBow && !IsBlock)
	{
		IsRun = true;

		IsFootstepSound = false;

	}

}

void AEntity::StopRun()
{
	IsRun = false;

	IsFootstepSound = false;

}

void AEntity::JumpAnim()
{
	if (!IsAimBow)
	{
		Jump();

		IsJump = true;

		FootstepSound->Stop();

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AEntity::StopJump, 0.1f, false);

	}

}

void AEntity::StopJump()
{
	IsJump = false;

	IsFootstepSound = false;

	GetWorld()->GetTimerManager().ClearTimer(Handle);

}

void AEntity::ChangeWeapon()
{
	if (!IsAimBow && !IsBlock && !IsMelee && !IsShootBow && CoolDown <= 0 && Life > 0)
	{
		IsChangeWeapon = true;

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AEntity::StopChangeWeapon, 0.2f, false);

	}

}

void AEntity::StopChangeWeapon()
{
	IsChangeWeapon = false;

	if (Weapon1 != NULL && Weapon2 != NULL)
	{
		if (Weapon1->GetClass()->IsChildOf(ABow::StaticClass()))
		{
			TSubclassOf<class AShield> shieldClass = TSubclassOf<AShield>(AShield::StaticClass());
			TSubclassOf<class AAxe> axeClass = TSubclassOf<AAxe>(AAxe::StaticClass());
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			Weapon1->Destroy();
			Weapon2->Destroy();

			AShield* shield = GetWorld()->SpawnActor<AShield>(shieldClass, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), ActorSpawnParams);
			AAxe* axe = GetWorld()->SpawnActor<AAxe>(axeClass, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), ActorSpawnParams);

			shield->Generated(SkeletalMesh);
			axe->Generated(SkeletalMesh);

			Weapon1 = shield;
			Weapon2 = axe;

			BaseTurnRate = 45.0f;
			BaseLookUpRate = 45.0f;
		}
		else
		{
			TSubclassOf<class ABow> bowClass = TSubclassOf<ABow>(ABow::StaticClass());
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			Weapon1->Destroy();
			Weapon2->Destroy();

			ABow* bow = GetWorld()->SpawnActor<ABow>(bowClass, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), ActorSpawnParams);

			bow->Generated(SkeletalMesh, false);

			Weapon1 = bow;
			Weapon2 = bow;

		}

		CoolDown = 1;
		IsTimeActive = true;
	}

	GetWorld()->GetTimerManager().ClearTimer(Handle);

}

void AEntity::UpdateLife(int nLife)
{
	Life += nLife;

	if (Life <= 0)
	{

		TArray<AActor*> attachActors;
		GetAttachedActors(attachActors);

		for (AActor* a : attachActors)
		{
			a->Destroy();

		}

		NumLifes--;

		SkeletalMesh->SetVisibility(false);
		SetActorEnableCollision(false);

		BaseTurnRate = 45.0f;
		BaseLookUpRate = 45.0f;

		Speed = 0.0f;
		Turn = 0.0f;

		IsRun = false;
		AccelerationRun = 0.5f;

		IsAimBow = false;
		IsShootBow = false;
		IsMelee = false;
		IsBlock = false;
		IsChangeWeapon = false;
		IsJump = false;
		IsMeleeHit = false;

		Time = 0.0f;
		CoolDown = 0;
		CurrentArrow = NULL;
		Weapon1 = NULL;
		Weapon2 = NULL;
		NCharge = 0;
		IsTimeActive = false;

		if (NumLifes <= 0)
		{
			UMyGameInstance* gi = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			gi->SaveEntityScore(NameEntity + ";" + FString::FromInt(Score));

			Destroy();

		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(Handle2, this, &AEntity::Respawn, 5.0f, false);

		}

	}
	else
	{
		UMaterialInstanceDynamic* material = SkeletalMesh->CreateAndSetMaterialInstanceDynamic(0);
		material->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(1.0f, 0.0f, 0.0f));

		GetWorld()->GetTimerManager().SetTimer(Handle2, this, &AEntity::StopRed, 0.5f, false);
	}

}

void AEntity::StopRed()
{
	TWeakObjectPtr<UMaterialInstanceDynamic> material = (SkeletalMesh)->CreateAndSetMaterialInstanceDynamic(0);
	material->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0.0f, 0.0f, 0.0f));

	GetWorld()->GetTimerManager().ClearTimer(Handle2);

}

void AEntity::Respawn()
{
	while (!SpawnLocation.Equals(GetActorLocation(), 100.0f))
	{
		TeleportTo(SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), false, true);

	}

	Life = 200;
	AmmoArrow = 8;

	TSubclassOf<class ABow> bowClass = TSubclassOf<ABow>(ABow::StaticClass());
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABow* bow = GetWorld()->SpawnActor<ABow>(bowClass, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), ActorSpawnParams);

	bow->Generated(SkeletalMesh, false);

	Weapon1 = bow;
	Weapon2 = bow;

	SkeletalMesh->SetVisibility(true);
	SetActorEnableCollision(true);

	TWeakObjectPtr<UMaterialInstanceDynamic> material = (SkeletalMesh)->CreateAndSetMaterialInstanceDynamic(0);
	material->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0.0f, 0.0f, 0.0f));

	GetWorld()->GetTimerManager().ClearTimer(Handle2);

}

float AEntity::GetSpeed()
{
	return Speed;

}

float AEntity::GetTurn()
{
	return Turn;

}

bool AEntity::GetIsAimBow()
{
	return IsAimBow;

}

bool AEntity::GetIsShootBow()
{
	return IsShootBow;

}

bool AEntity::GetIsMelee()
{
	return IsMelee;

}

bool AEntity::GetIsBlock()
{
	return IsBlock;

}

bool AEntity::GetIsChangeWeapon()
{
	return IsChangeWeapon;

}

bool AEntity::GetIsJump()
{
	return IsJump;

}

bool AEntity::GetIsMeleeHit()
{
	return IsMeleeHit;

}

int AEntity::GetLife()
{
	return Life;

}

int AEntity::GetNumLifes()
{
	return NumLifes;

}

int AEntity::GetCooldown()
{
	return CoolDown;

}

FLinearColor AEntity::GetFlagColor()
{
	return FlagColor;

}

FVector AEntity::GetSpawnLocation()
{
	return SpawnLocation;

}

int AEntity::GetScore()
{
	return Score;

}

FString AEntity::GetNameEntity()
{
	return NameEntity;

}

int AEntity::GetAmmoArrow()
{
	return AmmoArrow;

}

void AEntity::SetIsMelee(bool nIsMelee)
{
	IsMelee = nIsMelee;

}

void AEntity::SetIsMeleeHit(bool nIsMeleeHit)
{
	IsMeleeHit = nIsMeleeHit;

	if (IsMeleeHit)
	{
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AEntity::StopMeleeHit, 1.0f, false);

	}

}

void AEntity::StopMeleeHit()
{
	IsMeleeHit = false;

	GetWorld()->GetTimerManager().ClearTimer(Handle);

}

void AEntity::SetFlagColor(FLinearColor nFlagColor)
{
	FlagColor = nFlagColor;

}

void AEntity::SetSpawnLocation(FVector nSpawnLocation)
{
	SpawnLocation = nSpawnLocation;

}

void AEntity::SetNameEntity(FString nNameEntity)
{
	NameEntity = nNameEntity;

}

void AEntity::UpdateAmmoArrow(int nAmmoArrow)
{
	AmmoArrow += nAmmoArrow;

}

void AEntity::SetLife(int nLife)
{
	Life = nLife;

	UpdateLife(Life);

}

void AEntity::SetNumLifes(int nNumLifes)
{
	NumLifes = nNumLifes;

}

void AEntity::UpdateScore(int nScore)
{
	Score += nScore;

}

void AEntity::SetSpeed(float nSpeed)
{
	Speed = nSpeed;

}

void AEntity::SetTurn(float nTurn)
{
	Turn = nTurn;

}

void AEntity::SetIsAimBow(bool nIsAimBow)
{
	IsAimBow = nIsAimBow;

}

void AEntity::SetIsShootBow(bool nIsShootBow)
{
	IsShootBow = nIsShootBow;

}

void AEntity::SetIsBlock(bool nIsBlock)
{
	IsBlock = nIsBlock;

}

void AEntity::SetIsChangeWeapon(bool nIsChangeWeapon)
{
	IsChangeWeapon = nIsChangeWeapon;

}

void AEntity::SetIsJump(bool nIsJump)
{
	IsJump = nIsJump;

}
