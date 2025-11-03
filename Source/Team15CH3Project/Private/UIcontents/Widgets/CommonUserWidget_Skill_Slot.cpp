#include "UIcontents/Widgets/CommonUserWidget_Skill_Slot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameManager/Pro_M_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UIcontents/Widgets/CommonUserWidget_BattleGameHUD.h"
#include "UIcontents/Widgets/CommonUserWidget_Skill.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Skill/SkillInventoryComponent.h"
#include "PlayerMade/CharacterStatsComponent.h"
#include "PlayerMade/PlayerCharacter.h"
#include "Skill/passiveItem.h"
#include "GameFramework/CharacterMovementComponent.h"

int32 UCommonUserWidget_Skill_Slot::LastAssignedSlotIndex = 0;
float UCommonUserWidget_Skill_Slot::StaticBaseAttackDamage = 0.0f;

void UCommonUserWidget_Skill_Slot::NativeConstruct() // 위젯이 처음 나타날 때 한번만 실행
{
	Super::NativeConstruct();

	if (SkillButton)
	{
		SkillButton->OnClicked.AddDynamic(this, &UCommonUserWidget_Skill_Slot::OnSkillButtonClicked);
	}


	// DataAsset slot
	if (this->GetName().Contains(TEXT("WBP_Skill_Slot_1")))
	{
		if (UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld()))
		{
			if (UPro_M_GameInstance* ProGI = Cast<UPro_M_GameInstance>(GI))
			{
				if (ProGI->SkillDataAsset && ProGI->SkillDataAsset->PassiveSkills.Num() > 0)
				{

					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
					USkillInventoryComponent* SkillInventory = PlayerCharacter ? PlayerCharacter->FindComponentByClass<USkillInventoryComponent>() : nullptr;

					//선택 가능한 스킬 목록 필터링
					TArray<FPassiveItemData> AvailableSkills;
					for (const FPassiveItemData& SkillData : ProGI->SkillDataAsset->PassiveSkills)
					{
						bool bIsMaxStack = false;
						if (SkillInventory)
						{
							//현재 인벤토리에서 스킬 찾기
							for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
							{
								if (InvSkill.Type == SkillData.Type)
								{
									//현재 스택이 최대 스택과 같으면 또는 초과하면
									if (InvSkill.StackCnt >= InvSkill.MaxStackCnt)
									{
										bIsMaxStack = true;
									}
									break;
								}
							}
						}

						//최대 스택이 아닌 스킬만 목록에 추가합니다.
						if (!bIsMaxStack)
						{
							AvailableSkills.Add(SkillData);
						}
					}

					//선택 가능한 스킬이 없으면 위젯을 비활성화/숨기거나 리턴합니다.
					if (AvailableSkills.Num() == 0)
					{
						// 모든 스킬이 MAX 스택이면 위젯 슬롯을 숨기거나 비활성화합니다.
						UE_LOG(LogTemp, Warning, TEXT("No available skills to show in slot 1 (All skills maxed or already shown)."));
						return; // 스킬을 설정하지 않고 종료
					}


					//필터링된 목록에서 랜덤 선택
					int32 MaxIndex = AvailableSkills.Num() - 1;
					int32 RandomIndex = FMath::RandRange(0, MaxIndex);
					const FPassiveItemData& Skill = AvailableSkills[RandomIndex];

					//스택 계산 및 위젯 설정 (기존 로직 유지)
					int32 DisplayStack = Skill.StackCnt;

					if (SkillInventory)
					{
						for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
						{
							if (InvSkill.Type == Skill.Type)
							{
								DisplayStack = InvSkill.StackCnt;

								if (DisplayStack < InvSkill.MaxStackCnt)
								{
									DisplayStack++; // 다음 레벨+1
								}
								break;
							}
						}
					}

					SavedPassiveSkillData = Skill;

					if (SkillNameText) { SkillNameText->SetText(Skill.SkillName); }
					if (Skill.Icon) { SkillIcon->SetBrushFromTexture(Skill.Icon); SelectedSkillImage = SkillIcon; }
					if (SkillDescriptionText) { SkillDescriptionText->SetText(Skill.Description); }
					if (StackText) { FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack); StackText->SetText(FText::FromString(StackLevel)); }

				}
			}
		}
	}
	if (this->GetName().Contains(TEXT("WBP_Skill_Slot_2")))
	{
		if (UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld()))
		{
			if (UPro_M_GameInstance* ProGI = Cast<UPro_M_GameInstance>(GI))
			{
				if (ProGI->SkillDataAsset && ProGI->SkillDataAsset->PassiveSkills.Num() > 0)
				{

					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
					USkillInventoryComponent* SkillInventory = PlayerCharacter ? PlayerCharacter->FindComponentByClass<USkillInventoryComponent>() : nullptr;

					//선택 가능한 스킬 목록 필터링
					TArray<FPassiveItemData> AvailableSkills;
					for (const FPassiveItemData& SkillData : ProGI->SkillDataAsset->PassiveSkills)
					{
						bool bIsMaxStack = false;
						if (SkillInventory)
						{
							//현재 인벤토리에서 스킬 찾기
							for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
							{
								if (InvSkill.Type == SkillData.Type)
								{
									//현재 스택이 최대 스택과 같으면 또는 초과하면
									if (InvSkill.StackCnt >= InvSkill.MaxStackCnt)
									{
										bIsMaxStack = true;
									}
									break;
								}
							}
						}

						//최대 스택이 아닌 스킬만 목록에 추가합니다.
						if (!bIsMaxStack)
						{
							AvailableSkills.Add(SkillData);
						}
					}

					//선택 가능한 스킬이 없으면 위젯을 비활성화/숨기거나 리턴합니다.
					if (AvailableSkills.Num() == 0)
					{
						// 모든 스킬이 MAX 스택이면 위젯 슬롯을 숨기거나 비활성화합니다.
						UE_LOG(LogTemp, Warning, TEXT("No available skills to show in slot 1 (All skills maxed or already shown)."));
						return; // 스킬을 설정하지 않고 종료
					}


					//필터링된 목록에서 랜덤 선택
					int32 MaxIndex = AvailableSkills.Num() - 1;
					int32 RandomIndex = FMath::RandRange(0, MaxIndex);
					const FPassiveItemData& Skill = AvailableSkills[RandomIndex];

					//스택 계산 및 위젯 설정 (기존 로직 유지)
					int32 DisplayStack = Skill.StackCnt;

					if (SkillInventory)
					{
						for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
						{
							if (InvSkill.Type == Skill.Type)
							{
								DisplayStack = InvSkill.StackCnt;

								if (DisplayStack < InvSkill.MaxStackCnt)
								{
									DisplayStack++; // 다음 레벨+1
								}
								break;
							}
						}
					}

					SavedPassiveSkillData = Skill;

					if (SkillNameText) { SkillNameText->SetText(Skill.SkillName); }
					if (Skill.Icon) { SkillIcon->SetBrushFromTexture(Skill.Icon); SelectedSkillImage = SkillIcon; }
					if (SkillDescriptionText) { SkillDescriptionText->SetText(Skill.Description); }
					if (StackText) { FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack); StackText->SetText(FText::FromString(StackLevel)); }

				}
			}
		}
	}
	if (this->GetName().Contains(TEXT("WBP_Skill_Slot_3")))
	{
		if (UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld()))
		{
			if (UPro_M_GameInstance* ProGI = Cast<UPro_M_GameInstance>(GI))
			{
				if (ProGI->SkillDataAsset && ProGI->SkillDataAsset->PassiveSkills.Num() > 0)
				{

					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
					USkillInventoryComponent* SkillInventory = PlayerCharacter ? PlayerCharacter->FindComponentByClass<USkillInventoryComponent>() : nullptr;

					//선택 가능한 스킬 목록 필터링
					TArray<FPassiveItemData> AvailableSkills;
					for (const FPassiveItemData& SkillData : ProGI->SkillDataAsset->PassiveSkills)
					{
						bool bIsMaxStack = false;
						if (SkillInventory)
						{
							//현재 인벤토리에서 스킬 찾기
							for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
							{
								if (InvSkill.Type == SkillData.Type)
								{
									//현재 스택이 최대 스택과 같으면 또는 초과하면
									if (InvSkill.StackCnt >= InvSkill.MaxStackCnt)
									{
										bIsMaxStack = true;
									}
									break;
								}
							}
						}

						//최대 스택이 아닌 스킬만 목록에 추가합니다.
						if (!bIsMaxStack)
						{
							AvailableSkills.Add(SkillData);
						}
					}

					//선택 가능한 스킬이 없으면 위젯을 비활성화/숨기거나 리턴합니다.
					if (AvailableSkills.Num() == 0)
					{
						// 모든 스킬이 MAX 스택이면 위젯 슬롯을 숨기거나 비활성화합니다.
						UE_LOG(LogTemp, Warning, TEXT("No available skills to show in slot 1 (All skills maxed or already shown)."));
						return; // 스킬을 설정하지 않고 종료
					}


					//필터링된 목록에서 랜덤 선택
					int32 MaxIndex = AvailableSkills.Num() - 1;
					int32 RandomIndex = FMath::RandRange(0, MaxIndex);
					const FPassiveItemData& Skill = AvailableSkills[RandomIndex];

					//스택 계산 및 위젯 설정 (기존 로직 유지)
					int32 DisplayStack = Skill.StackCnt;

					if (SkillInventory)
					{
						for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
						{
							if (InvSkill.Type == Skill.Type)
							{
								DisplayStack = InvSkill.StackCnt;

								if (DisplayStack < InvSkill.MaxStackCnt)
								{
									DisplayStack++; // 다음 레벨+1
								}
								break;
							}
						}
					}

					SavedPassiveSkillData = Skill;

					if (SkillNameText) { SkillNameText->SetText(Skill.SkillName); }
					if (Skill.Icon) { SkillIcon->SetBrushFromTexture(Skill.Icon); SelectedSkillImage = SkillIcon; }
					if (SkillDescriptionText) { SkillDescriptionText->SetText(Skill.Description); }
					if (StackText) { FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack); StackText->SetText(FText::FromString(StackLevel)); }

				}
			}
		}
	}
}

