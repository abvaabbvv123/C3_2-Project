#include "BaseItem.h"
#include "ItemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

	RotateComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotate Movement"));
	RotateComponent->RotationRate = Spining;
	RotateComponent->bRotationInLocalSpace = true;
	RotateComponent->SetComponentTickEnabled(false);

}
void ABaseItem::BeginPlay() {
	Super::BeginPlay();
	if (RotateComponent) {
		RotateComponent->SetComponentTickEnabled(bIsSpin);
	}
}
void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor && OtherActor->ActorHasTag("Player")) {

		ActivateItem(OtherActor);
	}
}
void ABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}
void ABaseItem::ActivateItem(AActor* Activator) {
	UParticleSystemComponent* Particle = nullptr;
	if (PickupParticle) {
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupParticle, GetActorLocation(), GetActorRotation(), true);
	}
	if (PickupSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	}
	//if (Particle) {
	//	FTimerHandle DestroyParticleTimer;
	//
	//	GetWorld()->GetTimerManager().SetTimer(DestroyParticleTimer, [Particle]() {Particle->DestroyComponent(); }, 2.0f, false);
	//}
}
FName ABaseItem::GetItemType() const {
	return ItemType;
}

void ABaseItem::DestroyItem() {
	Destroy();

}