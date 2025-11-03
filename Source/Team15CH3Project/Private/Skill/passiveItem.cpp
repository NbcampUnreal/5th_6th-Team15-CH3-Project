#include "Skill/passiveItem.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "PlayerMade/PlayerCharacter.h"
#include "Skill/Actor/Drone.h"
#include "Kismet/GameplayStatics.h"

ApassiveItem::ApassiveItem()
{
	PassiveSkillData.StackCnt = 0;
	PassiveSkillData.MaxStackCnt = 5;
	PassiveType = EPassiveItemType::None;
}

void ApassiveItem::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		PassiveSkillApply(Player);
	}
}

void ApassiveItem::PassiveSkillApply(APlayerCharacter* Target)
{
	if (!Target) return;

	UCharacterStatsComponent* Stats = Target->FindComponentByClass<UCharacterStatsComponent>();
	if (!Stats) return;

	if (PassiveSkillData.StackCnt < PassiveSkillData.MaxStackCnt)
	{
		PassiveSkillData.StackCnt++;
	}

	const float Multiplier = 1.0f + (PassiveSkillData.PercentStack / 100.0f) * PassiveSkillData.StackCnt;

	switch (PassiveType) // 스탯 계산 수식 다시 정립 필요
	{
	case EPassiveItemType::AttackPowerBoost:
		Stats->AttackDamage *= Multiplier;
		break;
	case EPassiveItemType::AttackSpeedBoost:
		Stats->AttackSpeed *= Multiplier;

		break;
	case EPassiveItemType::AttackIncreasingDirection:
		Stats->ProjectileCount += 1;
		break;
	case EPassiveItemType::BulletBoost:

		break;
	case EPassiveItemType::HPBoost: //최대 HP 증가 효과
		Stats->MaxHP *= Multiplier;

		break;
	case EPassiveItemType::MPBoost: //최대 MP 증가 효과
		Stats->MaxMP *= Multiplier;

		break;
	case EPassiveItemType::ExpBoost: //경험치 획득량 증가 적AI 에서 받아올게 필요


		break;
	case EPassiveItemType::SprintBoost:
		Stats->MoveSpeed *= Multiplier;

		break;
	case EPassiveItemType::BloodAbsorbing: //현재 체력을 흡혈 효과로 회복하는 패시브
		// 여기서는 스킬 획득 판정만 하고 직접적인 스킬 효과는 몬스터가 죽을때 제공

		break;
	case EPassiveItemType::AutomaticAttackDrone:
	{
		if (!DroneClass) break;

		UWorld* World = GetWorld();
		if (!World) break;

		int32 MaxDrone = 5;

		TArray<AActor*> ExistingDrones;
		UGameplayStatics::GetAllActorsOfClass(World, DroneClass, ExistingDrones);
		int32 DronesToSpawn = FMath::Clamp(PassiveSkillData.StackCnt - ExistingDrones.Num(), 0, MaxDrone);

		for (int32 i = 0; i < DronesToSpawn; ++i)
		{
			FVector SpawnLoc = GetActorLocation();
			FRotator SpawnRot = FRotator::ZeroRotator;

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this; // 스킬 소유자
			SpawnParams.Instigator = Cast<APawn>(GetOwner());

			ADrone* NewDrone = World->SpawnActor<ADrone>(DroneClass, SpawnLoc, SpawnRot, SpawnParams);
			if (NewDrone)
			{
				NewDrone->SetOrbitTarget(this);
			}
		}
		break;
	}
	default:
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("[PassiveSkill] %s 획득 스택: %d"),
		*PassiveSkillData.SkillName.ToString(), PassiveSkillData.StackCnt);

	UE_LOG(LogTemp, Warning, TEXT("향상된 능력치: AttackDamage=%.1f, AttackSpeed=%.1f, MaxHP=%.1f, MaxMP=%.1f, MoveSpeed=%.1f"),
		Stats->AttackDamage, Stats->AttackSpeed, Stats->MaxHP, Stats->MaxMP, Stats->MoveSpeed);
}