// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CC3Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, NewStamina);
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
class UStaticMeshComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS()
class C32PROJECT_API ACC3Character : public ACharacter
{


	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACC3Character();

	virtual void Tick(float DeltaTime) override;
	//properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	UWidgetComponent* HeadWidget;
	//inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* WalkAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* PauseAction;
	//effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Slow")
	UParticleSystem* SlowEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Sound")
	USoundBase* SlowSound;
	//Delegate
	UPROPERTY(BlueprintAssignable, Category = "Status|Health")
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "STatus|Stamina")
	FOnStaminaChanged OnStaminaChanged;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STatus|Stamina")
	float StaminaCost = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STatus|Stamina")
	float StaminaRegen = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STatus|Stamina")
	float StaminaTickInterval = 0.1f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsMoving = false;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	//bool bIsJumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovingThreshold = 10.f;
	//getter
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetHealth() const;
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetStamina() const;
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetSpeed() const;

	//Debuffs
	void ApplySlow(float Strength, float Duration);
	void ApplyBlind(float Duration);
	void ApplyCameraFix(float Duration);
	void ApplyMoveReverse(float Duration);
	void ApplyLookReverse(float Duration);
	//buffs
	void AddHealth(float amount);
	void AddStamina(float amount);
	void Cleanse();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCasuer) override;

	FTimerHandle StaminaTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentSpeed;
	

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartWalk(const FInputActionValue& value);
	UFUNCTION()
	void StopWalk(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void Pause(const FInputActionValue& value);
	//Revert Debuff
	void RevertSlow();
	void RevertBlind();
	void RevertCameraFix();
	void RevertMoveReverse();
	void RevertLookReverse();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debuff")
	bool bIsSlowed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debuff")
	bool bIsBlinded = false;
	UPROPERTY(Transient) 
	UMaterialInstanceDynamic* BlindDynamicMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debuff")
	bool bIsCamerafixed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debuff")
	bool bIsInputReverse = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debuff")
	bool bIsLookReverse = false;
	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	UMaterialInterface* BlindPostProcessMaterial;

	FTimerHandle SlowTimerHandle;
	FTimerHandle BlindTimerHandle;
	FTimerHandle CameraFixTimerHandle;
	FTimerHandle MoveReverseTimerHandle;
	FTimerHandle LookReverseTimerHandle;
	//Basic

	void OnDeath();
	void UseStamina();
	void RegenStamina();
	bool bIsSprint = false;
	bool bIsWalk = false;
	void UpdateMovement();

private:
	float NormalSpeed;
	float BasicSpeed;
	float SprintMultiplier;
	float WalkMutiplier;
	float SprintSpeed;
	float WalkSpeed;
	float SlowMultiplier;
	bool bIsOnSlow;
	};
