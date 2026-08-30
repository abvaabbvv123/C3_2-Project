// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePlatform.generated.h"

//Enums for select animation basic::EaseInOut
UENUM(BlueprintType)
enum class EPlatformAnimType : uint8 {
	Flat UMETA(DisplayName = "Flat Speed"),
	EaseInOut UMETA(DisplayName = "EasyInOut"),
	Teleport UMETA(DisplayName = "No Animation")
};

UCLASS()
class C32PROJECT_API ABasePlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABasePlatform();


	UFUNCTION(BlueprintCallable, Category = "Platform")
	virtual void TurnSwitch();
	UFUNCTION(BlueprintCallable, Category = "Platform")
	virtual void SpawnItem();
	UFUNCTION(BlueprintCallable, Category = "Platform")
	virtual void DestroyItem();



protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//architecture: Uproperty -> move, rotate, speed, repeat, set start location/rotation at beginplay
	//set Origin location/rotation with function at BeginPlay, and moving with Lerp function at Tick.
	//move => Set Start Location/Rotation with Origin Location -> go to Target Location/Rotation (Origin + Offsets)
	//repeat => Set New Start Location/Rotation after complete moving -> go to Origin Location/Rotation

	//Appearance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	UStaticMeshComponent* StaticMesh;

	//for Move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Move")
	float MoveX = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Move")
	float MoveY = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Move")
	float MoveZ = 0.0f;
	//for rotating
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Rotate")
	float RotateYaw = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Rotate")
	float RotatePitch = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Rotate")
	float RotateRoll = 0.0f;
	//move settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting", meta = (ClampMin = "1"))
	float Speed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting")
	bool bRepeat = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting", meta = (ClampMin = "0.0"))
	float RepeatDelay = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting")
	bool bOnlyOnce = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting")
	bool bHasSwitch = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting")
	bool bNeedSpawn = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Setting")
	EPlatformAnimType AnimType = EPlatformAnimType::EaseInOut;

	//Random options
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Random")
	bool bIsRandom = false; // need to turn on this in editor if want to use random 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Random")
	float RandomMoveValue = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Random")
	float RandomRotateValue = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform|Random")
	float RandomSpeedValue = 0.0f;


private:
	void BeginMove();
	void OnMoveEnded();
	void SoftDestroy(); // for Reapeat + bOnlyOnce
	void StopMoving();

	bool bIsMoving = false;
	bool bNeedReverse = true; // when repeat, moved platforms need to back to da originLocation

	float ElapsedTime = 0.0f;
	float RuntimeSpeed = 1.0f; // animation duration

	FVector OriginLocation = FVector::ZeroVector;
	FRotator OriginRotation = FRotator::ZeroRotator;

	FVector MoveStartLocation = FVector::ZeroVector;
	FRotator MoveStartRotation = FRotator::ZeroRotator;
	FVector MoveTargetLocation = FVector::ZeroVector;
	FRotator MoveTargetRotation = FRotator::ZeroRotator;

	FTimerHandle RepeatTimerHandle;
	FTimerHandle RespawnTimerHandle;
};
