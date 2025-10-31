// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"


class ACharacter; // UCharacterMovementComponent 사용을 위해 전방선언 필요

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAM15CH3PROJECT_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterStatsComponent();

	float TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintPure, Category = "Stats | Resources")
	bool IsDead() const { return CurrentHP <= 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Stats | Progression")
	void GainExperience(int32 Amount);

protected:
	// 사망 시 호출되는 내부 함수
	void Die();

	void LevelUp();

	void ApplyLevelUpStats();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats | Progression")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Progression")
	int32 MaxExperience = 100; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats | Progression")
	int32 Experience = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	float AttackDamage = 10.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	//float AttackDamageMin = 9;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	//float AttackDamageMax = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	float AttackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Combat")
	int32 ProjectileCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Resources")
	float MaxMP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats | Resources")
	float CurrentMP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Resources")
	float MaxHP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats | Resources")
	float CurrentHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Misc")
	float MoveSpeed = 600.0f;

};