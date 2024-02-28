#include "../saveFunctions.h"

class PostProcessManager
{
private:
	static std::vector<POSTPROCESS*> posters;

public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	template <typename T>
	static POSTPROCESS* AddPoster()
	{
		POSTPROCESS* post = new POSTPROCESS();
		posters[0] = post;

		post->Init();
		post->AddMaterial<T>();

		return post;
	}

	template<typename T>
	static POSTPROCESS* GetPoster()
	{
		for (auto post : posters)
		{
			T* buff = dynamic_cast<T*>(post->GetMaterial());
			if (buff != nullptr)
			{
				return post;
			}
		}
		return nullptr;
	}

	template<typename T>
	static void RemovePoster()
	{
		POSTPROCESS* buff = GetPoster<T>();
		if (buff != nullptr)
		{
			auto it = std::find(posters.begin(), posters.end(), buff);

			buff->UnInitialize();
			if (it != posters.end()) { posters.erase(it); }
			delete buff;
		}
	}
};