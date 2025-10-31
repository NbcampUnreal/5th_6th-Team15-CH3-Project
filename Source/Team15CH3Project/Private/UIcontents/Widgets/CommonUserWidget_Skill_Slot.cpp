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

					int32 MaxIndex = ProGI->SkillDataAsset->PassiveSkills.Num() - 1;
					int32 RandomIndex = FMath::RandRange(0, MaxIndex);

					const FPassiveItemData& Skill = ProGI->SkillDataAsset->PassiveSkills[RandomIndex];

					int32 DisplayStack = Skill.StackCnt;

					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

					USkillInventoryComponent* SkillInventory = PlayerCharacter ? PlayerCharacter->FindComponentByClass<USkillInventoryComponent>() : nullptr;

					if (SkillInventory)
					{
						// 현재 인벤토리에서 해당 스킬 타입이 있는지 검색
						for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
						{
							if (InvSkill.Type == Skill.Type)
							{
								DisplayStack = InvSkill.StackCnt; // 현재 인벤토리의 스택 레벨

								// 다음 레벨+1
								if (DisplayStack < InvSkill.MaxStackCnt)
								{
									DisplayStack++;
								}
								break; //인벤토리에서 찾았으므로 반복 종료
							}
						}
					}
					SavedPassiveSkillData = Skill;

					if (SkillNameText)
					{
						SkillNameText->SetText(Skill.SkillName);
					}
					if (Skill.Icon)
					{
						SkillIcon->SetBrushFromTexture(Skill.Icon);

						SelectedSkillImage = SkillIcon;
					}
					if (SkillDescriptionText)
					{
						SkillDescriptionText->SetText(Skill.Description);
					}
					if (StackText)
					{
						FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack);
						StackText->SetText(FText::FromString(StackLevel));
					}
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

					int32 MaxIndex = ProGI->SkillDataAsset->PassiveSkills.Num() - 1;
					int32 RandomIndex = FMath::RandRange(0, MaxIndex);

					const FPassiveItemData& Skill = ProGI->SkillDataAsset->PassiveSkills[RandomIndex];

					int32 DisplayStack = Skill.StackCnt;

					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

					USkillInventoryComponent* SkillInventory = PlayerCharacter ? PlayerCharacter->FindComponentByClass<USkillInventoryComponent>() : nullptr;

					if (SkillInventory)
					{
						// 현재 인벤토리에서 해당 스킬 타입이 있는지 검색
						for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
						{
							if (InvSkill.Type == Skill.Type)
							{
								DisplayStack = InvSkill.StackCnt; // 현재 인벤토리의 스택 레벨

								// 다음 레벨+1
								if (DisplayStack < InvSkill.MaxStackCnt)
								{
									DisplayStack++;
								}
								break; //인벤토리에서 찾았으므로 반복 종료
							}
						}
					}
					SavedPassiveSkillData = Skill;

					if (SkillNameText)
					{
						SkillNameText->SetText(Skill.SkillName);
					}
					if (Skill.Icon)
					{
						SkillIcon->SetBrushFromTexture(Skill.Icon);

						SelectedSkillImage = SkillIcon;
					}
					if (SkillDescriptionText)
					{
						SkillDescriptionText->SetText(Skill.Description);
					}
					if (StackText)
					{
						FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack);
						StackText->SetText(FText::FromString(StackLevel));
					}
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

					int32 MaxIndex = ProGI->SkillDataAsset->PassiveSkills.Num() - 1;
					int32 RandomIndex = FMath::RandRange(0, MaxIndex);

					const FPassiveItemData& Skill = ProGI->SkillDataAsset->PassiveSkills[RandomIndex];

					int32 DisplayStack = Skill.StackCnt;

					APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

					USkillInventoryComponent* SkillInventory = PlayerCharacter ? PlayerCharacter->FindComponentByClass<USkillInventoryComponent>() : nullptr;

					if (SkillInventory)
					{
						// 현재 인벤토리에서 해당 스킬 타입이 있는지 검색
						for (const FPassiveItemData& InvSkill : SkillInventory->PassiveSkillsInv)
						{
							if (InvSkill.Type == Skill.Type)
							{
								DisplayStack = InvSkill.StackCnt; // 현재 인벤토리의 스택 레벨

								// 다음 레벨+1
								if (DisplayStack < InvSkill.MaxStackCnt)
								{
									DisplayStack++;
								}
								break; //인벤토리에서 찾았으므로 반복 종료
							}
						}
					}
					SavedPassiveSkillData = Skill;

					if (SkillNameText)
					{
						SkillNameText->SetText(Skill.SkillName);
					}
					if (Skill.Icon)
					{
						SkillIcon->SetBrushFromTexture(Skill.Icon);

						SelectedSkillImage = SkillIcon;
					}
					if (SkillDescriptionText)
					{
						SkillDescriptionText->SetText(Skill.Description);
					}
					if (StackText)
					{
						FString StackLevel = FString::Printf(TEXT("Lv.%d"), DisplayStack);
						StackText->SetText(FText::FromString(StackLevel));
					}
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

	//공격력 적용
	if (SavedPassiveSkillData.Type == EPassiveItemType::AttackPowerBoost)
	{
		float BaseToUse = StaticBaseAttackDamage;

		//(최대 스택 제한)
		int32 MaxStackToApply = ExistingSkill ? ExistingSkill->MaxStackCnt : SavedPassiveSkillData.MaxStackCnt;
		int32 AppliedStack = FMath::Min(CurrentStack, MaxStackToApply);

		// 누적 없이 Static 기본 공격력(StaticBaseAttackDamage)을 기준으로 계산
		StatComponent->AttackDamage = BaseToUse * (1.0f + 0.1f * AppliedStack);

		UE_LOG(LogTemp, Warning, TEXT("AttackDamage updated: %.1f (Static Base: %.1f, AppliedStack: %d)"),
			StatComponent->AttackDamage, BaseToUse, AppliedStack);
	}


    //HUD 갱신
    if (UCommonUserWidget_BattleGameHUD* BattleHUD = GetBattleHUD())
    {
		if (!SelectedSkillImage) return;

		// 1번 슬롯
		if (BattleHUD->SkillSlot_1_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_1_Type == EPassiveItemType::None)
		{
			BattleHUD->Image_Skill_1->SetBrush(SelectedSkillImage->GetBrush());
			BattleHUD->Image_Skill_1->SetColorAndOpacity(FLinearColor::White);

			int32 DisplayStack = FMath::Min(CurrentStack, 5);
			BattleHUD->StackCnt_1->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayStack)));
			BattleHUD->StackCnt_1->SetVisibility(ESlateVisibility::Visible);

			BattleHUD->SkillSlot_1_Type = SavedPassiveSkillData.Type;
		}

		// 2번 슬롯
		else if (BattleHUD->SkillSlot_2_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_2_Type == EPassiveItemType::None)
		{
			BattleHUD->Image_Skill_2->SetBrush(SelectedSkillImage->GetBrush());
			BattleHUD->Image_Skill_2->SetColorAndOpacity(FLinearColor::White);

			int32 DisplayStack = FMath::Min(CurrentStack, 5);
			BattleHUD->StackCnt_2->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayStack)));
			BattleHUD->StackCnt_2->SetVisibility(ESlateVisibility::Visible);

			BattleHUD->SkillSlot_2_Type = SavedPassiveSkillData.Type;
		}

		// 3번 슬롯
		else if (BattleHUD->SkillSlot_3_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_3_Type == EPassiveItemType::None)
		{
			BattleHUD->Image_Skill_3->SetBrush(SelectedSkillImage->GetBrush());
			BattleHUD->Image_Skill_3->SetColorAndOpacity(FLinearColor::White);

			int32 DisplayStack = FMath::Min(CurrentStack, 5);
			BattleHUD->StackCnt_3->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayStack)));
			BattleHUD->StackCnt_3->SetVisibility(ESlateVisibility::Visible);

			BattleHUD->SkillSlot_3_Type = SavedPassiveSkillData.Type;
		}

		// 4번 슬롯
		else if (BattleHUD->SkillSlot_4_Type == SavedPassiveSkillData.Type || BattleHUD->SkillSlot_4_Type == EPassiveItemType::None)
		{
			BattleHUD->Image_Skill_4->SetBrush(SelectedSkillImage->GetBrush());
			BattleHUD->Image_Skill_4->SetColorAndOpacity(FLinearColor::White);

			int32 DisplayStack = FMath::Min(CurrentStack, 5);
			BattleHUD->StackCnt_4->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"), DisplayStack)));
			BattleHUD->StackCnt_4->SetVisibility(ESlateVisibility::Visible);

			BattleHUD->SkillSlot_4_Type = SavedPassiveSkillData.Type;
		}
        // 뷰포트 갱신
        if (UWorld* World = GetWorld())
        {
            if (UGameViewportClient* ViewportClient = World->GetGameViewport())
            {
                ViewportClient->RemoveAllViewportWidgets();
            }
        }
        BattleHUD->AddToViewport();
    }
}

UCommonUserWidget_BattleGameHUD* UCommonUserWidget_Skill_Slot::GetBattleHUD()
{
	if (UWorld* World = GetWorld())
	{
		TArray<UUserWidget*> FoundWidgets;

		// 현재 월드에 존재하는 BattleGameHUD 클래스의 위젯을 검색
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundWidgets, UCommonUserWidget_BattleGameHUD::StaticClass(), false);

		if (FoundWidgets.Num() > 0)
		{
			return Cast<UCommonUserWidget_BattleGameHUD>(FoundWidgets[0]);
		}
	}
	return nullptr;
}


