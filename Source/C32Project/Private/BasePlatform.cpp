// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlatform.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABasePlatform::ABasePlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	StaticMesh->SetMobility(EComponentMobility::Movable); // essential for moving
}
void ABasePlatform::BeginPlay() {
	Super::BeginPlay();
	OriginLocation = GetActorLocation();
	OriginRotation = GetActorRotation();

	//bNeedSpawn:: hide until player trigger some switch
	if (bNeedSpawn) {
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
	else if (!bHasSwitch) {
		BeginMove();
	}

}
//didnt work for now@@@
void ABasePlatform::TurnSwitch() {
	BeginMove();
	//bIsTurnOn = !bIsTurnOn;
	//
	//if (bIsTurnOn) {
	//	BeginMove();
	//}
	//else {
	//	StopMoving();
	//}
}
//begin play->hide, when player trigger this, Spawn Platform
void ABasePlatform::SpawnItem() {
	SetActorLocation(OriginLocation);
	SetActorRotation(OriginRotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	BeginMove();
}
void ABasePlatform::DestroyItem() {
	Destroy();
}

//for one of repeat option
void ABasePlatform::StopMoving() {
	bIsMoving = false;
	SetActorTickEnabled(false);

	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
}
//for one of Only Once option
void ABasePlatform::SoftDestroy() {
	StopMoving();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ABasePlatform::BeginMove() {
	//set animation duration
	if (bIsRandom) { RuntimeSpeed = FMath::RandRange(Speed, Speed + RandomSpeedValue); }
	else { RuntimeSpeed = Speed; }

	if (RuntimeSpeed <= 0.0f) {
		RuntimeSpeed = 0.1f;
	}
	//set move, 0: dont work, 0 <: work
	FVector Offset = FVector::ZeroVector;
	if (MoveX != 0.0f) { Offset.X = MoveX; }
	if (MoveY != 0.0f) { Offset.Y = MoveY; }
	if (MoveZ != 0.0f) { Offset.Z = MoveZ; }
	//need to turn on bIsRandom first in Editor, Use RandRange
	if (bIsRandom) {
		if (Offset.X != 0.0f) { Offset.X = FMath::RandRange(Offset.X, Offset.X + RandomMoveValue); }
		if (Offset.Y != 0.0f) { Offset.Y = FMath::RandRange(Offset.Y, Offset.Y + RandomMoveValue); }
		if (Offset.Z != 0.0f) { Offset.Z = FMath::RandRange(Offset.Z, Offset.Z + RandomMoveValue); }
	}
	//set StartLocation -> TargetLocation (aim)
	MoveStartLocation = GetActorLocation();
	//set Target Location, if repeating origin location <-> offset location
	MoveTargetLocation = bNeedReverse ? (OriginLocation + Offset) : OriginLocation;
	//set rotate
	FRotator Rotate = FRotator::ZeroRotator;
	if (RotatePitch != 0.0f) { Rotate.Pitch = RotatePitch; }
	if (RotateYaw != 0.0f) { Rotate.Yaw = RotateYaw; }
	if (RotateRoll != 0.0f) { Rotate.Roll = RotateRoll; }
	if (bIsRandom) {
		if (Rotate.Pitch != 0.0f) { Rotate.Pitch = FMath::RandRange(Rotate.Pitch, Rotate.Pitch + RandomRotateValue); }
		if (Rotate.Yaw != 0.0f) { Rotate.Yaw = FMath::RandRange(Rotate.Yaw, Rotate.Yaw + RandomRotateValue); }
		if (Rotate.Roll != 0.0f) { Rotate.Roll = FMath::RandRange(Rotate.Roll, Rotate.Roll + RandomRotateValue); }
	}
	//set start 0< target
	MoveStartRotation = GetActorRotation();
	MoveTargetRotation = MoveStartRotation + Rotate;

	const bool bHasMove = !Offset.IsNearlyZero();
	const bool bHasRotate = !Rotate.IsNearlyZero();

	if (!bHasMove && !bHasRotate) {
		OnMoveEnded();
		return;
	}
	//if teleport-> move instantly
	if (AnimType == EPlatformAnimType::Teleport) {
		SetActorLocation(MoveTargetLocation);
		SetActorRotation(MoveTargetRotation);
		OnMoveEnded();
		return;
	}
	ElapsedTime = 0.0f;
	bIsMoving = true;
	SetActorTickEnabled(true);
}

void ABasePlatform::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!bIsMoving) {
		return;
	}
	//Check Moved time (Add DeltaTime for consistency)
	ElapsedTime += DeltaTime;
	//Check Moved Percentage (ElapsedTime / RuntimeSpeed =? 0~1)
	float Alpha = FMath::Clamp(ElapsedTime / RuntimeSpeed, 0.0f, 1.0f);
	//if EaseInOut => use interpeaseinout func
	if (AnimType == EPlatformAnimType::EaseInOut) {
		Alpha = FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f);
	}
	//Moving Platform
	SetActorLocation(FMath::Lerp(MoveStartLocation, MoveTargetLocation, Alpha));
	SetActorRotation(FMath::Lerp(MoveStartRotation, MoveTargetRotation, Alpha));
	//alpha > 1 = Move done. OnMoveEnded => repeat or not
	if (Alpha >= 1.0f) {
		bIsMoving = false;
		SetActorTickEnabled(false);
		OnMoveEnded();
	}
}

void ABasePlatform::OnMoveEnded() {
	bNeedReverse = !bNeedReverse;
	//set repeat or not

	if (bOnlyOnce) {
		if (bRepeat) {
			SoftDestroy();
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ABasePlatform::SpawnItem, FMath::Max(RepeatDelay, 0.1f), false);
		}
		else {
			DestroyItem();
		}
		return;
	}
	//if repeat -> wait for timer first -> BeginMove()again. Set Start Location/Rotation agaub when moving done
	if (bRepeat) {
		if (RepeatDelay != 0.0f) {
			GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ABasePlatform::BeginMove, RepeatDelay, false);
		}
		else {
			BeginMove();
		}
	}
}
