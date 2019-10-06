#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Bow.h"
#include "Components/InputComponent.h"
#include "Arrow.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Shield.h"
#include "Axe.h"

AMyCharacter::AMyCharacter()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SkeletalMesh);
	Camera->SetupAttachment(RootComponent);
	
	Camera->SetRelativeLocation(FVector(50.0f, 25.0, 85.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	Camera->FieldOfView = 100.0f;

	Camera->bUsePawnControlRotation = true;

	Rotate = FRotator(0.0f, 0.0f, 0.0f);

	NView = 0;

}

void AMyCharacter::BeginPlay()
{
	ACharacter::BeginPlay();

	Camera->AttachToComponent(SkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), "HeadTop_EndSocketCamera");

	TSubclassOf<class ABow> bowClass = TSubclassOf<ABow>(ABow::StaticClass());
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABow* bow = GetWorld()->SpawnActor<ABow>(bowClass, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), ActorSpawnParams);

	bow->Generated(SkeletalMesh, false);

	Weapon1 = bow;
	Weapon2 = bow;

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AEntity::JumpAnim);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AEntity::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AEntity::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AEntity::StopRun);

	PlayerInputComponent->BindAction("BlockOrAim", IE_Pressed, this, &AMyCharacter::BlockOrAim);
	PlayerInputComponent->BindAction("BlockOrAim", IE_Released, this, &AMyCharacter::StopBlockOrAim);

	PlayerInputComponent->BindAction("MeleeOrShoot", IE_Pressed, this, &AMyCharacter::MeleeOrShoot);

	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &AEntity::ChangeWeapon);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("WalkForward", this, &AEntity::WalkForward);
	PlayerInputComponent->BindAxis("WalkRight", this, &AEntity::WalkRight);

	PlayerInputComponent->BindAxis("TurnXAxis", this, &AMyCharacter::TurnXAxis);
	PlayerInputComponent->BindAxis("TurnYAxis", this, &AMyCharacter::TurnYAxis);

}

void AMyCharacter::TurnYAxis(float Rate)
{
	if (Rate != 0.0f)
	{
		if (IsAimBow) 
		{
			Rotate = FRotator(0.0f, 5.0f, GetActorRotation().Pitch - GetControlRotation().Pitch - 5.0f);
		}
		else
		{
			Rotate = FRotator(0.0f, 0.0f, GetActorRotation().Pitch - GetControlRotation().Pitch);
		}

		AddControllerPitchInput(-Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());

	}

}

void AMyCharacter::TurnXAxis(float Rate)
{
	if (Rate != 0.0f)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

	}

}

void AMyCharacter::BlockOrAim()
{
	if (!IsJump && CoolDown <= 0 && Life > 0 && Weapon1 != NULL)
	{
		IsRun = false;

		if (Weapon1->GetClass()->IsChildOf(ABow::StaticClass()) )
		{
			IsAimBow = true;
			IsShootBow = false;

			Rotate = FRotator(0.0f, 5.0f, GetActorRotation().Pitch - GetControlRotation().Pitch - 5.0f);

			if (AmmoArrow > 0)
			{
				if (NCharge <= 0)
				{
					ArrowClass = TSubclassOf<AArrow>(AArrow::StaticClass());

					if (ArrowClass != NULL)
					{
						UWorld* const World = GetWorld();

						if (World != NULL)
						{
							const FRotator SpawnRotation = SkeletalMesh->GetSocketRotation("LeftHandIndex1SocketArrow");
							const FVector SpawnLocation = SkeletalMesh->GetSocketLocation("LeftHandIndex1SocketArrow");

							FActorSpawnParameters ActorSpawnParams;
							ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

							CurrentArrow = World->SpawnActor<AArrow>(ArrowClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

							CurrentArrow->AttachToComponent(SkeletalMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), "LeftHandIndex1SocketArrow");

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

			}

			GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMyCharacter::ReduceView, 0.01f, false);

		}
		else
		{
			IsBlock = true;

			BaseLookUpRate = 22.5f;
			BaseTurnRate = 22.5f;

			AShield* s = Cast<AShield>(Weapon1);
			s->Action();

		}
	}

}

void AMyCharacter::ReduceView()
{
	if (NCharge < 140)
	{
		NCharge++;

		if(CurrentArrow != NULL && AmmoArrow > 0)
		{
			CurrentArrow->SetActorRelativeLocation(FVector(0.0f, -NCharge / 2, 0.0f));

		}

		if (NView < 90)
		{
			NView++;

			Camera->FieldOfView -= 0.5f;

			BaseTurnRate -= 0.25f;
			BaseLookUpRate -= 0.25f;
		}

		GetWorld()->GetTimerManager().ClearTimer(Handle);

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMyCharacter::ReduceView, 0.005f, false);

	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);

	}

}

