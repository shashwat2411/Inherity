#pragma once
#include "manager.h"
#include "soundReader.h"
#include "input.h"

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