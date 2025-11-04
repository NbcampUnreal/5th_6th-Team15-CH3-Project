// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMade/CharacterStatsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
UCharacterStatsComponent::UCharacterStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// ====================================================================
// 데미지 및 사망 로직
// ====================================================================

// 데미지 처리 함수
float UCharacterStatsComponent::TakeDamage(float DamageAmount)
{
    // 이미 사망했으면 데미지 처리 및 Die() 호출을 중복방지
    if (IsDead())
    {
        return 0.0f;
    }

    CurrentHP -= DamageAmount;

    UE_LOG(LogTemp, Warning, TEXT("[Stats] Damage Taken: %.2f, Current HP: %.2f"), DamageAmount, CurrentHP);

    // 사망 체크
    if (CurrentHP <= 0.0f)
    {
        CurrentHP = 0.0f;
        Die();
    }

    return DamageAmount;
}

// 사망 처리 함수 
void UCharacterStatsComponent::Die()
{
    UE_LOG(LogTemp, Error, TEXT("[Stats] Character has DIED! Notifying UI/AI."));
    // APlayerCharacter::TakeDamage에서 PlayerIsDead()가 호출
}

// ====================================================================
// 경험치 및 레벨업 로직
// ====================================================================

void UCharacterStatsComponent::GainExperience(int32 Amount)
{
    // 사망 상태에서는 경험치 획득 방지
    if (IsDead()) return;

    Experience += Amount;

    UE_LOG(LogTemp, Warning, TEXT("[Stats] Gained %d EXP. Current: %d/%d (Level %d)"),
        Amount, Experience, MaxExperience, Level);

    // 경험치가 최대치를 넘었는지 체크
    if (Experience >= MaxExperience)
    {
        LevelUp();
    }
}

void UCharacterStatsComponent::LevelUp()
{
    // 초과 경험치 계산
    int32 OverExp = Experience - MaxExperience;

    Level++;
    Experience = OverExp;

    // MaxExperience 자동 증가 (20%씩 증가)
    MaxExperience = FMath::CeilToInt(MaxExperience * 1.2f);

    UE_LOG(LogTemp, Warning, TEXT("=== LEVEL UP! New Level: %d, Next MaxEXP: %d ==="),
        Level, MaxExperience);

    ApplyLevelUpStats();

    // 남은 경험치로 연속 레벨업 가능 여부 확인
    if (Experience >= MaxExperience)
    {
        LevelUp();
    }
}

// 레벨 업 스탯 적용 함수 구현
void UCharacterStatsComponent::ApplyLevelUpStats()
{
    //// 1. MaxHP 증가 및 HP 회복
    //MaxHP += 10.0f;
    //CurrentHP = MaxHP; // 레벨업 시 체력 완전 회복

    //// 2. 공격력
    //AttackDamage += 2.0f;

}