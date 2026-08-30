// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Box"));
	Box->SetupAttachment(Scene);

	SpawnChanceDataTable = nullptr;
	ItemSpawnAmount = 0;
}
FVector ASpawnVolume::GetRandomLocation() const {
	FVector BoxScale = Box->GetScaledBoxExtent();
	FVector BoxOrigin = Box->GetComponentLocation();

	return BoxOrigin + FVector(FMath::FRandRange(-BoxScale.X, BoxScale.X), FMath::FRandRange(-BoxScale.Y, BoxScale.Y), FMath::FRandRange(-BoxScale.Z, BoxScale.Z));
}
FItemSpawnRow* ASpawnVolume::GetRandomItem() const {
	if (!SpawnChanceDataTable) {
		return nullptr;
	}
	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	SpawnChanceDataTable->GetAllRows(ContextString, AllRows);
	if (AllRows.IsEmpty()) {
		return nullptr;
	}
	float TotalChance = 0.f;
	for (const FItemSpawnRow* Row : AllRows) {
		if (Row) {
			TotalChance += Row->SpawmChance;
		}
	}
	const float RandLocation = FMath::FRandRange(0.f, TotalChance);
	float AccumulateChange = 0.f;

	for (FItemSpawnRow* Row : AllRows) {
		AccumulateChange += Row->SpawmChance;
		if (RandLocation < AccumulateChange) {
			return Row;
		}
	}
	return nullptr;
}
AActor* ASpawnVolume::SpawnRandomItem() {
	if (FItemSpawnRow* SelectedRow = GetRandomItem()) {
		if (UClass* ActualClass = SelectedRow->ItemClass.Get()) {
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass) {
	if (!ItemClass) {
		return nullptr;
	}
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ItemClass, GetRandomLocation(), FRotator::ZeroRotator);
	return SpawnedActor;
}
