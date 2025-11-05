// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMade/AutoAttackComponent.h"
#include "PlayerMade/PlayerCharacter.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "PlayerMade/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AI_Monster/AI_Monsters.h"

UAutoAttackComponent::UAutoAttackComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAutoAttackComponent::BeginPlay()
{
    Super::BeginPlay();

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn)
    {
        StatsComponent = OwnerPawn->FindComponentByClass<UCharacterStatsComponent>();
    }

    if (StatsComponent && ProjectileClass)
    {
        StartAutoAttack();
    }
}

void UAutoAttackComponent::StopAutoAttack()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
        UE_LOG(LogTemp, Warning, TEXT("Auto Attack Stopped by PlayerIsDead event."));
    }
}


// ====================================================================
// 공격 제어 로직
// ====================================================================

void UAutoAttackComponent::StartAutoAttack()
{
    float AttackInterval = CalculateAttackInterval();

    GetWorld()->GetTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &UAutoAttackComponent::FireProjectile,
        AttackInterval,
        true
    );
}

float UAutoAttackComponent::CalculateAttackInterval() const
{
    if (StatsComponent && StatsComponent->AttackSpeed > 0)
    {
        return 1.0f / StatsComponent->AttackSpeed;
    }
    return 1.0f;
}

// ====================================================================
// 타겟 및 발사 로직
// ====================================================================

APawn* UAutoAttackComponent::FindTarget() const
{
    UWorld* World = GetWorld();
    APlayerCharacter* OwnerChar = Cast<APlayerCharacter>(GetOwner());
    if (!World || !OwnerChar || !MonsterClassFilter) return nullptr;

    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(World, MonsterClassFilter, OverlappingActors);

    if (OverlappingActors.Num() > 0)
    {
        APawn* BestTarget = nullptr;
        float ClosestDistSq = FMath::Square(AttackRange);

        FVector OwnerLocation = OwnerChar->GetActorLocation();

        for (AActor* Actor : OverlappingActors)
        {
            if (APawn* Pawn = Cast<APawn>(Actor))
            {
                const UCharacterStatsComponent* EnemyStats = Pawn->FindComponentByClass<UCharacterStatsComponent>();
                if (EnemyStats)
                {
                    if (EnemyStats->IsDead() || EnemyStats->CurrentHP <= 0.0f)
                    {
                        continue;
                    }
                }
                if (!Pawn->IsValidLowLevel() || Pawn->IsPendingKillPending())
                {
                    continue;
                }

                float DistSq = FVector::DistSquared(OwnerLocation, Pawn->GetActorLocation());

                if (DistSq <= ClosestDistSq)
                {
                    ClosestDistSq = DistSq;
                    BestTarget = Pawn;
                }
            }
        }
        return BestTarget;
    }

    return nullptr;
}

FRotator UAutoAttackComponent::GetFireRotation(const APawn* Target) const
{
    if (Target)
    {
        FVector Direction = Target->GetActorLocation() - GetOwner()->GetActorLocation();
        return Direction.Rotation();
    }
    else
    {
        return GetOwner()->GetActorRotation();
    }
}

void UAutoAttackComponent::FireProjectile()
{
    if (!ProjectileClass || !StatsComponent) return;

    UWorld* World = GetWorld();
    if (!World) return;

    APawn* Target = FindTarget();

    // 🔹 타겟이 없으면 그냥 패스 (다음 Tick에 다시 시도)
    if (!Target)
        return;

    // 🔹 유효성 확인
    if (!Target->IsValidLowLevel() || Target->IsPendingKillPending())
        return;


    // 🔹 몬스터 상태 컴포넌트 확인
    if (AAI_Monsters* Enemy = Cast<AAI_Monsters>(Target))
    {
        if (Enemy->IsDead())
        {
            UE_LOG(LogTemp, Warning, TEXT("[AutoAttack] Target is dead. Searching for next target..."));
            return; // 다음 Tick에서 새 타겟 자동 탐색
        }
    }

    // 🔹 투사체 발사
    FRotator BaseRotation = GetFireRotation(Target);
    int32 Count = StatsComponent->ProjectileCount;
    FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(0, 0, 50.0f);

    float SpreadAngle = 10.0f;
    float HalfAngle = SpreadAngle * (Count - 1) / 2.0f;

    for (int32 i = 0; i < Count; ++i)
    {
        FRotator FinalRotation = BaseRotation;
        if (Count > 1)
        {
            float CurrentAngle = -HalfAngle + (i * SpreadAngle);
            FinalRotation.Yaw += CurrentAngle;
        }

        APlayerCharacter* OwnerChar = Cast<APlayerCharacter>(GetOwner());
        FActorSpawnParameters Params;
        Params.Owner = OwnerChar;
        Params.Instigator = OwnerChar;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* NewActor = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, FinalRotation, Params);
        if (AProjectile* Projectile = Cast<AProjectile>(NewActor))
        {
            Projectile->ProjectileMovement->Velocity = FinalRotation.Vector() * Projectile->ProjectileMovement->InitialSpeed;
            Projectile->InitializeProjectile(StatsComponent->AttackDamage);
        }
    }
}
