#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI_Monster/MonsterSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;


UCLASS()
class TEAM15CH3PROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* MonsterDataTable;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomMonster();

	FVector GetRandomPointInVolume() const;
	FMonsterSpawnRow* GetRandomMonster() const;
	AActor* SpawnMonster(TSubclassOf<AActor> Monster);
};
