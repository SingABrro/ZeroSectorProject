// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Input/ZeroInputBaseComponent.h"
#include "ZeroInputNightComponent.generated.h"

class UAnimInstance;
class UAnimMontage;
class AZeroWeaponBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnforceWeapon, int32)

UCLASS()
class ZEROSECTOR_API UZeroInputNightComponent : public UZeroInputBaseComponent
{
	GENERATED_BODY()
	
public:
	UZeroInputNightComponent();
	
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Run() override;
	virtual void Walk() override;

	virtual void Fire() override;
	virtual void ChangeWeapon() override;
	virtual void Reloading() override;
	virtual void SetupWeapon(const EWeaponType& WeaponType) override;
	virtual void SetUnequipWeapon() override;
	virtual EWeaponType GetWeaponType() override;

	virtual void PauseMenu() override;

	static FOnEnforceWeapon OnEnforceWeapon;
	
protected:
	virtual void BeginPlay() override;

private:
	void SetNoWeapon();
	void SetRifle();
	void SetPistol();
	void SetShotgun();
	void SetupTransformWeapon(const FName& SocketName);
	void ChangeWeaponMesh();

	void TryPlayFootstepSound();
	void SetFootstepInterval(float NewInterval);
	
	void EndReloading(UAnimMontage* Target, bool bIsProperlyEnd);

// Weapon Data
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TMap<EWeaponType, AZeroWeaponBase*> Weapons;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<AZeroWeaponBase> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	EWeaponType CurrentWeaponType;

	EWeaponType ChoicedWeapon;
	EWeaponType TempWeaponType;

	FTimerHandle FootstepTimerHandle;
	float CurrentFootstepInterval = 0.5f;

// Anim Data
private:
	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UAnimInstance> Anim;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> PistolFireMontage;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> RifleFireMontage;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> ShotgunFireMontage;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> PistolReloadMontage;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> RifleReloadMontage;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> ShotgunReloadMontage;

	bool bIsCanFire{ true };
};