void UCommonUserWidget_Skill_Slot::OnSkillButtonClicked()
{
	if (!SelectedSkillImage) return;

	// 플레이어 및 컴포넌트 참조 가져오기
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!PlayerCharacter) {
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter not found."));
		return;
	}

	USkillInventoryComponent* SkillInventory = PlayerCharacter->FindComponentByClass<USkillInventoryComponent>();
	UCharacterStatsComponent* StatComponent = PlayerCharacter->FindComponentByClass<UCharacterStatsComponent>();

	if (!SkillInventory || !StatComponent) {
		UE_LOG(LogTemp, Error, TEXT("SkillInventory or StatComponent not found."));
		return;
	}

	//StatComponent의 초기 공격력을 저장
	if (FMath::IsNearlyZero(StaticBaseAttackDamage))
	{
		StaticBaseAttackDamage = StatComponent->AttackDamage;
		UE_LOG(LogTemp, Log, TEXT("STATIC Base Attack CACHED: %.1f"), StaticBaseAttackDamage);
	}

	//기존 스킬 존재 여부 확인 및 스택 계산
	int32 CurrentStack = 0;
	FPassiveItemData* ExistingSkill = nullptr;

	for (FPassiveItemData& Skill : SkillInventory->PassiveSkillsInv)
	{
		if (Skill.Type == SavedPassiveSkillData.Type)
		{
			ExistingSkill = &Skill;
			break;
		}
	}

	//스택 증가 또는 새 스킬 추가 로직 실행
	if (ExistingSkill)
	{
		if (ExistingSkill->StackCnt < ExistingSkill->MaxStackCnt)
		{
			ExistingSkill->StackCnt++;
			CurrentStack = ExistingSkill->StackCnt;
		}
		else
		{
			CurrentStack = ExistingSkill->StackCnt;
		}
	}
	else
	{
		//새 스킬 추가
		FPassiveItemData NewSkill = SavedPassiveSkillData;
		NewSkill.StackCnt = 1;

		SkillInventory->PassiveSkillsInv.Add(NewSkill);
		CurrentStack = NewSkill.StackCnt;

		UE_LOG(LogTemp, Log, TEXT("Passive Skill inv Added to [%s]! Name: %s, Initial Stack: %d. Inventory Size: %d"),
			*SkillInventory->GetName(), //컴포넌트 이름 (인벤토리 이름) 추가
			*NewSkill.SkillName.ToString(),
			CurrentStack,
			SkillInventory->PassiveSkillsInv.Num());
	}


	//하드코딩 패시브 적용
	if (!StatComponent) return;

	//스택 및 제한 계산
	int32 MaxStackToApply = ExistingSkill ? ExistingSkill->MaxStackCnt : SavedPassiveSkillData.MaxStackCnt;
	int32 AppliedStack = FMath::Min(CurrentStack, MaxStackToApply);

	//스택당 효과 비율 (10%)
	float RatePerStack = 0.1f;

	//블루프린트에서 수정된 현재 능력치 값(=실제 적용 중인 값)을 기준으로 계산
	float BP_BaseAttackDamage = StatComponent->AttackDamage;
	float BP_BaseAttackSpeed = StatComponent->AttackSpeed;
	float BP_BaseMaxHP = StatComponent->MaxHP;
	float BP_BaseMaxMP = StatComponent->MaxMP;
	float BP_BaseMoveSpeed = StatComponent->MoveSpeed;


	//공격력 증가 패시브
	if (SavedPassiveSkillData.Type == EPassiveItemType::AttackPowerBoost)
	{
		float BaseToUse_Attack = BP_BaseAttackDamage;

		StatComponent->AttackDamage = BaseToUse_Attack * (1.0f + RatePerStack * AppliedStack);

		UE_LOG(LogTemp, Warning, TEXT("[Passive] AttackDamage updated: %.1f (Base: %.1f, Stack: %d)"),
			StatComponent->AttackDamage, BaseToUse_Attack, AppliedStack);
	}

	//공격 속도 증가 패시브
	if (SavedPassiveSkillData.Type == EPassiveItemType::AttackSpeedBoost)
	{
		float BaseToUse_Speed = BP_BaseAttackSpeed;

		StatComponent->AttackSpeed = BaseToUse_Speed * (1.0f + RatePerStack * AppliedStack);

		UE_LOG(LogTemp, Warning, TEXT("[Passive] AttackSpeed updated: %.2f (Base: %.2f, Stack: %d)"),
			StatComponent->AttackSpeed, BaseToUse_Speed, AppliedStack);
	}

	//최대 HP 증가 패시브
	if (SavedPassiveSkillData.Type == EPassiveItemType::HPBoost)
	{
		float BaseToUse_HP = BP_BaseMaxHP;
		float OldMaxHP = StatComponent->MaxHP;

		//현재 HP 비율 계산 (MaxHP가 0일 경우 예외 처리)
		float HealthRatio = (OldMaxHP > 0.0f) ? (StatComponent->CurrentHP / OldMaxHP) : 1.0f;

		//MaxHP 증가
		StatComponent->MaxHP = BaseToUse_HP * (1.0f + RatePerStack * AppliedStack);

		//CurrentHP를 새로운 MaxHP에 비율을 적용하여 설정
		StatComponent->CurrentHP = StatComponent->MaxHP * HealthRatio;

		UE_LOG(LogTemp, Warning, TEXT("[Passive] MaxHP updated: %.1f (Base: %.1f, Stack: %d)"),
			StatComponent->MaxHP, BaseToUse_HP, AppliedStack);
	}

	//최대 MP 증가 패시브
	if (SavedPassiveSkillData.Type == EPassiveItemType::MPBoost)
	{
		float BaseToUse_MP = BP_BaseMaxMP;
		float OldMaxMP = StatComponent->MaxMP;

		//현재 MP 비율 계산 (MaxMP가 0일 경우 예외 처리)
		float ManaRatio = (OldMaxMP > 0.0f) ? (StatComponent->CurrentMP / OldMaxMP) : 1.0f;

		//MaxMP 증가
		StatComponent->MaxMP = BaseToUse_MP * (1.0f + RatePerStack * AppliedStack);

		//CurrentMP를 새로운 MaxMP에 비율을 적용하여 설정
		StatComponent->CurrentMP = StatComponent->MaxMP * ManaRatio;

		UE_LOG(LogTemp, Warning, TEXT("[Passive] MaxMP updated: %.1f (Base: %.1f, Stack: %d)"),
			StatComponent->MaxMP, BaseToUse_MP, AppliedStack);
	}

	//이동 속도 증가 패시브
	if (SavedPassiveSkillData.Type == EPassiveItemType::SprintBoost)
	{
		float BaseToUse_Move = BP_BaseMoveSpeed;

		//StatComponent의 MoveSpeed 값 갱신
		StatComponent->MoveSpeed = BaseToUse_Move * (1.0f + RatePerStack * AppliedStack);

		//UCharacterMovementComponent 찾기 및 실제 MaxWalkSpeed에 적용
		if (APlayerCharacter* PC = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			if (UCharacterMovementComponent* MovementComp = PC->GetCharacterMovement())
			{
				//StatComponent에 저장된 새 MoveSpeed 값을 실제 MaxWalkSpeed에 직접 적용
				MovementComp->MaxWalkSpeed = StatComponent->MoveSpeed;
				UE_LOG(LogTemp, Warning, TEXT("[Passive] MaxWalkSpeed applied: %.1f"), MovementComp->MaxWalkSpeed);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("[Passive] MoveSpeed updated: %.1f (Base: %.1f, Stack: %d)"),
			StatComponent->MoveSpeed, BaseToUse_Move, AppliedStack);
	}

	//발사체 수 증가
	if (SavedPassiveSkillData.Type == EPassiveItemType::AttackIncreasingDirection)
	{
		int32 BaseToUse_Projectile = 1;
		StatComponent->ProjectileCount = BaseToUse_Projectile + AppliedStack;

		UE_LOG(LogTemp, Warning, TEXT("[Passive] ProjectileCount updated: %d (Base: %d, Stack: %d)"),
			StatComponent->ProjectileCount, BaseToUse_Projectile, AppliedStack);
	}

	//흡혈 효과
	if (SavedPassiveSkillData.Type == EPassiveItemType::BloodAbsorbing)
	{
		float HealAmount = StatComponent->MaxHP * RatePerStack * AppliedStack;
		StatComponent->CurrentHP = FMath::Clamp(StatComponent->CurrentHP + HealAmount, 0.0f, StatComponent->MaxHP);

		UE_LOG(LogTemp, Warning, TEXT("[Passive] BloodAbsorbing healed: +%.1f → CurrentHP=%.1f / MaxHP=%.1f (Stack: %d)"),
			HealAmount, StatComponent->CurrentHP, StatComponent->MaxHP, AppliedStack);
	}


	//HUD 갱신
	if (UCommonUserWidget_BattleGameHUD* BattleHUD = GetBattleHUD())
	{
		if (!SelectedSkillImage) return;

		//1번 슬롯
		if (BattleHUD->SkillSlot_1_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_1_Type == EPassiveItemType::None)
		{
			BattleHUD->Image_Skill_1->SetBrush(SelectedSkillImage->GetBrush());
			BattleHUD->Image_Skill_1->SetColorAndOpacity(FLinearColor::White);

			int32 DisplayStack = FMath::Min(CurrentStack, 5);
			BattleHUD->StackCnt_1->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayStack)));
			BattleHUD->StackCnt_1->SetVisibility(ESlateVisibility::Visible);

			BattleHUD->SkillSlot_1_Type = SavedPassiveSkillData.Type;
		}

		//2번 슬롯
		else if (BattleHUD->SkillSlot_2_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_2_Type == EPassiveItemType::None)
		{
			BattleHUD->Image_Skill_2->SetBrush(SelectedSkillImage->GetBrush());
			BattleHUD->Image_Skill_2->SetColorAndOpacity(FLinearColor::White);

			int32 DisplayStack = FMath::Min(CurrentStack, 5);
			BattleHUD->StackCnt_2->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayStack)));
			BattleHUD->StackCnt_2->SetVisibility(ESlateVisibility::Visible);

			BattleHUD->SkillSlot_2_Type = SavedPassiveSkillData.Type;
		}

		//3번 슬롯
		else if (BattleHUD->SkillSlot_3_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_3_Type == EPassiveItemType::None)
		{
			BattleHUD->Image_Skill_3->SetBrush(SelectedSkillImage->GetBrush());
			BattleHUD->Image_Skill_3->SetColorAndOpacity(FLinearColor::White);

			int32 DisplayStack = FMath::Min(CurrentStack, 5);
			BattleHUD->StackCnt_3->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayStack)));
			BattleHUD->StackCnt_3->SetVisibility(ESlateVisibility::Visible);

			BattleHUD->SkillSlot_3_Type = SavedPassiveSkillData.Type;
		}

		//4번 슬롯
		else if (BattleHUD->SkillSlot_4_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_4_Type == EPassiveItemType::None)
		{
			BattleHUD->Image_Skill_4->SetBrush(SelectedSkillImage->GetBrush());
			BattleHUD->Image_Skill_4->SetColorAndOpacity(FLinearColor::White);

			int32 DisplayStack = FMath::Min(CurrentStack, 5);
			BattleHUD->StackCnt_4->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayStack)));
			BattleHUD->StackCnt_4->SetVisibility(ESlateVisibility::Visible);

			BattleHUD->SkillSlot_4_Type = SavedPassiveSkillData.Type;
		}
		//뷰포트 갱신
		if (UWorld* World = GetWorld())
		{
			TArray<UUserWidget*> FoundSkillWidgets;

			//월드에 존재하는 UCommonUserWidget_Skill 클래스의 모든 인스턴스를 검색합니다.
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundSkillWidgets, UCommonUserWidget_Skill::StaticClass(), false);

			for (UUserWidget* SkillWidget : FoundSkillWidgets)
			{
				//찾은 스킬 위젯을 제거합니다. (스킬 창 전체가 닫힘)
				if (SkillWidget)
				{
					SkillWidget->RemoveFromParent();
				}
			}
		}
	}
}

UCommonUserWidget_BattleGameHUD* UCommonUserWidget_Skill_Slot::GetBattleHUD()
{
	if (UWorld* World = GetWorld())
	{
		TArray<UUserWidget*> FoundWidgets;

		//현재 월드에 존재하는 BattleGameHUD 클래스의 위젯을 검색
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundWidgets, UCommonUserWidget_BattleGameHUD::StaticClass(), false);

		if (FoundWidgets.Num() > 0)
		{
			return Cast<UCommonUserWidget_BattleGameHUD>(FoundWidgets[0]);
		}
	}
	return nullptr;
}


