#include "component.h"
#include "baseobjects.h"

void Number::Start()
{
	digits = 3;
	number = 0;


	//----------------------------------------------------------------
	for (int i = 0; i < digits; i++)
	{
		auto b = new DIGIT();
		b->Start();

		b->GetComponent<SpriteRenderer>()->elementsX = 5;
		b->GetComponent<SpriteRenderer>()->elementsY = 5;
		b->GetComponent<SpriteRenderer>()->animate = true;
		numbers.push_back(b);
	}


}

void Number::End()
{
	for (auto num : numbers)
	{
		num->UnInitialize();
		delete num;
	}
}

void Number::Update()
{
	int i = digits / 2;
	int c = 0;
	int d = digits;

	for (auto num : numbers)
	{
		num->transform->Scale = gameObject->transform->Scale;

		d--;
		int dig = number % (int)pow(10, digits - d) / (int)pow(10, digits - (d + 1));
		num->GetComponent<SpriteRenderer>()->count = dig;

		num->transform->Position.x = gameObject->transform->Position.x + i * num->GetComponent<SpriteRenderer>()->GetSize().x * 1.3f * gameObject->transform->Scale.x;
		num->transform->Position.y = gameObject->transform->Position.y;
		if (num->GetComponent<SpriteRenderer>()->count >= 10) { num->GetComponent<SpriteRenderer>()->count = 0; }
		i--;
		c++;
	}
}

void Number::Draw()
{
	for (auto num : numbers)
	{
		num->Draw();
	}
}

void Number::SetDigits(int num)
{
	int diff = num - digits;
	while (diff > 0)
	{
		auto b = new DIGIT();
		
		b->Initialize();
		b->Init();
		b->Start();
		
		b->GetComponent<SpriteRenderer>()->elementsX = 5;
		b->GetComponent<SpriteRenderer>()->elementsY = 5;
		b->GetComponent<SpriteRenderer>()->animate = true;
		numbers.push_back(b);
		diff--;
		digits++;
	}
	while (diff < 0)
	{
		numbers.back()->UnInitialize();
		delete numbers.back();
		numbers.pop_back();
		diff++;
		digits--;
	}
}

void Number::SetNumberColor(D3DXCOLOR value)
{
	for (auto num : numbers)
	{
		num->SetColor(value);
	}
}
