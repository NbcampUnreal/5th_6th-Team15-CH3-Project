#include "AI_Monster/BossMonster.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/DamageType.h"

ABossMonster::ABossMonster()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = (CurrentHP > 0.f) ? FMath::Clamp(CurrentHP, 0.f, MaxHP) : MaxHP;
}

float ABossMonster::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentHP <= 0.f) return 0.f;

	const float Applied = FMath::Max(0.f, DamageAmount);
	CurrentHP = FMath::Clamp(CurrentHP - Applied, 0.f, MaxHP);

	UE_LOG(LogTemp, Warning, TEXT("[Boss] -%.1f HP  ->  %.1f / %.1f"),
		Applied, CurrentHP, MaxHP);

	if (CurrentHP <= 0.f)
	{
		HandleDeath();
	}
	return Applied;
}

void ABossMonster::BasicAttack(AActor* Target)
{
	if (!Target || CurrentHP <= 0.f) return;
	if (!IsTargetInRange(Target, AttackRange)) return;

	UGameplayStatics::ApplyDamage(
		Target,
		AttackDamage,
		GetController(),
		this,
		UDamageType::StaticClass()
	);

	UE_LOG(LogTemp, Warning, TEXT("[Boss] BasicAttack  dmg=%.0f"), AttackDamage);
}

void ABossMonster::UseSkill()
{
	if (CurrentHP <= 0.f) return;

	TArray<AActor*> Ignore; Ignore.Add(this);
	const bool bFullDamage = true;
	const ECollisionChannel Channel = ECC_Visibility;

	const bool bHitAny = UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		SkillDamage,
		GetActorLocation(),
		SkillRadius,
		UDamageType::StaticClass(),
		Ignore,
		this,
		GetController(),
		bFullDamage,
		Channel
	);

	if (bHitAny)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Boss] UseSkill (AOE)  dmg=%.0f  radius=%.0f"),
			SkillDamage, SkillRadius);
	}
}

void ABossMonster::HandleDeath()
{
	// 충돌 끄고 컨트롤러 분리
	if (UCapsuleComponent* Cap = GetCapsuleComponent())
	{
		Cap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	DetachFromControllerPendingDestroy();

	// 골드 드랍(고정 개수)
	DropGold();

	UE_LOG(LogTemp, Warning, TEXT("[Boss] Dead"));
	SetLifeSpan(3.0f);
}

void ABossMonster::DropGold()
{
	if (GoldAmount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Boss] GoldAmount <= 0, skip"));
		return;
	}

	if (!GoldPickupClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Boss] Drop %d gold (no class set, log only)"), GoldAmount);
		return;
	}

	for (int32 i = 0; i < GoldAmount; ++i)
	{
		const FVector Offset(
			FMath::FRandRange(-80.f, 80.f),
			FMath::FRandRange(-80.f, 80.f),
			FMath::FRandRange(20.f, 60.f)
		);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(
			GoldPickupClass,
			GetActorLocation() + Offset,
			FRotator::ZeroRotator,
			Params
		);
	}

	UE_LOG(LogTemp, Warning, TEXT("[Boss] Dropped %d gold actors"), GoldAmount);
}

bool ABossMonster::IsTargetInRange(AActor* Target, float Range) const
{
	if (!Target) return false;
	return FVector::Dist2D(GetActorLocation(), Target->GetActorLocation()) <= Range;
}
