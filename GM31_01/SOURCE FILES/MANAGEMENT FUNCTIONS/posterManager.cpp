#include "posterManager.h"


std::vector<POSTPROCESS*> PosterManager::posters;

void PosterManager::Init()
{
	AddPoster<PostProcessMaterial>();
	AddPoster<LuminanceMaterial>();
	AddPoster<BloomMaterial>();
	AddPoster<MergeMaterial>();
}

void PosterManager::Uninit()
{
	for (POSTPROCESS* post : posters)
	{
		post->UnInitialize();
		delete post;
	}
}

void PosterManager::Update()
{
	for (POSTPROCESS* post : posters)
	{
		post->Update();
	}
}

void PosterManager::Draw()
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

	//Renderer::BeginPostProcess();
	//Renderer::SetDefaultViewPort();
	//GetPoster<MergeMaterial>()->Draw();

	Renderer::Begin();
	Renderer::SetDefaultViewPort();
	GetPoster<PostProcessMaterial>()->Draw();
}
