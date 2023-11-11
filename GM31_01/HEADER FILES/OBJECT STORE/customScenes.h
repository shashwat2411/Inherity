#pragma once
#include "../MANAGEMENT FUNCTIONS/scene.h"
#include "../manager.h"
#include "../PRIVATE PATTERN/soundReader.h"

class EMPTY_SCENE : public SCENE
{
public:

	void Init() override {}
	void Update() override {}
};
class TITLE_SCENE : public SCENE
{
public:

	void Init() override;
	void Update() override;
};
class GAME_SCENE : public SCENE
{
public:

	void Init() override;
	void Update() override;
};
class RESULT_SCENE : public SCENE
{
public:

	void Init() override;
	void Update() override;
};