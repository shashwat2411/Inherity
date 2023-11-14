#include "baseobjects.h"
#include "input.h"

void BAR::Init() 
{
	Initialize();

	amount = 0.0f;
	maxAmount = 100.0f;

	transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50.0f, 0.0f);
	transform->Scale = D3DXVECTOR3(1.5f, 0.15f, 0.5f);

	sprite = AddComponent<SpriteRenderer>();

	SetBarType();
}

void BAR::Update() 
{
	if (Input::GetKeyPress('Q')) { Amount(0.5f); }
	if (Input::GetKeyPress('E')) { Amount(-0.5f); }

	if (type == RIGHT)
	{
		sprite->barOffsetRight = ((maxAmount - amount) / maxAmount) * -(sprite->GetSize().x * transform->Scale.x * 2.0f);
	}
	else if (type == LEFT)
	{
		sprite->barOffsetLeft = ((maxAmount - amount) / maxAmount) * (sprite->GetSize().x * transform->Scale.x * 2.0f);
	}
	else if (type == MIDDLE)
	{
		sprite->barOffsetRight = ((maxAmount - amount) / maxAmount) * -(sprite->GetSize().x * transform->Scale.x);
		sprite->barOffsetLeft = ((maxAmount - amount) / maxAmount) * (sprite->GetSize().x * transform->Scale.x);
	}

#ifdef DEBUG	// デバッグ情報を表示する
	char* str = GetDebugStr();
	//sprintf(&str[strlen(str)], " | Amount : %.2f", amount);
	//sprintf(&str[strlen(str)], " | barOffsetRight : %.2f", sprite->barOffsetRight);
	//sprintf(&str[strlen(str)], " | barOffsetLeft : %.2f", sprite->barOffsetLeft);
#endif
}

void BAR::SetBarType(BAR_TYPE value)
{
	type = value;

	if (type == RIGHT)
	{
		sprite->barOffsetRight = -(sprite->GetSize().x * transform->Scale.x) * 2.0f;
	}
	else if (type == LEFT)
	{
		sprite->barOffsetLeft = (sprite->GetSize().x * transform->Scale.x) * 2.0f;
	}
	else if (type == MIDDLE)
	{
		sprite->barOffsetRight = -(sprite->GetSize().x * transform->Scale.x);
		sprite->barOffsetLeft = (sprite->GetSize().x * transform->Scale.x);
	}
}

void BAR::Amount(float value)
{
	if (value >= 0.0f)
	{
		if ((amount + value) <= maxAmount) { amount += value; }
		else { amount = maxAmount; }
	}
	else if (value < 0.0f)
	{
		if ((amount + value) >= 0.0f) { amount += value; }
		else { amount = 0.0f; }
	}
}