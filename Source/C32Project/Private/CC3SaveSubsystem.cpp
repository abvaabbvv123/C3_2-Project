
#include "CC3SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"




void UCC3SaveSubsystem::SetSavePoint(const FTransform& NewTransform) {
	LastSavePoint = NewTransform;
	bHasSavePoint = true;
}


void UCC3SaveSubsystem::RespawnPawn(APawn* Respawn) const {
	if (!IsValid(Respawn)) {
		return;
	}
	//set Save Point Location
	FVector TargetLocation;
	FRotator TargetRotation;
	if (bHasSavePoint) {
		TargetLocation = LastSavePoint.GetLocation();
		TargetRotation = LastSavePoint.GetRotation().Rotator();
	}
	// no save point => go to player start
	else {
		AActor* PlayerStart = UGameplayStatics::GetActorOfClass(Respawn->GetWorld(), APlayerStart::StaticClass());
		TargetLocation = PlayerStart->GetActorLocation();
		TargetRotation = PlayerStart->GetActorRotation();
	}
	// make actor stop first -> teleport
	if (UMovementComponent* MovementComponent = Respawn->GetMovementComponent()) {
		MovementComponent->StopMovementImmediately();
	}
	//bool TeleportTo( location XYZ, Rotation pitch,Yaw,Roll, Test(Only check Can teleport or not (return as bool),check stuck or not(true=teleport anyway, false = if stuck, no teleport)
	Respawn->TeleportTo(TargetLocation, TargetRotation, false, true);

}

bool UCC3SaveSubsystem::HasSavePoint() {
	return bHasSavePoint;
}














