// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "BP_SpikeItem.generated.h"
class UBoxComponent;
/**
 * 
 */
UCLASS()
class C32PROJECT_API ABP_SpikeItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ABP_SpikeItem();
	UBoxComponent* ExplosionCollision;
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Explosion")
	float Damage = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|Options|Explosion")
	float Delay = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Move")
	float MoveX = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Move")
	float MoveY = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Move")
	float MoveZ = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting", meta = (ClampMin = "1"))
	float Speed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting", meta = (ClampMin = "0.0"))
	float StartDelay = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting", meta = (ClampMin = "0.0"))
	float RepeatDelay = 0.0f;
private:
	void BeginMove();
	void OnMoveEnded();
	void ResetDelay();
	void TickDamage();

	bool bIsMoving = false;
	bool bIsRepeating = true;
	bool bIsDelay = false;
	
	float DelayTickInterval = 1.0f;
	float ElapsedTime = 0.0f;
	float RuntimeSpeed = 1.0f; // animation duration

	FVector OriginLocation = FVector::ZeroVector;
	FRotator OriginRotation = FRotator::ZeroRotator;

	FVector MoveStartLocation = FVector::ZeroVector;
	FVector MoveTargetLocation = FVector::ZeroVector;

	FTimerHandle RepeatTimerHandle;
	FTimerHandle DelayTimerHandle;
	FTimerHandle StartDelayTimerHandle;
};
