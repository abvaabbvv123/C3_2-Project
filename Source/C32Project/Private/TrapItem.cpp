// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapItem.h"
#include "CC3Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ATrapItem::ATrapItem() {

	ExplosionDelay = 0.0f;
	ExplosionRadius = 0.0f;
	ExplosionDamage = 0.0f;
	bHasExploded = false;

//ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
//ExplosionCollision->InitSphereRadius(ExplosionRadius);
//ExplosionCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
//ExplosionCollision->SetupAttachment(Scene);

	FollowMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("FollowMovement"));
	FollowMovement->ProjectileGravityScale = 0.f;
	FollowMovement->bRotationFollowsVelocity = true;
	FollowMovement->bShouldBounce = false;

	ItemType = "Trap";
}
void ATrapItem::BeginPlay() {
	Super::BeginPlay();
	if (bIsFollow) {
		if (ACharacter* PC = UGameplayStatics::GetPlayerCharacter(this, 0)) {
			FollowMovement->bIsHomingProjectile = true;
			FollowMovement->HomingTargetComponent = PC->GetRootComponent();
			FollowMovement->HomingAccelerationMagnitude = FollowingSpeed;
			FollowMovement->MaxSpeed = FollowingSpeed;
			FollowMovement->InitialSpeed = FollowingSpeed;
		}
	}
}

void ATrapItem::ActivateItem(AActor* Activator) {
	Super::ActivateItem(Activator);
	UParticleSystemComponent* Particle = nullptr;
	if (ACC3Character* Player = Cast<ACC3Character>(Activator)) {
		if (bIsSlow) {
			Player->ApplySlow(SlowStrength, SlowDuration, LogText);
		}
		if (bIsBlind) {
			Player->ApplyBlind(BlindDuration, LogText);
		}
		if (bIsCameraFix) {
			Player->ApplyCameraFix(CameraFixDuration, LogText);
		}
		if (bIsReverseMove) {
			Player->ApplyMoveReverse(MoveReverseDuration, LogText);
		}
		if (bIsReverseLook) {
			Player->ApplyLookReverse(LookReverseDuration, LogText);
		}
	}

	if (bIsExplosion && !bIsDelay) {
		TArray<AActor*> OverlappingActors;
		Collision->GetOverlappingActors(OverlappingActors);
	
		for (AActor* Actor : OverlappingActors) {
			if (Actor && Actor->ActorHasTag("Player")) {
				UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, nullptr, this, UDamageType::StaticClass());
			}
		//	if (ACC3Character* Player = Cast<ACC3Character>(Actor)) {
		//		UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, nullptr, this, UDamageType::StaticClass());
		//	}
		}
	}
	else if (bIsDelay) {
		GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &ATrapItem::DelayExplode, ExplosionDelay, false);
	}

	if (ExplosionParticle) {
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), false);
	}
	if (ExplosionSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
	if (bNeedDestroy) {
		DestroyItem();
	}
	if (Particle) {
		FTimerHandle DestroyParticleTimer;

		GetWorld()->GetTimerManager().SetTimer(DestroyParticleTimer, [Particle]() {Particle->DestroyComponent(); }, 2.0f, false);
	}
}

void ATrapItem::DelayExplode() {}