void AMyCharacter::StopBlockOrAim()
{
	if (Life > 0 && Weapon1 != NULL)
	{

		if (Weapon1->GetClass()->IsChildOf(ABow::StaticClass()))
		{
			Rotate = FRotator(0.0f, 0.0f, 0.0f);

			BowSound->Stop();

			IsAimBow = false;

			GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMyCharacter::IncreaseView, 0.01f, false);

		}
		else
		{
			IsBlock = false;

			BaseLookUpRate = 45.0f;
			BaseTurnRate = 45.0f;

			AShield* s = Cast<AShield>(Weapon1);
			s->Undo();

		}
	}

}

void AMyCharacter::IncreaseView()
{
	if (NView > 0)
	{
		NView--;

		Camera->FieldOfView += 0.5f;

		BaseTurnRate += 0.25f;
		BaseLookUpRate += 0.25f;

		if (NCharge > 0)
		{
			NCharge -= 2;

			if (CurrentArrow != NULL && AmmoArrow > 0)
			{
				CurrentArrow->SetActorRelativeLocation(FVector(0.0f, -NCharge / 2, 0.0f));

				if (NCharge <= 0)
				{
					CurrentArrow->Destroy();

				}
			}
		}

		GetWorld()->GetTimerManager().ClearTimer(Handle);

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMyCharacter::IncreaseView, 0.005f, false);

	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);

	}

}

void AMyCharacter::MeleeOrShoot()
{
	if (Life > 0)
	{
		if (IsAimBow)
		{
			IsShootBow = true;

			IsAimBow = false;

			CoolDown = 1;
			IsTimeActive = true;

			if (AmmoArrow > 0)
			{
				FVector velocity = (Camera->GetComponentRotation() - FRotator(-3.0f, 1.0f, 0.0f)).Vector();

				if (CurrentArrow != NULL)
				{
					CurrentArrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

					CurrentArrow->Shoot(velocity, NCharge);

					AmmoArrow--;

				}

				NCharge = 0;
			}

			BowSound->Stop();

			GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMyCharacter::StopShoot, 0.2f, false);

		}
		else
		{
			if (Weapon2 != NULL)
			{
				if (Weapon2->GetClass()->IsChildOf(AAxe::StaticClass()) && CoolDown <= 0 && !IsBlock)
				{
					IsMelee = true;

					AAxe* a = Cast<AAxe>(Weapon2);

					a->Overlap = true;

					TimeMelee = 2;

					GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMyCharacter::StopMelee, 0.2f, false);

				}
			}
		}
	}

}

void AMyCharacter::StopShoot()
{
	IsAimBow = false;
	IsShootBow = false;

	StopBlockOrAim();

	GetWorld()->GetTimerManager().ClearTimer(Handle);

}

void AMyCharacter::StopMelee()
{
	IsMelee = false;

	CoolDown = 1;
	IsTimeActive = true;

	GetWorld()->GetTimerManager().ClearTimer(Handle);

}

FRotator AMyCharacter::GetRotate()
{
	return Rotate;

}

void AMyCharacter::SetRotate(FRotator nRotate)
{
	Rotate = nRotate;

}
