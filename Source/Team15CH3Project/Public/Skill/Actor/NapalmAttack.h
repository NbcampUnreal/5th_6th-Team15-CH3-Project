#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NapalmAttack.generated.h"

UCLASS()
class TEAM15CH3PROJECT_API ANapalmAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	ANapalmAttack();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 备己夸家
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NapalmAttack")
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NapalmAttack")
	UStaticMeshComponent* CastingMeshIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NapalmAttack")
	UStaticMeshComponent* CastingMeshOut;



	// 胶纳老 包访
	UPROPERTY(EditAnywhere, Category = "Skill")
	float GrowSpeed;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float MaxScale;

	float CurrentScale;

	// 气惯 包访
	UPROPERTY(EditAnywhere, Category = "Skill")
	float DamageRadius;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Skill")
	UParticleSystem* ExplosionEffect;

	bool bExploded = false;

	void Explode();
};
