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

// 플레이어 참조 가져오기
APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
if (!PlayerCharacter)
{
    UE_LOG(LogTemp, Error, TEXT("PlayerCharacter not found."));
    return;
}

// 컴포넌트 가져오기
USkillInventoryComponent* SkillInventory = PlayerCharacter->FindComponentByClass<USkillInventoryComponent>();
UCharacterStatsComponent* StatComponent = PlayerCharacter->FindComponentByClass<UCharacterStatsComponent>();

if (!SkillInventory || !StatComponent)
{
    UE_LOG(LogTemp, Error, TEXT("SkillInventory or StatComponent not found."));
    return;
}

// --- 배열에서 스킬 검색 ---
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

if (ExistingSkill)
{
    // 이미 존재하면 스택 증가
    ExistingSkill->StackCnt++;
    CurrentStack = ExistingSkill->StackCnt;
    UE_LOG(LogTemp, Log, TEXT("Existing skill '%s' found. Stack increased to %d."), *ExistingSkill->SkillName.ToString(), CurrentStack);
}
else
{
    // 새 스킬 추가
    FPassiveItemData NewSkill = SavedPassiveSkillData;
    NewSkill.StackCnt = 1;

    SkillInventory->PassiveSkillsInv.Add(NewSkill);
    CurrentStack = NewSkill.StackCnt;

    UE_LOG(LogTemp, Log, TEXT("New skill '%s' added to inventory. Stack: %d"), *NewSkill.SkillName.ToString(), CurrentStack);
}

// --- 공격력 적용 ---
if (SavedPassiveSkillData.Type == EPassiveItemType::AttackPowerBoost)
{
    // BaseAttackDamage가 0이면 StatComponent에서 가져오기
    if (BaseAttackDamage <= 0.0f)
    {
        BaseAttackDamage = StatComponent->AttackDamage;
    }

    float NewDamage = BaseAttackDamage * (1.0f + 0.1f * CurrentStack);
    StatComponent->AttackDamage = NewDamage;

    UE_LOG(LogTemp, Warning, TEXT("AttackDamage updated: %.1f (Stack: %d)"), StatComponent->AttackDamage, CurrentStack);
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
