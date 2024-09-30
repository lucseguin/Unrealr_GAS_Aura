// Copyright Luc Seguin


#include "Character/AuraEnemy.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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
