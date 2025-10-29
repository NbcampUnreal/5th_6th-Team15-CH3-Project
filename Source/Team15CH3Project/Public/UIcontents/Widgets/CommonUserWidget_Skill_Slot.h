#pragma once

#include "CoreMinimal.h"
#include "UIcontents/Widgets/CommonUserWidget_Skill.h"
#include "Blueprint/UserWidget.h"
#include "Skill/passiveItem.h"
#include "Skill/ActiveSkillItem.h"
#include "Skill/SkillDataAsset.h"
#include "CommonUserWidget_Skill_Slot.generated.h"

class UCommonUserWidget_BattleGameHUD;


UCLASS()
class TEAM15CH3PROJECT_API UCommonUserWidget_Skill_Slot : public UCommonUserWidget_Skill
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SkillNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SkillDescriptionText;

    UPROPERTY(meta = (BindWidget))
    class UImage* SkillIcon;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StackText;

    UPROPERTY(meta = (BindWidget))
    class UButton* SkillButton;

    UFUNCTION()
    void OnSkillButtonClicked();

    UCommonUserWidget_BattleGameHUD* GetBattleHUD();

    UPROPERTY()
    class UImage* SelectedSkillImage;

    UPROPERTY()
    FPassiveItemData SavedPassiveSkillData;

    UPROPERTY()
    float BaseAttackDamage;

    void SetupInitialData();

    static int32 LastAssignedSlotIndex;
};
