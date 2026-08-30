// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "TrapItem.generated.h"

class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class C32PROJECT_API ATrapItem : public ABaseItem
{
	GENERATED_BODY()

public:
	bool bHasExploded;
	FString ItemType;
	ATrapItem();

	//USphereComponent* ExplosionCollision;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Explosion")
	bool bIsExplosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Explosion")
	float ExplosionDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Explosion")
	bool bIsDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Explosion")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Explosion")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Slow")
	bool bIsSlow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Slow")
	float SlowStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Slow")
	float SlowDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Blind")
	bool bIsBlind;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Blind")
	float BlindDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Camera")
	bool bIsCameraFix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Camera")
	float CameraFixDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Reverse")
	bool bIsReverseMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Reverse")
	float MoveReverseDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Reverse")
	bool bIsReverseLook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Reverse")
	float LookReverseDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Random")
	bool AutoRandom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Random")
	float RandomDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options")
	FString LogText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Movement")
	bool bIsFollow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Movement")
	UProjectileMovementComponent* FollowMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Movement")
	float FollowingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options")
	bool bNeedDestroy = true;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Movement")
	//bool bIsMoving;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Movement")
	//bool bIsRandomSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Effects")
	UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Effects")
	USoundBase* ExplosionSound;
	FTimerHandle ExplosionTimerHandle;

	virtual void ActivateItem(AActor* Activator) override;
	virtual void BeginPlay() override;
	void DelayExplode();

};
