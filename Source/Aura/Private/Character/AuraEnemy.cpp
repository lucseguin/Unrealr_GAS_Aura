// Copyright Luc Seguin


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	EnableHighlight(true);
}

void AAuraEnemy::UnHighlightActor()
{
	EnableHighlight(false);
}

void AAuraEnemy::EnableHighlight(bool ShouldHighlight)
{
	GetMesh()->SetRenderCustomDepth(ShouldHighlight);
	Weapon->SetRenderCustomDepth(ShouldHighlight);
}
