#include "Skill/ActiveSkillItem.h"
#include "PlayerMade/PlayerCharacter.h"
#include "Skill/SkillUseIndicatorComponent.h"
#include "skill/Actor/PowerSurgeBullet.h"

AActiveSkillItem::AActiveSkillItem()
{
	ActiveSkillData.StackCnt = 0;
	ActiveType = EActiveSkillItemType::None;
}

void AActiveSkillItem::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		ActiveSkillApply(Player);
	}
}

void AActiveSkillItem::ActiveSkillApply(class APlayerCharacter* Target)
{

	if (!Target) return;

	FActorSpawnParameters Params;
	Params.Owner = Target;
	Params.Instigator = Target;

	switch (ActiveType)
	{
	case EActiveSkillItemType::ShootingGun:
	{
		FVector SpawnLocation = Target->GetActorLocation() + Target->GetActorForwardVector() * 120.f + FVector(0, 0, 50.f);
		FRotator SpawnRotation = Target->GetActorRotation();

		// 마우스 위치 인디케이터를 이용하면 방향 보정 가능
		USkillUseIndicatorComponent* Indicator = Target->FindComponentByClass<USkillUseIndicatorComponent>();
		if (Indicator && Indicator->GetSpawnedIndicatorActor())
		{
			FVector Dir = (Indicator->GetSpawnedIndicatorActor()->GetActorLocation() - SpawnLocation).GetSafeNormal();
			SpawnRotation = Dir.Rotation();
		}

		// 샷건 이펙트 액터 Spawn
		if (PowerSurgeBulletEffectActorClass)
		{
			APowerSurgeBullet* Effect = GetWorld()->SpawnActor<APowerSurgeBullet>(PowerSurgeBulletEffectActorClass, SpawnLocation, SpawnRotation, Params);
			if (Effect)
			{
				Effect->SetInstigator(Target);
			}
		}
		break;
	}
	case EActiveSkillItemType::CircleNearbySword:

		break;
	case EActiveSkillItemType::ShootingLaser:

		break;
	case EActiveSkillItemType::NapalmAttack:

		break;
	case EActiveSkillItemType::NuclearAttack:

		break;
	default:
		break;
	}
}
