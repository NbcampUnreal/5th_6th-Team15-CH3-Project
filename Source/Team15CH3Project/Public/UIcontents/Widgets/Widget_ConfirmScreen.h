#pragma once

#include "CoreMinimal.h"
#include "UIcontents/Widgets/Widget_ActivatableBase.h"
#include "UIcontents/FrontendTypes/FrontendEnumtendTypes.h"
#include "Widget_ConfirmScreen.generated.h"


class UCommonTextBlock;
class UDynamicEntryBox;


UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class TEAM15CH3PROJECT_API UWidget_ConfirmScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;

	UPROPERTY(meta = (BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons;
};
