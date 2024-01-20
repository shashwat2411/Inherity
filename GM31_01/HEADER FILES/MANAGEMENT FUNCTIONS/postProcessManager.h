
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
		posters.push_back(post);

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

};