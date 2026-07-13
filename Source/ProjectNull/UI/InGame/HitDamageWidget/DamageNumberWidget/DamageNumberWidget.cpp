//----------------------------------------------------------
//		Includes
//----------------------------------------------------------
#include "DamageNumberWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

//----------------------------------------------------------
//		ダメージの反映
//----------------------------------------------------------
void UDamageNumberWidget::SetDamageValue(int32 Damage, bool bCritical)
{
	if (!DamageText)
	{
		return;
	}

	FText Text;

	if (bCritical)
	{
		Text = FText::FromString(FString::Printf(TEXT("%d!!"), Damage));
	}
	else
	{
		Text = FText::AsNumber(Damage);
	}

	DamageText->SetText(Text);

}

//----------------------------------------------------------
//		アニメーション再生
//----------------------------------------------------------
void UDamageNumberWidget::PlayPopupAnimation()
{
	if (PopupAnim)
	{
		PlayAnimation(PopupAnim, 0.0f, 1);
	}
}
