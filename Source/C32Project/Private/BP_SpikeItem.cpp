// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_SpikeItem.h"
#include "CC3Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
ABP_SpikeItem::ABP_SpikeItem() {
	PrimaryActorTick.bCanEverTick = true;
	Damage = 0.0f;
	Delay = 0.0f;
	StartDelay = 0.0f;

	ExplosionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);

	ExplosionCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	ExplosionCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

	if (RotateComponent) {
		RotateComponent->DestroyComponent();
		RotateComponent = nullptr;
	}
	bIsSpin = false;
}
void ABP_SpikeItem::BeginPlay() {
	Super::BeginPlay();
	
	OriginLocation = GetActorLocation();
	OriginRotation = GetActorRotation();
		if (Speed == 0.f) {
			return;
		}
		if (StartDelay > 0.f) {
			GetWorldTimerManager().SetTimer(StartDelayTimerHandle, this, &ABP_SpikeItem::BeginMove, StartDelay, false);
		}
		else { 
			BeginMove(); 
		}
}
void ABP_SpikeItem::BeginMove() {
	RuntimeSpeed = Speed;

	if (RuntimeSpeed <= 0.0f) {
		RuntimeSpeed = 0.1f;
	}
	//set move, 0: dont work, 0 <: work
	FVector Offset = FVector::ZeroVector;
	if (MoveX != 0.0f) { Offset.X = MoveX; }
	if (MoveY != 0.0f) { Offset.Y = MoveY; }
	if (MoveZ != 0.0f) { Offset.Z = MoveZ; }

	MoveStartLocation = GetActorLocation();
	MoveTargetLocation = bIsRepeating ? (OriginLocation + Offset) : OriginLocation;

	const bool bHasMove = !Offset.IsNearlyZero();

	if (!bHasMove) {
		OnMoveEnded();
		return;
	}
	//if teleport-> move instantly

	ElapsedTime = 0.0f;
	bIsMoving = true;
	SetActorTickEnabled(true);
}
void ABP_SpikeItem::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!bIsMoving) {
		return;
	}
	ElapsedTime += DeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / RuntimeSpeed, 0.0f, 1.0f);

	SetActorLocation(FMath::Lerp(MoveStartLocation, MoveTargetLocation, Alpha));
	if (Alpha >= 1.0f) {
		bIsMoving = false;
		SetActorTickEnabled(false);
		OnMoveEnded();
	}
}
void ABP_SpikeItem::OnMoveEnded() {
	bIsRepeating = !bIsRepeating;
	if (RepeatDelay >= 0.0f) {
		GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ABP_SpikeItem::BeginMove, RepeatDelay, false);
	}
	else {
		BeginMove();
	}

}
void ABP_SpikeItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnItemOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor && OtherActor->ActorHasTag("Player")) {
		GetWorldTimerManager().ClearTimer(DelayTimerHandle);
		GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &ABP_SpikeItem::TickDamage, DelayTickInterval, true);
	}
}
void ABP_SpikeItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnItemEndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor && OtherActor->ActorHasTag("Player")) {
		GetWorldTimerManager().ClearTimer(DelayTimerHandle);
	}
}
void ABP_SpikeItem::ActivateItem(AActor* Activator) {
//	if (bIsDelay) {
//		return;
//	}
	Super::ActivateItem(Activator);
//	bIsDelay = true;
	if (ACC3Character* Player = Cast<ACC3Character>(Activator)) {
		UGameplayStatics::ApplyDamage(Player, Damage, nullptr, this, UDamageType::StaticClass());
	}
	// GetWorldTimerManager().ClearTimer(RepeatTimerHandle);
//	GetWorldTimerManager().SetTimer(RepeatTimerHandle, this, &ABP_SpikeItem::ResetDelay, DelayTickInterval, true);
}
void ABP_SpikeItem::ResetDelay() {
	bIsDelay = false;
}
void ABP_SpikeItem::TickDamage() {
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors) {
		if (Actor && Actor->ActorHasTag("Player")) {
			UGameplayStatics::ApplyDamage(Actor, Damage, nullptr, this, UDamageType::StaticClass());
		}
	}
}