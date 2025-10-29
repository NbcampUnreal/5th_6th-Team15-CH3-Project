#include "AI_Monster/SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"



ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Monster"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
	MonsterDataTable = nullptr;
}
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	//////////////////////////////////////////////////////////// 게임 스테이터스에서 사용해야하는데 테스트로 사용중.
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 10;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomMonster();
			}
		}

	}
	////////////////////////////////////////////////////////////스폰 시키는 코드.
}

AActor* ASpawnVolume::SpawnRandomMonster()
{
	if (FMonsterSpawnRow* SelectedRow = GetRandomMonster())
	{
		if (UClass* ActualClass = SelectedRow->MonsterClass.Get())
		{
			return SpawnMonster(ActualClass);
		}
	}
	return nullptr;
}

FMonsterSpawnRow* ASpawnVolume::GetRandomMonster() const
{
	if (!MonsterDataTable) return nullptr;

	TArray<FMonsterSpawnRow*> AllRows;
	static const FString ContextString(TEXT("MonsterSpawnContext"));

	MonsterDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FMonsterSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FMonsterSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}
	return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const //랜덤 위치를 알아내는곳
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}
AActor* ASpawnVolume::SpawnMonster(TSubclassOf<AActor> Monster)

{
	if (!Monster) return nullptr;

	AActor* SpawnedActor =  GetWorld()->SpawnActor<AActor>(
		Monster,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);
	return SpawnedActor;
}

