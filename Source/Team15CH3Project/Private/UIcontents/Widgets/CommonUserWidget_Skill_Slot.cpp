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


void UCommonUserWidget_Skill_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	SetupInitialData();

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
						FString StackLevel = FString::Printf(TEXT("Lv.%d"), Skill.StackCnt);
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
						FString StackLevel = FString::Printf(TEXT("Lv.%d"), Skill.StackCnt);
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
						FString StackLevel = FString::Printf(TEXT("Lv.%d"), Skill.StackCnt);
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

	int32 CurrentStack = 0;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		USkillInventoryComponent* SkillInventory = PlayerCharacter->FindComponentByClass<USkillInventoryComponent>();
		UCharacterStatsComponent* StatComponent = PlayerCharacter->FindComponentByClass<UCharacterStatsComponent>();

		if (SkillInventory && StatComponent)
		{
			// 스킬 추가 및 스택 증가
			SkillInventory->AddPassiveSkill(SavedPassiveSkillData);

			// 최신 스택 가져오기
			for (const FPassiveItemData& Skill : SkillInventory->PassiveSkillsInv)
			{
				if (Skill.Type == SavedPassiveSkillData.Type)
				{
					CurrentStack = Skill.StackCnt;
					break;
				}
			}

			// 공격력 적용 (BaseAttackDamage 기준)
			if (SavedPassiveSkillData.Type == EPassiveItemType::AttackPowerBoost)
			{
				if (BaseAttackDamage <= 0.0f)
				{
					BaseAttackDamage = StatComponent->AttackDamage; //재확인
				}

				float NewDamage = BaseAttackDamage * (1.0f + 0.1f * CurrentStack);
				StatComponent->AttackDamage = NewDamage;

				UE_LOG(LogTemp, Warning, TEXT("AttackDamage updated: %.1f (Stack: %d)"), StatComponent->AttackDamage, CurrentStack);
			}

			//=============================================================================== Image & HUD Update
			UCommonUserWidget_BattleGameHUD* BattleHUD = GetBattleHUD();
			if (BattleHUD)
			{
				struct FSkillSlotInfo
				{
					UImage* Image;
					UTextBlock* StackText;
					EPassiveItemType Type;
					int32 Stack;
				};

				static TArray<FSkillSlotInfo> SkillSlots;

				for (int32 i = 0; i < SkillSlots.Num(); i++)
				{
					if (!IsValid(SkillSlots[i].Image) || !IsValid(SkillSlots[i].StackText))
					{
						UE_LOG(LogTemp, Warning, TEXT("Invalid SkillSlot found. Clearing SkillSlots."));
						SkillSlots.Empty();
						break;
					}
				}

				// 초기 슬롯 설정 (처음 실행 시 한 번만)
				if (SkillSlots.Num() == 0)
				{
					SkillSlots.SetNum(4);
					SkillSlots[0] = { BattleHUD->Image_Skill_1, BattleHUD->StackCnt_1, EPassiveItemType::None, 0 };
					SkillSlots[1] = { BattleHUD->Image_Skill_2, BattleHUD->StackCnt_2, EPassiveItemType::None, 0 };
					SkillSlots[2] = { BattleHUD->Image_Skill_3, BattleHUD->StackCnt_3, EPassiveItemType::None, 0 };
					SkillSlots[3] = { BattleHUD->Image_Skill_4, BattleHUD->StackCnt_4, EPassiveItemType::None, 0 };
				}

				bool bFoundSameType = false;

				// 이미 같은 타입의 스킬이 있는지 확인
				for (FSkillSlotInfo& SkillSlot : SkillSlots)
				{
					if (SkillSlot.Type == SavedPassiveSkillData.Type)
					{
						bFoundSameType = true;

						// 스택 증가 (최대 5)
						if (SkillSlot.Stack < 5)
						{
							SkillSlot.Stack++;
							FString LevelText = FString::Printf(TEXT("Lv.%d"), SkillSlot.Stack);
							if (SkillSlot.StackText)
							{
								SkillSlot.StackText->SetText(FText::FromString(LevelText));
								SkillSlot.StackText->SetVisibility(ESlateVisibility::Visible);
							}
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Skill %d is already at max stack (5)."), (int32)SavedPassiveSkillData.Type);
						}

						break;
					}
				}

				// 같은 타입이 없다면 새 슬롯에 추가
				if (!bFoundSameType)
				{
					for (FSkillSlotInfo& SkillSlot : SkillSlots)
					{
						if (SkillSlot.Type == EPassiveItemType::None)
						{
							SkillSlot.Type = SavedPassiveSkillData.Type;
							SkillSlot.Stack = 1;

							if (SkillSlot.Image)
							{
								SkillSlot.Image->SetBrush(SelectedSkillImage->GetBrush());
								SkillSlot.Image->SetColorAndOpacity(FLinearColor::White);
							}

							if (SkillSlot.StackText)
							{
								SkillSlot.StackText->SetText(FText::FromString(TEXT("Lv.1")));
								SkillSlot.StackText->SetVisibility(ESlateVisibility::Visible);
							}

							break;
						}
					}
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

void UCommonUserWidget_Skill_Slot::SetupInitialData()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		if (UCharacterStatsComponent* StatComponent = PlayerCharacter->FindComponentByClass<UCharacterStatsComponent>())
		{
			// 캐릭터 스테이트의 데미지 초기값 가져오기
			BaseAttackDamage = StatComponent->AttackDamage;
			UE_LOG(LogTemp, Log, TEXT("BaseAttackDamage initialized to: %.1f"), BaseAttackDamage);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CharacterStatsComponent NOT found during SetupInitialData."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter NOT found during SetupInitialData."));
	}
}