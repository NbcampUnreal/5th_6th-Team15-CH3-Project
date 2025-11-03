#include "Skill/Actor/Drone.h"
#include "Kismet/GameplayStatics.h"
#include "AI_Monster/AI_Monsters.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

    DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
    SetRootComponent(DroneMesh);

    DroneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();
    LastAttackTime = -AttackInterval;
}

void ADrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!OrbitTarget) return;

    CurrentAngle += OrbitSpeed * DeltaTime;
    if (CurrentAngle >= 360.f) CurrentAngle -= 360.f;

    float Rad = FMath::DegreesToRadians(CurrentAngle);
    FVector Offset = FVector(FMath::Cos(Rad) * OrbitRadius, FMath::Sin(Rad) * OrbitRadius, 50.f);
    SetActorLocation(OrbitTarget->GetActorLocation() + Offset);

    if (GetWorld()->GetTimeSeconds() - LastAttackTime >= AttackInterval)
    {
        AttackNearbyEnemies();
        LastAttackTime = GetWorld()->GetTimeSeconds();
    }
}
void ADrone::AttackNearbyEnemies()
{
    if (!OwnerCharacter) return;

    FVector MyLocation = GetActorLocation();
    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*> Monsters;
    UGameplayStatics::GetAllActorsOfClass(World, AAI_Monsters::StaticClass(), Monsters);

    for (AActor* Actor : Monsters)
    {
        AAI_Monsters* Monster = Cast<AAI_Monsters>(Actor);
        if (!Monster || Monster->IsDead()) continue;

        if (FVector::Dist(MyLocation, Monster->GetActorLocation()) <= AttackRadius)
        {
            if (BulletClass)
            {
                FRotator ShootRot = (Monster->GetActorLocation() - MyLocation).Rotation();
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                AMonsterBullet* Bullet = World->SpawnActor<AMonsterBullet>(BulletClass, MyLocation, ShootRot, SpawnParams);
                if (Bullet)
                {
                    Bullet->FireInDirection(ShootRot.Vector());
                }
            }
        }
    }
}

void ADrone::SetOrbitTarget(AActor* Target)
{
    OrbitTarget = Target;
}
