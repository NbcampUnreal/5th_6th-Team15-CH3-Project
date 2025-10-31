#include "AI_Monster/MonsterBullet.h"



AMonsterBullet::AMonsterBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComp->InitSphereRadius(15.0f);
	RootComponent = CollisionComp;
	// ÃÑ¾Ë ¼Óµµ
	ProjecttileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjecttileMovementComp->SetUpdatedComponent(CollisionComp);
	ProjecttileMovementComp->InitialSpeed = 3000.0f;
	ProjecttileMovementComp->MaxSpeed = 3000.0f;
	ProjecttileMovementComp->bRotationFollowsVelocity = true;
	ProjecttileMovementComp->bShouldBounce = true;
	ProjecttileMovementComp->Bounciness = 0.5f;


	InitialLifeSpan = 3.0f;
}


void AMonsterBullet::FireInDirection(const FVector& ShootDirection)
{
	ProjecttileMovementComp->Velocity = ShootDirection * ProjecttileMovementComp->InitialSpeed;
}
