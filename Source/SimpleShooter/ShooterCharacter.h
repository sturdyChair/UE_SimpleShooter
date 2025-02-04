// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

private:
	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputMappingContext* DefaultInputContext;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* MovementInput;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* JumpInput;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* ControllerPitchInput;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* ControllerYawInput;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* LookUpRateInput;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* LookRightRateInput;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* ShootInput;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	class UInputAction* SwitchInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float RotationRate = 60.f;

	class APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> GunClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> LauncherClass;


	UPROPERTY()
	TArray<AGun*> Gun;
	int ActiveGunIndex = 0;
	TArray<FName> GunBoneName;

	UPROPERTY(EditAnywhere)
	float MaxHp = 100;

	float Hp = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void LookUp(const FInputActionValue& Value);

	void LookRight(const FInputActionValue& Value);

	void LookUpRate(const FInputActionValue& Value);

	void LookRightRate(const FInputActionValue& Value);

	void SwitchWeapon(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Shoot();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
};
