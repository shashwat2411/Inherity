#include "postProcessManager.h"


std::vector<POSTPROCESS*> PostProcessManager::posters{};

void PostProcessManager::Init()
{
	posters.emplace_back();
	AddPoster<PostProcessMaterial>();
	//AddPoster<LuminanceMaterial>();
	//AddPoster<BloomMaterial>();
	//AddPoster<MergeMaterial>();
}

void PostProcessManager::Uninit()
{
	for (GAMEOBJECT* post : posters)
	{
		post->UnInitialize();
		delete post;
	}
}

void PostProcessManager::Update()
{
	for (GAMEOBJECT* post : posters)
	{
		post->Update();
	}
}

void PostProcessManager::Draw()
{
	//Renderer::BeginLuminence();
	//Renderer::SetBloomViewport();

	//GetPoster<LuminanceMaterial>()->Draw();
	////GetPoster<LUMINANCE>()->Draw();

	//POSTPROCESS* bloom = GetPoster<BloomMaterial>();
	////BLOOM* bloom = GetPoster<BLOOM>();
	//for (int i = 0; i < 6; i++)
	//{
	//	Renderer::BeginBloom(i);
	//	Renderer::SetBloomViewport((i < 4 ? i : -1));

	//	bloom->GetMaterial()->SetInt("_Index", i);
	//	bloom->Draw();
	//}

	//Renderer::Begin();
	//Renderer::SetDefaultViewPort();
	//GetPoster<MergeMaterial>()->Draw();

	Renderer::Begin();
	Renderer::SetDefaultViewPort();
	posters[0]->Draw();
}
