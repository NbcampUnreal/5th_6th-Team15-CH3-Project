#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MonsterBullet.generated.h"



UCLASS()
class TEAM15CH3PROJECT_API AMonsterBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	AMonsterBullet();

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	UProjectileMovementComponent* ProjecttileMovementComp;

	void FireInDirection(const FVector& ShootDirection);

};
