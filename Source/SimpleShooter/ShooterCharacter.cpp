// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_CONTEXT
	(TEXT("/Game/input/IMC_Default.IMC_Default"));
	if (DEFAULT_CONTEXT.Succeeded())
		DefaultInputContext = DEFAULT_CONTEXT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_MOVEMENT
	(TEXT("/Game/input/IA_Movement.IA_Movement"));
	if(IA_MOVEMENT.Succeeded()) MovementInput = IA_MOVEMENT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_JUMP
	(TEXT("/Game/input/IA_Jump.IA_Jump"));
	if(IA_JUMP.Succeeded()) JumpInput = IA_JUMP.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_CONTROLLERPITCH
	(TEXT("/Game/input/IA_ControllerPitch.IA_ControllerPitch"));
	if(IA_CONTROLLERPITCH.Succeeded()) ControllerPitchInput = IA_CONTROLLERPITCH.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_CONTROLLERYAW
	(TEXT("/Game/input/IA_ControllerYaw.IA_ControllerYaw"));
	if(IA_CONTROLLERYAW.Succeeded()) ControllerYawInput = IA_CONTROLLERYAW.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOKUPRATE
	(TEXT("/Game/input/IA_LookUpRate.IA_LookUpRate"));
	if(IA_LOOKUPRATE.Succeeded()) LookUpRateInput = IA_LOOKUPRATE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LOOKRIGHTRATE
	(TEXT("/Game/input/IA_LookRightRate.IA_LookRightRate"));
	if(IA_LOOKRIGHTRATE.Succeeded()) LookRightRateInput = IA_LOOKRIGHTRATE.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SHOOT
	(TEXT("/Game/input/IA_Shoot.IA_Shoot"));
	if(IA_SHOOT.Succeeded()) ShootInput = IA_SHOOT.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_SWITCHWEAPON
	(TEXT("/Game/input/IA_SwitchWeapon.IA_SwitchWeapon"));
	if(IA_SWITCHWEAPON.Succeeded()) SwitchInput = IA_SWITCHWEAPON.Object;

	GunBoneName.Add(TEXT("RifleRoot"));
	GunBoneName.Add(TEXT("LauncherRoot"));


}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if ((PlayerController = Cast<APlayerController>(GetController())) != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			SubSystem->AddMappingContext(DefaultInputContext, 0);
	}

	Gun.Add(GetWorld()->SpawnActor<AGun>(GunClass));
	Gun.Add(GetWorld()->SpawnActor<AGun>(LauncherClass));

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun[0]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weaponSocket"));
	Gun[0]->SetOwner(this);
	Gun[1]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weaponSocket"));
	Gun[1]->SetOwner(this);
	if(Gun[1] != nullptr)
	{
		Gun[1]->GetMesh()->HideBoneByName(GunBoneName[1], EPhysBodyOp::PBO_None);
	}
	Hp = MaxHp;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(
			MovementInput,
			ETriggerEvent::Triggered,
			this,
			&AShooterCharacter::Move);

		EnhancedInputComponent->BindAction(
			JumpInput,
			ETriggerEvent::Triggered,
			this,
			&AShooterCharacter::Jump);

		EnhancedInputComponent->BindAction(
			JumpInput,
			ETriggerEvent::Triggered,
			this,
			&ACharacter::Jump);

		EnhancedInputComponent->BindAction(
			ControllerPitchInput,
			ETriggerEvent::Triggered,
			this,
			&AShooterCharacter::LookUp);

		EnhancedInputComponent->BindAction(
			ControllerYawInput,
			ETriggerEvent::Triggered,
			this,
			&AShooterCharacter::LookRight);

		EnhancedInputComponent->BindAction(
			LookUpRateInput,
			ETriggerEvent::Triggered,
			this,
			&AShooterCharacter::LookUpRate);

		EnhancedInputComponent->BindAction(
			LookRightRateInput,
			ETriggerEvent::Triggered,
			this,
			&AShooterCharacter::LookRightRate);

		EnhancedInputComponent->BindAction(
			ShootInput,
			ETriggerEvent::Triggered,
			this,
			&AShooterCharacter::Shoot);
		EnhancedInputComponent->BindAction(
			SwitchInput,
			ETriggerEvent::Triggered,
			this,
			&AShooterCharacter::SwitchWeapon);
	}
}

void AShooterCharacter::Move(const FInputActionValue& Value)
{
	AddMovementInput(Value.Get<FVector>().X*GetActorForwardVector()*MovementSpeed);
	AddMovementInput(Value.Get<FVector>().Y*GetActorRightVector()*MovementSpeed);
}

void AShooterCharacter::LookUp(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<float>());
}

void AShooterCharacter::LookRight(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>());
}

void AShooterCharacter::LookUpRate(const FInputActionValue& Value)
{
	AddControllerPitchInput(
		Value.Get<float>() * RotationRate * (GetWorld()->GetDeltaSeconds()) );
}

void AShooterCharacter::LookRightRate(const FInputActionValue& Value)
{
	AddControllerYawInput(
		Value.Get<float>() * RotationRate * (GetWorld()->GetDeltaSeconds()) );
}

void AShooterCharacter::SwitchWeapon(const FInputActionValue &Value)
{
	Gun[ActiveGunIndex]->GetMesh()->HideBoneByName(GunBoneName[ActiveGunIndex], EPhysBodyOp::PBO_None);
	ActiveGunIndex = (ActiveGunIndex + 1) % Gun.Num();
	Gun[ActiveGunIndex]->GetMesh()->UnHideBoneByName(GunBoneName[ActiveGunIndex]);
}

void AShooterCharacter::Shoot()
{
	Gun[ActiveGunIndex]->PullTrigger();
}

float AShooterCharacter::TakeDamage(
	float DamageAmount, 
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	if(DamageCauser == Gun[ActiveGunIndex]) return 0;
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Hp, DamageToApply);
	Hp -= DamageToApply;
	UE_LOG(LogTemp, Display, TEXT("ShooterCharacter Hp: %f"), Hp);

	if(IsDead())
	{

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
	}

	return DamageToApply;
}

bool AShooterCharacter::IsDead() const
{
	return (Hp <= 0);
}

float AShooterCharacter::GetHealthPercent() const
{

    return Hp / MaxHp;
}
